#include "TennisState_BoundBallAtk.h"
#include "../CharacterFunction.h"
#include "Tennis_HitEvent.h"
#include "TennisPlayerState_UsualMove.h"
#include "../../Render/MeshRenderer.h"
#include "../../Ball/UsualBall.h"
#include "../../Collision/Collision.h"
#include "../../Ball/BallFadeOutRenderer.h"
#include "../../Effect/HitEffectObject.h"
#include "../../Camera/Camera.h"

TennisState_BoundBallAtk::TennisState_BoundBallAtk(
    ControllClass* pControllClass   //バウンド方向をコントロールするクラス(終了時にdeleteする)
    ):
    m_pControllClass(pControllClass),
    m_Timer(0)
{

}

TennisState_BoundBallAtk::~TennisState_BoundBallAtk()
{
    delete m_pControllClass;
}


void TennisState_BoundBallAtk::Enter(TennisPlayer* t)
{
    t->m_Renderer.SetMotion(TennisPlayer::_mt_BoundSmash);

    chr_func::ResetMove(t);
}


void TennisState_BoundBallAtk::Execute(TennisPlayer* t)
{
    const int SmashFrame = 15;
    const int EndFrame = 25;

    //カウント更新
    ++m_Timer;

    //着地点オブジェクトを生成
    if (m_Timer == 1)
    {

    }

    //発射
    if (m_Timer == SmashFrame)
    {
        BallBase::Params param;

        param.pos = t->m_Params.pos + Vector3(0, BallBase::UsualBallShotY, 0);
        param.pParent = t;
        param.type = BallBase::Type::_CantCounter;


        chr_func::GetFront(t, &param.move);
        param.move *= 0.5f;
        param.move.y = 0.8f;

        new TennisBoundBall(
            param,
            -0.04f,
            Vector3AxisX
            );
    }

    //終了で通常ステートへ
    if (m_Timer == EndFrame)
    {
        t->SetState(TennisState_PlayerControll_Move::GetPlayerControllMove(t));
    }

    //基本的な更新
    {
        TennisHitEvent HitEvent(t);
        chr_func::UpdateAll(t, &HitEvent);

        //モデル更新
        t->m_Renderer.Update(1);

        //行列更新
        chr_func::CreateTransMatrix(t, t->m_ModelSize, &t->m_Renderer.m_TransMatrix);

    }
}


void TennisState_BoundBallAtk::Exit(TennisPlayer* t)
{

}



//****************************************************
//	テニス_バウンドするボールクラス
//****************************************************


TennisBoundBall::TennisBoundBall(
    BallBase::Params	params,			//ボールパラメータ
    float               glavity,        //重力
    CrVector3           AtkVec          //バウンドする方向
    ) :
    m_Locus(8),
    m_pStateFunc(&TennisBoundBall::State_Flying),
    m_Glavity(glavity),
    m_Timer(0),
    m_MoveVec(AtkVec)
{
    m_Params = params;

    //メッシュ作成
    {
        iexMesh* pMesh = nullptr;

        UsualBall::GetBallMesh(params.pParent->m_PlayerInfo.chr_type, &pMesh);

        m_pMeshRenderer = new MeshRenderer(pMesh, false, MeshRenderer::RenderType::UseColor);
    }

    {
        //ワールド変換行列の更新
        D3DXQuaternionIdentity(&m_BallRot);
        UpdateTransMatrix(0.5f);
    }

    {
        //軌跡
        m_Locus.m_StartParam.Width = 0.25f;
        m_Locus.m_EndParam.Width = 0.05f;

        m_Locus.m_EndParam.Color = Vector4(1, 1, 1, 0);
    }
}

TennisBoundBall::~TennisBoundBall()
{
    delete m_pMeshRenderer;
}

bool TennisBoundBall::Update()
{
    {
        Vector3 vec;

        Vector3Cross(vec, DefCamera.GetForward(), m_Params.move);
        vec.Normalize();

        m_Locus.AddPoint(m_Params.pos, vec);
    }


    (this->*m_pStateFunc)();

    
    
    return m_pStateFunc != &TennisBoundBall::State_Finish;
}

bool TennisBoundBall::Msg(MsgType mt)
{
    return false;
}

void TennisBoundBall::UpdateTransMatrix(RATIO scaleYratio)
{
    Matrix M;

    GetTransMatrix(M, scaleYratio);

    m_pMeshRenderer->SetMatrix(M);
}

//ワールド変換行列を算出する
void TennisBoundBall::GetTransMatrix(Matrix& m,RATIO scaleYratio)
{
    Matrix T;

    {
        const float scale = UsualBall::GetBallScale(m_Params.pParent->m_PlayerInfo.chr_type);
        D3DXMatrixScaling(&m, scale, scale*scaleYratio, scale);
    }

    {
        D3DXMatrixRotationQuaternion(&T, &m_BallRot);
        m *= T;
    }

    {
        m._41 = m_Params.pos.x;
        m._42 = m_Params.pos.y;
        m._43 = m_Params.pos.z;
    }

}


void TennisBoundBall::RotateY()
{
    //Ｙ軸回転や！
    {
        D3DXQUATERNION q;

        D3DXQuaternionRotationYawPitchRoll(&q, D3DXToRadian(10), 0, 0);

        m_BallRot *= q;
    }
}



void TennisBoundBall::State_Flying()
{
    m_Params.move.y += m_Glavity;
    m_Params.pos += m_Params.move;

    RotateY();
    UpdateTransMatrix(0.5f);

    {
        Vector3 out, pos(m_Params.pos), vec(m_Params.move);
        float dist = m_Params.move.Length()*1.5f;
        int material;

        vec.Normalize();

        if (
            DefCollisionMgr.RayPick(
            &out,
            &pos,
            &vec,
            &dist,
            &material,
            CollisionManager::RayType::_Usual
            ) != nullptr)
        {
            //あたった面が大体上向きで、高低さもない場合成功
            if (
                Vector3Radian(Vector3AxisY, vec) < D3DXToRadian(45) &&
                BallBase::UsualBallShotY > out.y
                )
            {
                m_Params.pos = out + Vector3(0, 0.5f, 0);
                m_pStateFunc = &TennisBoundBall::State_Rolling;
            }
            else
            {
                //失敗の場合(壁に当たった)
                m_pStateFunc = &TennisBoundBall::State_Failed;
            }
        }
    }
}

void TennisBoundBall::State_Failed()
{
    iexMesh*   pMesh;
    Matrix     BaseMatrix;
    RigidBody* pRigidBody;

    UsualBall::GetBallMesh(m_Params.pParent->m_PlayerInfo.chr_type, &pMesh);

    UpdateTransMatrix(1);

    BaseMatrix = m_pMeshRenderer->GetMatrix();

    BaseMatrix._41 = 0;
    BaseMatrix._42 = 0;
    BaseMatrix._43 = 0;

    pRigidBody = DefBulletSystem.AddRigidSphere(
        1.5f,
        RigidBody::ct_dynamic,
        m_Params.pos,
        Vector3Zero,
        0.5f,
        0.8f,
        0.25f,
        m_Params.move * 30.0f
        );

    new BallFadeOutRenderer(
        pMesh,
        BaseMatrix,
        pRigidBody,
        60
        );

    m_pStateFunc = &TennisBoundBall::State_Finish;
}

void TennisBoundBall::State_Rolling()
{
    const int RollingFrame = 60;

    if (++m_Timer == RollingFrame)
    {
        m_pStateFunc = &TennisBoundBall::State_AtkStart;
    }
}

void TennisBoundBall::State_AtkStart()
{
    //エフェクト
    {
        const COLORf Color = CharacterBase::GetPlayerColorF(m_Params.pParent->m_PlayerInfo.number);

        //ヒットエフェクト作成
        new HitEffectObject(
            m_Params.pos,
            Vector3Normalize(m_MoveVec),
            0.05f,
            0.15f,
            Vector3(Color.r, Color.g, Color.b)
        );
    }

    //ボール発射
    {
        BallBase::Params param = m_Params;

        //移動量
        param.move = Vector3Normalize(m_MoveVec);
        //スピードは適当
        param.move *= 1.0f;
        //高さをキャラ共通ボール発射のYに(違和感あるかもな～
        param.pos.y = BallBase::UsualBallShotY;
        //通常タイプ
        param.type = BallBase::Type::_Usual;

        //生成
        new UsualBall(param, DamageBase::Type::_WeekDamage, 1);
    }

    m_pStateFunc = &TennisBoundBall::State_Finish;
}
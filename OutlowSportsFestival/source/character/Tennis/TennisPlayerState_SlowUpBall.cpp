#include "TennisPlayerState_SlowUpBall.h"
#include "../CharacterFunction.h"
#include "Tennis_HitEvent.h"
#include "../../Render/MeshRenderer.h"
#include "../../Ball/UsualBall.h"
#include "../../Ball/BallFadeOutRenderer.h"
#include "TennisPlayerState_UsualMove.h"

//*****************************************************
//		投げ上げるボールクラス
//*****************************************************

TennisUpBall::TennisUpBall(
    CrVector3     pos,
    TennisPlayer* pTennis
    ) :
    m_Pos(pos),
    m_MoveY(0.32f),
    m_SlowFlag(true),
    m_Timer(0),
    m_pTennis(pTennis),
    m_pRigidBody(nullptr),
    m_pStateFunc(&TennisUpBall::State_SlowFly)
{
    {
        iexMesh* pMesh = nullptr;

        UsualBall::GetBallMesh(CharacterType::_Tennis, &pMesh);

        m_pMeshRenderer = new MeshRenderer(
            pMesh,
            false,
            MeshRenderer::RenderType::UseColorSpecular
        );
    }

    //メッシュ初期更新
    UsualMeshUpdate();
}


TennisUpBall::~TennisUpBall()
{
    delete m_pMeshRenderer;
}


bool TennisUpBall::Update()
{
    //ステート関数を実行
    (this->*m_pStateFunc)();


    return m_pStateFunc != &TennisUpBall::State_Destroy;
}

bool TennisUpBall::Msg(MsgType mt)
{
    return false;
}

void TennisUpBall::Destory()
{
    m_pStateFunc = &TennisUpBall::State_Destroy;
}

void TennisUpBall::DoRealTime()
{
    m_SlowFlag = false;
}

void TennisUpBall::UsualMeshUpdate()
{
    const float s = UsualBall::GetBallScale(CharacterType::_Tennis);
    Matrix m;

    D3DXMatrixScaling(&m, s, s, s);
    
    m._41 = m_Pos.x;
    m._42 = m_Pos.y;
    m._43 = m_Pos.z;

    m_pMeshRenderer->SetMatrix(m);
}

bool TennisUpBall::isHitFloor()const
{
    return m_Pos.y <= 0.0f;
}



void TennisUpBall::State_SlowFly()
{
    //投げ上げた状態

    const int UpFrame = 5;
    const int DownFrame = 60;

    const float SlowValue = 0.1f;
    const float Glavity = -0.01f;

    //スローする条件
    if (m_SlowFlag &&
        (UpFrame <= m_Timer && DownFrame >= m_Timer)
        )
    {
        m_MoveY += Glavity*SlowValue;
        m_Pos.y += m_MoveY*SlowValue;
    }
    else
    {
        //通常の速度
        m_MoveY += Glavity;
        m_Pos.y += m_MoveY;
    }

    //メッシュ更新
    UsualMeshUpdate();

    //床に当たっていたら物理挙動に
    if (isHitFloor())
    {
        m_Timer = 0;
        m_pStateFunc = &TennisUpBall::State_PhysicMove;
    }
}

void TennisUpBall::State_PhysicMove()
{
    if (m_pRigidBody == nullptr)
    {
        //剛体作成

        UsualBall::PhysicsParam p = UsualBall::GetBallPhysics(CharacterType::_Tennis);

        m_pRigidBody = DefBulletSystem.AddRigidSphere(
            p.Mass,
            RigidBody::CollisionTypes::ct_dynamic,
            m_Pos,
            Vector3Zero,
            p.Radius,
            p.Friction,
            p.Restitution,
            Vector3(0, -m_MoveY, 0)*40.0f
            );
    }

    //メッシュ更新
    const float s = UsualBall::GetBallScale(CharacterType::_Tennis);
    Matrix m;

    m_pRigidBody->Get_TransMatrix(m);

    m._11 *= s;
    m._12 *= s;
    m._13 *= s;

    m._21 *= s;
    m._22 *= s;
    m._23 *= s;

    m._31 *= s;
    m._32 *= s;
    m._33 *= s;

    m_pMeshRenderer->SetMatrix(m);

    //一定時間でフェードアウト
    if (++m_Timer > 120)
    {
        m_pStateFunc = &TennisUpBall::State_CreateFadeOut;
    }
}

void TennisUpBall::State_CreateFadeOut()
{
    //フェードアウト用ボールの作成
    {
        const float s = UsualBall::GetBallScale(CharacterType::_Tennis);
        Matrix m, ms;
        iexMesh* pMesh;

        UsualBall::GetBallMesh(CharacterType::_Tennis, &pMesh);

        m_pRigidBody->Get_TransMatrix(m);
        D3DXMatrixScaling(&ms, s, s, s);

        m._41 = 0;
        m._42 = 0;
        m._43 = 0;

        m *= ms;

        new BallFadeOutRenderer(
            pMesh,
            m,
            m_pRigidBody,
            60
            );

    }

    //自身で開放しないようにnullに設定しておく
    m_pRigidBody = nullptr;


    //消去ステートへ
    m_pStateFunc = &TennisUpBall::State_Destroy;
}

void TennisUpBall::State_Destroy()
{
    //何もしない

}



//****************************************************
//	テニス_ボールを上に上げるクラス
//****************************************************


TennisState_SlowUpBall::TennisState_SlowUpBall(
    ControllClass* pControllClass   //操作コントロールするクラス(終了時にdeleteする)
    ) :
    m_pControllClass(pControllClass),
    m_Timer(0),
    m_EndFrame(80)
{

}


TennisState_SlowUpBall::~TennisState_SlowUpBall()
{
    delete m_pControllClass;
}


void TennisState_SlowUpBall::Enter(TennisPlayer* t)
{
    t->m_Renderer.SetMotion(TennisPlayer::_mt_Smash);
}

void TennisState_SlowUpBall::Execute(TennisPlayer* t)
{
    const int SlowFrame = 3;
    const int CanSwitchFrame  = SlowFrame + 2;
    const int DontSwitchFrame = CanSwitchFrame + 60;
    const int EndFrame = 100;

    //カウンター更新
    ++m_Timer;

    //移動値の現象
    {
        chr_func::XZMoveDown(t, 0.1f);
    }

    //派生アクションの入力更新
    if (m_Timer >= CanSwitchFrame && m_Timer <= DontSwitchFrame)
    {
        m_pControllClass->SwitchState(this);
    }

    //ボールを投げる
    if (m_Timer == SlowFrame)
    {
        new TennisUpBall(
            t->m_Params.pos + Vector3(0, 3, 0),
            t
            );
    }

    //時間経過で通常移動ステートへ
    if (m_Timer >= EndFrame)
    {
        t->SetState(TennisState_PlayerControll_Move::GetPlayerControllMove(t));
    }

    //基本的な更新↓
    TennisHitEvent HitEvent(t);

    chr_func::UpdateAll(t, &HitEvent);
    chr_func::CreateTransMatrix(t, t->m_ModelSize, &t->m_Renderer.m_TransMatrix);
    
    t->m_Renderer.Update(1);
}

void TennisState_SlowUpBall::Exit(TennisPlayer* t)
{

}





RATIO TennisState_SlowUpBall::GetChargePower()const
{
    return (float)m_Timer / (float)m_EndFrame;
}

TennisUpBall* TennisState_SlowUpBall::GetSlowUpBall()
{
    return m_pUpBall;
}


void TennisState_SlowUpBall::DeleteSlowBall()
{
    delete m_pUpBall;
    m_pUpBall = nullptr;
}
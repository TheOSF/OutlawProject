#include "AmefootBoundBall.h"
#include "../character/CharacterFunction.h"
#include "../character/Amefoot/AmefootUsualHitEvent.h"
#include "../character/Amefoot/AmefootPlayerState.h"
#include "../Render/MeshRenderer.h"
#include "UsualBall.h"
#include "../Collision/Collision.h"
#include "BallFadeOutRenderer.h"
#include "../Effect/HitEffectObject.h"
#include "../Camera/Camera.h"
#include "../GameSystem/ResourceManager.h"
#include "../Effect/EffectFactory.h"
#include "../Sound/Sound.h"
#include "../character/CharacterManager.h"

// アメフトのバウンドするボールクラス

AmefootBoundBall::AmefootBoundBall(
    CrVector3  pos,
    CrVector3  first_move,
    AmefootPlayer* const pOwner
    ) :
    m_Locus(15),
    m_pRigidBody(nullptr),
    m_pOwner(pOwner)
{
    //ボール基本パラメータ初期化
    m_Params.pos = pos;
    m_Params.move = first_move;
    m_Params.pParent = pOwner;
    m_Params.scale = 0.3f;
    m_Params.type = BallBase::Type::_CantCounter;

    //初期ステートを設定
    SetState(&AmefootBoundBall::StateFly);

    //メッシュ初期化
    {
        iexMesh* pBallMesh;

        UsualBall::GetBallMesh(pOwner->m_PlayerInfo.chr_type, &pBallMesh);

        m_pBallRenderer = new MeshRenderer(
            pBallMesh,
            false,
            MeshRenderer::RenderType::UseColor
            );

        UpdateMesh();
    }

    {
        //軌跡の設定
        m_Locus.m_StartParam.Width = 0.36f;
        m_Locus.m_EndParam.Width = 0.1f;

        UpdateLocusColor();
    }

    {
        //ダメージ判定の更新
        m_Damage.Value = 2.5f;
        m_Damage.type = DamageBase::Type::_WeekDamage;
        m_Damage.pParent = pOwner;
        m_Damage.m_Enable = true;
        m_Damage.pBall = this;
        m_Damage.m_Param.size = 1.0f;
        m_Damage.MaxChrHit = 1;
        m_Damage.HitMotionFrame = 35;

        UpdateDamage();
    }

    m_BallEffect.SetParent(this);
}


AmefootBoundBall::~AmefootBoundBall()
{
    delete m_pBallRenderer;
    m_pBallRenderer = nullptr;

    DefBulletSystem.RemoveRigidBody(m_pRigidBody);
    m_pRigidBody = nullptr;
}


bool AmefootBoundBall::Update()
{
    (this->*m_pStateFunc)();

    UpdateMesh();
    UpdateLocusColor();
    UpdateDamage();
    m_BallEffect.Update();

    return m_pStateFunc != &AmefootBoundBall::StateFinish;
}


bool AmefootBoundBall::Msg(MsgType mt)
{
    return false;
}



void AmefootBoundBall::StateFly()
{
    const float Glavity = -0.02f;

    //重力計算
    m_Params.move.y += Glavity;

    //ステージとの判定
    {
        Vector3 out, pos(m_Params.pos), vec(Vector3Normalize(m_Params.move));
        float dist = m_Params.move.Length()*1.5f;
        int material = 0;

        //ステージとあたっていたら
        if ( DefCollisionMgr.RayPick(
            &out,
            &pos,
            &vec,
            &dist,
            &material,
            CollisionManager::RayType::_Ball
            ) != nullptr
            )
        {
            //あたっていた場所が 高すぎる or 床でない　場合は失敗
            if ( out.y > BallBase::UsualBallShotY*1.5f ||
                Vector3Normalize(vec).y < 0.5f )
            {
                SetState(&AmefootBoundBall::StateNoDamage);

                m_Params.move = Vector3Refrect(m_Params.move, vec) * 0.5f;
            }
            else
            {
                //成功
                SetState(&AmefootBoundBall::StateGroundTouch);
            }

            //高さを床の位置に補正する
            m_Params.pos.y = out.y + m_Params.scale;
            return;
        }
    }


    //寿命管理(ステージ外につけぬけた場合の応急処置
    if ( ++m_Timer > 300 )
    {
        SetState(&AmefootBoundBall::StateFinish);
    }

    //移動更新
    m_Params.pos += m_Params.move;

    //軌跡の点を追加
    AddLocusPoint();

    //エフェクト更新
    m_BallEffect.Update();
}

void AmefootBoundBall::StateGroundTouch()
{
    //軌跡の点を追加
    AddLocusPoint();

    m_Params.move = Vector3Zero;

    //あたり判定を有効化
    m_Damage.m_Enable = true;

    //if ( ++m_Timer > 5 )
    {

        //一番近いプレイヤーに向かっていくように設定
        const auto& chr_map = DefCharacterMgr.GetCharacterMap();
        float dist = FLT_MAX;
        CharacterBase* pTarget = m_pOwner;
        for ( auto&& chr : chr_map )
        {
            float temp = (chr.first->m_Params.pos - m_Params.pos).Length();

            if ( dist < temp )continue;

            dist = temp;
            pTarget = chr.first;
        }

        m_Params.move = pTarget->m_Params.pos - m_Params.pos;
        m_Params.move.y = 0;

        m_Params.move.Normalize();
        m_Params.move *= 0.55f;

        m_Params.type = BallBase::Type::_Usual;

        //エフェクト
        {
            COLORf EffectColor(CharacterBase::GetPlayerColor(m_Params.pParent->m_PlayerInfo.number));

            //エフェクトの設定
            new HitEffectObject(
                m_Params.pos,
                Vector3Normalize(m_Params.move),
                0.1f,
                0.1f,
                Vector3(EffectColor.r, EffectColor.g, EffectColor.b)
                );

            //パーティクル
            EffectFactory::Smoke(
                m_Params.pos,
                Vector3Zero,
                2.0f,
                1.0f,
                true
                );
        }

        {
            //メッシュを光らせる
            COLORf Color = CharacterBase::GetPlayerColorF(m_Params.pParent->m_PlayerInfo.number);

            m_pBallRenderer->m_HDR = Vector3(1, 1, 1) * 0.1f;
            m_pBallRenderer->m_Lighting = Vector3(1, 1, 1) * 0.1f;
        }

        //ＳＥ
        Sound::Play(Sound::Swing2);

        //移動ステートに移行
        SetState(&AmefootBoundBall::StateMove);

    }
}

void AmefootBoundBall::StateMove()
{
    //床についているので、適切な高さ ( BallBase::UsualBallShotY ) まで上昇する
    m_Params.pos.y += (BallBase::UsualBallShotY - m_Params.pos.y) * 0.2f;

    //移動
    m_Params.pos += m_Params.move;

    //ステージとの判定
    {
        Vector3 out, pos(m_Params.pos), vec(Vector3Normalize(m_Params.move));
        float dist = m_Params.move.Length() * 1.5f;
        int material = 0;

        //ステージとあたっていたら
        if ( DefCollisionMgr.RayPick(
            &out,
            &pos,
            &vec,
            &dist,
            &material,
            CollisionManager::RayType::_Ball
            ) != nullptr )
        {
            //成功
            SetState(&AmefootBoundBall::StateNoDamage);

            //高さを床の位置に補正する
            m_Params.pos.y = out.y + m_Params.scale*0.5f;
        }
    }

    //寿命管理(ステージ外につけぬけた場合の応急処置
    if ( ++m_Timer > 300 )
    {
        SetState(&AmefootBoundBall::StateFinish);
    }

    //軌跡の点を追加
    AddLocusPoint();
}

void AmefootBoundBall::StateNoDamage()
{
    //剛体作成
    CreateRigidBody();

    //あたり判定を無効化
    m_Damage.m_Enable = false;

    //ボールタイプをカウンターできないタイプに設定
    m_Params.type = BallBase::Type::_DontWork;

    //RigidBodyクラスの行列を自身に適用する
    {
        Matrix M;
        Vector3 PrePos = m_Params.pos;

        m_pRigidBody->Get_TransMatrix(M);

        M = m_BaseMatrix * M;

        m_pBallRenderer->SetMatrix(M);

        m_Params.pos = Vector3(M._41, M._42, M._43);
        m_Params.move = m_Params.pos - PrePos;

    }

    //軌跡の太さを徐々に減らしていく
    {
        //軌跡
        m_Locus.m_StartParam.Color.w *= 0.95f;

        //太さが一定以下なら描画しない
        if ( m_Locus.m_StartParam.Color.w < 0.1f )
        {
            m_Locus.m_Visible = false;
        }

        if ( m_Locus.m_Visible )
        {
            //軌跡の点を追加
            AddLocusPoint();
        }
    }

    //ライト少く
    {
        m_pBallRenderer->m_HDR *= 0.1f;
        m_pBallRenderer->m_Lighting *= 0.1f;
    }

    //時間経過でフェードアウト
    if ( ++m_Timer > 60 )
    {
        SetState(&AmefootBoundBall::StateCreateFadeOutBall);
    }
}

void AmefootBoundBall::StateFinish()
{

}

void AmefootBoundBall::StateCreateFadeOutBall()
{
    //フェードアウトして消えるボールクラスを作成する
    iexMesh* pMesh;

    //剛体作成
    CreateRigidBody();

    UsualBall::GetBallMesh(m_Params.pParent->m_PlayerInfo.chr_type, &pMesh);

    new BallFadeOutRenderer(
        pMesh,
        m_BaseMatrix,
        m_pRigidBody,
        60
        );

    //自身で開放しないようにnullに設定しておく
    m_pRigidBody = nullptr;


    //ステートを終了に設定
    SetState(&AmefootBoundBall::StateFinish);
}

void AmefootBoundBall::CreateRigidBody()
{
    //すでに作成済みならreturn
    if ( m_pRigidBody != nullptr )
    {
        return;
    }

    m_BaseMatrix = m_pBallRenderer->GetMatrix();

    m_BaseMatrix._41 = 0;
    m_BaseMatrix._42 = 0;
    m_BaseMatrix._43 = 0;

    const UsualBall::PhysicsParam p = UsualBall::GetBallPhysics(m_Params.pParent->m_PlayerInfo.chr_type);


    m_pRigidBody = DefBulletSystem.AddRigidSphere(
        p.Mass,
        RigidBody::ct_dynamic,
        m_Params.pos,
        Vector3Zero,
        p.Radius,
        p.Friction,
        p.Restitution,
        m_Params.move * 45.0f
        );
}

void AmefootBoundBall::SetState(StateFunc pNextState)
{
    m_pStateFunc = pNextState;
    m_Timer = 0;
}

void AmefootBoundBall::Counter(CharacterBase* pCounterCharacter)
{
    m_Damage.pParent = m_Params.pParent = pCounterCharacter;

    UpdateLocusColor();
    m_BallEffect.Counter();

    m_Damage.ResetCounts();
    m_Damage.type = DamageBase::Type::_VanishDamage;
}


void AmefootBoundBall::AddLocusPoint()
{
    Vector3 v;
    Vector3Cross(v, m_Params.move, DefCamera.GetForward());
    v.Normalize();

    m_Locus.AddPoint(m_Params.pos, v);
}

void AmefootBoundBall::UpdateMesh()
{
    //メッシュ更新
    const float scale = UsualBall::GetBallScale(CharacterType::_Americanfootball);
    Matrix m;

    D3DXMatrixScaling(&m, scale, scale, scale);
    m._41 = m_Params.pos.x;
    m._42 = m_Params.pos.y;
    m._43 = m_Params.pos.z;

    m_pBallRenderer->SetMatrix(m);
}

void AmefootBoundBall::UpdateLocusColor()
{
    //軌跡色更新
    const COLORf Color = CharacterBase::GetPlayerColorF(m_Params.pParent->m_PlayerInfo.number);

    m_Locus.m_StartParam.Color = Color.toVector4();
    m_Locus.m_StartParam.Color.w = 0.3f;
    m_Locus.m_StartParam.HDRColor = m_Locus.m_StartParam.Color;

    m_Locus.m_StartParam.HDRColor.w = 0.5f;

    m_Locus.m_EndParam.Color = m_Locus.m_StartParam.Color;
    m_Locus.m_EndParam.Color.w = 0;

    m_Locus.m_EndParam.HDRColor = m_Locus.m_StartParam.HDRColor;
    m_Locus.m_EndParam.HDRColor.w = 0;


}


void AmefootBoundBall::UpdateDamage()     //当たり判定の更新
{
    m_Damage.m_Vec = m_Params.move;
    m_Damage.m_Param.pos = m_Params.pos;
}




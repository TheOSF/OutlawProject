#include "TennisHeartBall.h"
#include "../CharacterFunction.h"
#include "../../Render/MeshRenderer.h"
#include "../../GameSystem/ResourceManager.h"
#include "../CharacterManager.h"
#include "../../Effect/ParticleRenderer.h"
#include "../../Effect/ParticleMoveObject.h"
#include "../../Effect/ImpactLightObject.h"
#include "../../Effect/EffectFactory.h"

TennisHeartBall::TennisHeartBall(
    CrVector3 pos,
    CrVector3 move,
    TennisPlayer* pTennis
    ) :
    m_InitMove(move.Length()),
    m_pTennis(pTennis),
    m_LiveCount(4),
    m_MoveCount(0),
    m_ScaleCount(0.15f),
    m_ScaleMove(0.0025f)
{
    {
        m_PrePos = pos;
        m_Params.pos = pos;
        m_Params.move = move;
        m_Params.pParent = pTennis;
        m_Params.scale = 1.0f;
        m_Params.type = BallBase::Type::_ICantCounter;
    }

    {
        m_Damage.HitMotionFrame = 50;
        m_Damage.m_Enable = true;
        m_Damage.m_Param.pos = pos;
        m_Damage.m_Param.size = 1.2f;
        m_Damage.m_VecPower = Vector2(0.2f, 0);
        m_Damage.m_VecType = DamageShpere::DamageVecType::MemberParam;
        m_Damage.pParent = pTennis;
        m_Damage.pBall = this;

        //m_Damage.Value = 1000;
        //m_Damage.m_Param.size = 100.0f;
    }

    {
        m_pMesh = new MeshRenderer(
            DefResource.Get(Resource::MeshType::Tennis_Heart),
            false,
            MeshRenderer::RenderType::UseColor
            );

        UpdateMesh();
    }
}


TennisHeartBall::~TennisHeartBall()
{
    delete m_pMesh;
}

bool TennisHeartBall::Update()
{
    m_PrePos = m_Params.pos;

    Move();
    UpdateDamage();
    UpdateMesh();

    if (m_Damage.HitCount > 0)
    {
        m_isLive = false;
        Effect(15);

        new ImpactLightObject(
            m_Params.pos,
            30.0f,
            Vector3(1, 0, 1), 
            0.1f
            );
    }


    if (m_LiveCount < 0)
    {
        m_isLive = false;

        Effect(15);

        SmokeEffect();
    }


    m_pTennis->SetDontBoundBallAtkTimer();

    return m_isLive;
}

bool TennisHeartBall::Msg(MsgType mt)
{

    return false;
}

void TennisHeartBall::UpdateDamage()
{
    m_Damage.m_Param.pos = m_Params.pos;
    m_Damage.m_Vec = m_Params.move;
}

bool TennisHeartBall::GetTarget(CharacterBase** ppOut)
{   
    CharacterManager::CharacterMap ChrMap = DefCharacterMgr.GetCharacterMap();

    const float check_distance = 30.0f;
    RADIAN MostMinAngle = D3DXToRadian(90);   //もっとも狭い角度
    RADIAN TempAngle;

    Vector3 toEnemy;
    Vector3 MyFront = Vector3Normalize(m_Params.move);

    auto it = ChrMap.begin();

    //戻り値を初期化
    *ppOut = nullptr;


    while (it != ChrMap.end())
    {
        //自身と死んでいるキャラを除外
        if (m_pTennis->m_PlayerInfo.number == it->first->m_PlayerInfo.number ||
            chr_func::isDie(it->first)
            )
        {
            ++it;
            continue;
        }

        //敵へのベクトル
        toEnemy = it->first->m_Params.pos - m_Params.pos;

        //距離外ならcontinue
        if (toEnemy.Length() > check_distance)
        {
            ++it;
            continue;
        }

        //前ベクトルと敵へのベクトルの角度を計算する
        TempAngle = Vector3Radian(MyFront, toEnemy);

        //角度が一番狭かったら更新
        if (TempAngle < MostMinAngle)
        {
            *ppOut = it->first;
            MostMinAngle = TempAngle;
        }

        ++it;
    }

    return *ppOut != nullptr;
}

void TennisHeartBall::UpdateMesh()
{
    Matrix M;
    const float Scale = CalcScale();

    D3DXMatrixScaling(&M, Scale, Scale, Scale);

    {
        M._41 = m_Params.pos.x;
        M._42 = m_Params.pos.y;
        M._43 = m_Params.pos.z;
    }

    m_pMesh->SetMatrix(M);

}

void TennisHeartBall::MoveRotate(RADIAN angle)
{
    CharacterBase* pTarget = nullptr;

    if (GetTarget(&pTarget) == false)
    {
        return;
    }

    Vector3 v1, v2, axis;

    v1 = m_Params.move;
    v2 = pTarget->m_Params.pos - m_Params.pos;

    v1.y = 0.0f;
    v2.y = 0.0f;

    RADIAN toTargetAngle = Vector3Radian(v1, v2);
    toTargetAngle = min(toTargetAngle, angle);
  
    if (v1.z*v2.x - v1.x*v2.z < 0.0f)
    {
        axis = -Vector3AxisY;
    }
    else
    {
        axis = Vector3AxisY;
    }

    m_Params.move = Vector3RotateAxis(axis, toTargetAngle, m_Params.move);

}

void TennisHeartBall::Move()
{
    const float Speed = 0.15f;

    MoveRotate(D3DXToRadian(5));

    if (rand() % 5 == 0)
    {
        Effect(1);
    }

    m_MoveCount += Speed;

    if (m_MoveCount > PI*2.0f)
    {
        m_MoveCount = 0;
        --m_LiveCount;
    }

    m_Params.pos += m_Params.move*((sinf(m_MoveCount)*0.5f + 0.5f)*0.5f + 0.5f);
}

float TennisHeartBall::CalcScale()
{
    const float Scale = 0.125f;

    m_ScaleMove += (Scale - m_ScaleCount)*0.05f;
    m_ScaleCount += m_ScaleMove;

    return m_ScaleCount;
}

void TennisHeartBall::Effect(int Level)
{
    COLORf color(1, 1, 1, 1);

    for (int i = 0; i < Level; ++i)
    {
        ParticleRenderer*p = new ParticleRenderer();

        p->m_Param.color = color;
        p->m_Param.dw_Flag = RS_COPY;
        p->m_Param.pos = m_PrePos + Vector3Rand();
        p->m_pTexture = DefResource.Get(Resource::TextureType::Heart);
        p->m_Param.size = Vector2(1, 1)*0.45f;

        new ParticleMoveObject2(
            p,
            Vector3Rand()*0.1f,
            Vector3(0,0.02f,0),
            false,
            1,
            1,
            18,
            20,
            0x00FFFFFF,
            0x00FFFFFF
            );
    }
}

void TennisHeartBall::SmokeEffect()
{
    for (int i = 0; i<6; ++i)
    {
        EffectFactory::SmokeParticle(
            m_PrePos + Vector3Rand()*0.3f,
            Vector3(0, 0.05f, 0),
            40,
            1.0f,
            0x80FFFFFF
            );
    }
}

void TennisHeartBall::Counter(CharacterBase* pCounterCharacter)
{
    m_isLive = false;
    m_Damage.pParent = pCounterCharacter;
    m_Damage.m_Enable = false;

    Effect(10);

    SmokeEffect();
}
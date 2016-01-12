#include "Item_Bell.h"
#include "../Damage/Damage.h"

#include "../Effect/EffectFactory.h"
#include "../Sound/Sound.h"
#include "RainbowSkillBall.h"

const Vector3 Item_Bell::BellFreezePos = Vector3(0, 10, 0);

Item_Bell::Item_Bell(UINT HitCount) :
m_Pos(0, 30, 0),
m_pStateFunc(&Item_Bell::State_MoveToTarget),
m_MaxHitCount((int)HitCount),
m_HitCount(0),
m_NoDamageFrame(0),
m_pLastHitChr(nullptr)
{
    //メッシュ読み込み
    {
        m_pMeshRenderer = new MeshRenderer(
            new iexMesh("DATA\\mesh\\Capsure.imo"),
            true,
            MeshRenderer::RenderType::NoTexture
        );
        
        MeshUpdate();
    }

    m_CameraDrawObject.m_isDraw = false;
}

Item_Bell::~Item_Bell()
{
    delete m_pMeshRenderer;
}

void Item_Bell::Hit(Item_WallBoundBall* pHitBall, CharacterBase* pHitOwner)
{
    ++m_HitCount;

    pHitBall->Destroy();

    //リストから削除
    for (auto& it = m_WallBoundBallArray.begin();
        it != m_WallBoundBallArray.end();
        ++it)
    {
        if (*it == pHitBall)
        {
            m_WallBoundBallArray.erase(it);
            break;
        }
    }

    //当たったキャラクタを保存
    m_pLastHitChr = pHitOwner;

    //最大回数なったら
    if (m_HitCount >= m_MaxHitCount)
    {
        m_pStateFunc = &Item_Bell::State_ApperSkillSoul;

        //ＳＥ
        Sound::Play(Sound::Gong_End);
    }
    else
    {
        //ＳＥ
        Sound::Play(Sound::Gong_Start);
    }

    //エフェクト
    EffectFactory::CircleAnimationBillbord(
        m_Pos,
        Vector3Zero,
        Vector3Zero,
        Vector2(20, 20),
        0xFFFFFFFF,
        RS_ADD
        );

}

Vector3 Item_Bell::GetPos()
{
    return m_Pos;
}


bool Item_Bell::Update()
{
    (this->*m_pStateFunc)();

    m_CameraDrawObject.m_Pos = m_Pos;

    MeshUpdate();

    UpdateBallLive();

    return m_pStateFunc != &Item_Bell::State_Finish;
}

bool Item_Bell::Msg(MsgType mt)
{


    return false;
}

void Item_Bell::UpdateBallLive()
{
    const int BallNumTarget = m_MaxHitCount - m_HitCount;  //出ていてほしい数

    auto it = m_WallBoundBallArray.begin();

    while (it != m_WallBoundBallArray.end())
    {
        if ((*it)->m_Params.pos.Length() > 200.0f)
        {
            (*it)->Destroy();
            it = m_WallBoundBallArray.erase(it);
            continue;
        }

        ++it;
    }

    while (BallNumTarget > (int)m_WallBoundBallArray.size())
    {
        m_WallBoundBallArray.push_back(
            new Item_WallBoundBall(
                Vector3(0, 20, 10),
                Vector3Normalize(Vector3(frand(), 0, frand()))*0.65f,
                Item_Bell::BellFreezePos,
                this
                )
            );
    }

}

void Item_Bell::MeshUpdate()
{
    const float Scale = 3.0f;
    Matrix m;
    
    D3DXMatrixScaling(&m, Scale, Scale, Scale);

    {
        m._41 = m_Pos.x;
        m._42 = m_Pos.y;
        m._43 = m_Pos.z;
    }
    
    m_pMeshRenderer->SetMatrix(m);
}



//----------------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------------//

void Item_Bell::State_MoveToTarget()
{

    const float Speed = 0.1f;

    Vector3 v = BellFreezePos - m_Pos;

    if (v.Length() > Speed)
    {
        v.Normalize();
        v *= Speed;
    }

    m_Pos += v;

    if (Vector3Distance(m_Pos, BellFreezePos) < 0.01f)
    {
        m_pStateFunc = &Item_Bell::State_WaitHit;
    }
}

void Item_Bell::State_WaitHit()
{
    Vector3 HitPos(0, 0, 0);

    m_NoDamageFrame = max(0, m_NoDamageFrame - 1);

    m_CameraDrawObject.m_isDraw = true;

}

void Item_Bell::State_ApperSkillSoul()
{
    new RainbowSkillBall(
        m_Pos,
        Vector3AxisY * 0.2f,
        m_pLastHitChr
        );

    m_pStateFunc = &Item_Bell::State_FadeOut;
}

void Item_Bell::State_FadeOut()
{
    const Vector3 TargetPos = BellFreezePos + Vector3(0, 20, 0);
    const float Speed = 0.2f;

    Vector3 v = TargetPos - m_Pos;

    m_CameraDrawObject.m_isDraw = false;

    if (v.Length() > Speed)
    {
        v.Normalize();
        v *= Speed;
    }

    m_Pos += v;

    if (Vector3Distance(m_Pos, TargetPos) < 0.01f)
    {
        m_pStateFunc = &Item_Bell::State_Finish;
    }
}

void Item_Bell::State_Finish()
{

}

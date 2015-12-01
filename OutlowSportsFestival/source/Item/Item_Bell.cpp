#include "Item_Bell.h"
#include "../Damage/Damage.h"

#include "../Effect/EffectFactory.h"
#include "../Sound/Sound.h"

const Vector3 Item_Bell::BellFreezePos = Vector3(0, 10, 0);

Item_Bell::Item_Bell(UINT HitCount) :
m_Pos(0, 30, 0),
m_pStateFunc(&Item_Bell::State_MoveToTarget),
m_MaxHitCount((int)HitCount),
m_HitCount(0),
m_NoDamageFrame(0)
{
    //ƒƒbƒVƒ…“Ç‚Ýž‚Ý
    {
        m_pMeshRenderer = new MeshRenderer(
            new iexMesh("DATA\\mesh\\Capsure.imo"),
            true,
            MeshRenderer::RenderType::NoTexture
        );
        
        MeshUpdate();
    }

}

Item_Bell::~Item_Bell()
{
    delete m_pMeshRenderer;
}

bool Item_Bell::Update()
{
    (this->*m_pStateFunc)();

    MeshUpdate();

    return m_pStateFunc != &Item_Bell::State_Finish;
}

bool Item_Bell::Msg(MsgType mt)
{


    return false;
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


bool Item_Bell::HitCheck(Vector3& out_hitPos)
{
    class HitCheckClass :public DamageManager::HitEventBase
    {
    public:
        bool    isHit;
        Vector3 HitPos;
        const Vector3 Pos;

        HitCheckClass(CrVector3 pos) :Pos(pos), isHit(false){}

        bool Hit(DamageBase* pDmg)override
        {
            Vector3 vec;
            isHit = true;
            pDmg->CalcPosVec(Pos, &HitPos, &vec);

            return true;
        }
    };

    SphereParam sp;
    HitCheckClass hit_check(m_Pos);

    sp.pos = m_Pos;
    sp.size = 4.0f;

    DefDamageMgr.HitCheckSphere(sp, hit_check);

    if (hit_check.isHit)
    {
        out_hitPos = hit_check.HitPos;
    }

    return hit_check.isHit;
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

    if (HitCheck(HitPos) && m_NoDamageFrame <= 0)
    {
        //‚P•bŠÔ–³“G‚É
        m_NoDamageFrame = 60;

        ++m_HitCount;

        //Å‘å‰ñ”‚È‚Á‚½‚ç
        if (m_HitCount >= m_MaxHitCount)
        {
            m_pStateFunc = &Item_Bell::State_ApperSkillSoul;

            //‚r‚d
            Sound::Play(Sound::Gong_End);
        }
        else
        {
            //‚r‚d
            Sound::Play(Sound::Gong_Start);
        }

        //ƒGƒtƒFƒNƒg
        EffectFactory::CircleAnimationBillbord(
            m_Pos,
            Vector3Zero,
            Vector3Zero,
            Vector2(20,20),
            0xFFFFFFFF,
            RS_ADD
            );

        
    }
}

void Item_Bell::State_ApperSkillSoul()
{
    m_pStateFunc = &Item_Bell::State_FadeOut;
}

void Item_Bell::State_FadeOut()
{
    const Vector3 TargetPos = BellFreezePos + Vector3(0, 20, 0);
    const float Speed = 0.2f;

    Vector3 v = TargetPos - m_Pos;

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

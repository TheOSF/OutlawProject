#include "StageDamagePhysicMoveObject.h"
#include "../Ball/BallFadeOutRenderer.h"
#include "../Damage/Damage.h"

#include "StagePhysicMoveObject.h"

StageDamagePhysicMoveObject::StageDamagePhysicMoveObject(
    MeshRenderer* pRenderMesh,       //•`‰æ‚·‚éƒƒbƒVƒ…
    CrVector3     pos,
    CrVector3     scale,
    CrVector3     angle,
    float         HitScale,           //‚ ‚½‚èˆÀ’è‚Ì‘å‚«‚³
    RigidBodyCreater* pCreater,
    CrVector3     diff_pos
    ) :
    m_pRenderMesh(pRenderMesh),
    m_HitScale(HitScale),
    m_Pos(pos),
    m_Scale(scale),
    m_Angle(angle),
    m_pCreater(pCreater),
    m_Diffpos(diff_pos)
{
    {
        Matrix m;
        Vector3 p = m_Pos + m_Diffpos;

        //	À•WŒnÝ’è 
        SetTransformMatrixXYZ(&m, p.x, p.y, p.z, m_Angle.x, m_Angle.y, m_Angle.z);
        
        m._11 *= m_Scale.x;
        m._12 *= m_Scale.x;
        m._13 *= m_Scale.x;

        m._21 *= m_Scale.y;
        m._22 *= m_Scale.y;
        m._23 *= m_Scale.y;

        m._31 *= m_Scale.z;
        m._32 *= m_Scale.z;
        m._33 *= m_Scale.z;

        m_pRenderMesh->SetMatrix(m);
    }
}

StageDamagePhysicMoveObject::~StageDamagePhysicMoveObject()
{
    delete m_pRenderMesh;
    delete m_pCreater;
}

bool StageDamagePhysicMoveObject::Update()
{
    class MyHitEvent :public DamageManager::HitEventBase
    {
    public:
        MyHitEvent() :hit(false){}

        Vector3 vec;
        bool hit;

        bool Hit(DamageBase* pDmg)
        {
            vec = pDmg->vec;
            hit = true;
            return true;
        }
    };

    SphereParam  param;
    MyHitEvent   hit_event;

    param.pos = m_Pos;


    param.size = m_HitScale;

    DefDamageMgr.HitCheckSphere(
        param,
        hit_event
        );
    
    if (hit_event.hit)
    {
        Matrix m;
        {
            Vector3 temp;

            temp = m_Pos;
            m_pRenderMesh->m_pMesh->SetPos(temp);

            temp = m_Angle;
            m_pRenderMesh->m_pMesh->SetAngle(temp);

            temp = m_Scale;
            m_pRenderMesh->m_pMesh->SetScale(temp);

        }

        D3DXMatrixScaling(&m, m_Scale.x, m_Scale.y, m_Scale.z);

        m._41 = m_Diffpos.x;
        m._42 = m_Diffpos.y;
        m._43 = m_Diffpos.z;


        new StagePhysicMoveObject(
            m_pRenderMesh,
            m_pCreater->Create(hit_event.vec),
            m,
            300,
            360
            );

        m_pRenderMesh = nullptr;
    }

    return !hit_event.hit;
}

bool StageDamagePhysicMoveObject::Msg(GameObjectBase::MsgType type)
{
    return false;
}
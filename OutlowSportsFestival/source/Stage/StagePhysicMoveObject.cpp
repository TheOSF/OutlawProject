#include "StagePhysicMoveObject.h"
#include "../Ball/BallFadeOutRenderer.h"
#include "../Damage/Damage.h"

#include "../Effect/HitEffectObject.h"

StagePhysicMoveObject::StagePhysicMoveObject(
    MeshRenderer*       pRenderMesh,
    RigidBody*          pRigidBody,
    CrVector3           Scale,
    float               HitScale
    ) :
    m_pRenderMesh(pRenderMesh),
    m_pRigidBody(pRigidBody),
    m_HitScale(HitScale),
    m_Scale(Scale),
    m_HitCount(0),
    m_Destroy(false)
{
    MeshUpdate();
}

StagePhysicMoveObject::~StagePhysicMoveObject()
{
    delete m_pRenderMesh;
    DefBulletSystem.RemoveRigidBody(m_pRigidBody);
}

void StagePhysicMoveObject::MeshUpdate()
{
    Matrix M;
    m_pRigidBody->Get_TransMatrix(M);

    {
        M._11 *= m_Scale.x;
        M._12 *= m_Scale.x;
        M._13 *= m_Scale.x;

        M._21 *= m_Scale.y;
        M._22 *= m_Scale.y;
        M._23 *= m_Scale.y;

        M._31 *= m_Scale.z;
        M._32 *= m_Scale.z;
        M._33 *= m_Scale.z;

        m_pRenderMesh->SetMatrix(M);

    }

}

bool StagePhysicMoveObject::Update()
{
    class HitCheckObj :public DamageManager::HitEventBase
    {
    public:
        const Vector3 pos;
        Vector3 vec;
        
        bool isHit;

        HitCheckObj(CrVector3 pos) :
            isHit(false),
            pos(pos){}

        bool Hit(DamageBase* pDmg)
        {
            Vector3 p;

            pDmg->CalcPosVec(pos, &p, &vec);

            isHit = true;

            return false;
        }
    };

    m_HitCount = max(m_HitCount - 1, 0);

    Matrix M;
    Vector3 pre_pos;

    m_pRigidBody->Get_TransMatrix(M);

    MeshUpdate();

    SphereParam sp;

    sp.pos = Vector3(M._41, M._42, M._43);
    sp.size = m_HitScale;

    HitCheckObj HitEvent(sp.pos);

    DefDamageMgr.HitCheckSphere(sp, HitEvent);


    if (HitEvent.isHit && m_HitCount == 0)
    {
        HitEvent.vec.Normalize();

        //エフェクト
        new HitEffectObject(
            sp.pos,
            HitEvent.vec,
            0.025f,
            0.075f,
            Vector3(1, 1, 1)
            );

        HitEvent.vec.y = 0.2f;
        HitEvent.vec *= 35.0f;

        btVector3 p(HitEvent.vec.x, HitEvent.vec.y, HitEvent.vec.z);
       
        m_pRigidBody->pRigidBody->activate(false);

        m_pRigidBody->pRigidBody->setLinearVelocity(
            p
            );

        m_pRigidBody->pRigidBody->setAngularVelocity(
            btVector3(frand(), frand(), frand())*(frand() - 0.5f)*0.05f
            );

        
        m_HitCount = 60;
    }

    if (m_Destroy == false)
    {
        m_Destroy = Vector3(M._41, M._42, M._43).Length() > 150.0f;
    }

    return !m_Destroy;
}

bool StagePhysicMoveObject::Msg(GameObjectBase::MsgType type)
{
    if (type == GameObjectBase::MsgType::_RoundReset)
    {
        m_Destroy = true;
    }

    return false;
}
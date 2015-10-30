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
    m_HitCount(0)
{

}

StagePhysicMoveObject::~StagePhysicMoveObject()
{
    delete m_pRenderMesh;
    DefBulletSystem.RemoveRigidBody(m_pRigidBody);
}

bool StagePhysicMoveObject::Update()
{
    class HitCheckObj :public DamageManager::HitEventBase
    {
    public:
        Vector3 vec;
        bool isHit;

        HitCheckObj() :
            isHit(false){}

        bool Hit(DamageBase* pDmg)
        {
            vec = pDmg->vec;
            isHit = true;

            return false;
        }
    };

    m_HitCount = max(m_HitCount - 1, 0);

    Matrix M;
    Vector3 pre_pos;

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

    SphereParam sp;
    HitCheckObj HitEvent;

    sp.pos = Vector3(M._41, M._42, M._43);
    sp.size = m_HitScale;

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


    return Vector3(M._41, M._42, M._43).Length() < 200.0f;
}

bool StagePhysicMoveObject::Msg(GameObjectBase::MsgType type)
{
    return false;
}
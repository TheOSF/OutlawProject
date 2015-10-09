#include "CharacterPhysic.h"
#include "CharacterBase.h"

CharacterPhysic::CharacterPhysic(CharacterBase*  pOwnerCharacter):
m_pOwnerCharacter(pOwnerCharacter)
{
    m_pRigidBody = DefBulletSystem.AddRigidCapsure(
        50,
        RigidBody::CollisionTypes::ct_kinematic,
        pOwnerCharacter->m_Params.pos + Vector3(0, 2, 0),
        Vector3Zero,
        1.0f,
        4,
        0.5f,
        0.5f,
        Vector3Zero
        );

}


CharacterPhysic::~CharacterPhysic()
{
    DefBulletSystem.RemoveRigidBody(m_pRigidBody);
}


void CharacterPhysic::Update()
{
    m_pRigidBody->TransformKinematicObject(
        m_pOwnerCharacter->m_Params.pos + Vector3(0, 2, 0),
        Vector3Zero
        );
}

#include "PhysicsMeshObject.h"

//----------------------------------------------------------------------------
//�@�@�@�@ �����v�Z�Ń��b�V�������_���[�𓮂����I�u�W�F�N�g�N���X
//----------------------------------------------------------------------------


PhysicsMeshObject::PhysicsMeshObject(
    MeshRenderer* pRenderer,
    RigidBody*    pRigitBody,
    UINT          DeleteTime
    ):
    m_pRenderer(pRenderer),
    m_pRigitBody(pRigitBody),
    m_DeleteTime(DeleteTime)
{
    assert(DeleteTime > 0 && "DeleteTime�̒l�͂O�ȏ�ɂ��Ă�������");

    m_BaseMatrix = pRenderer->GetMatrix();
    
    m_BaseMatrix._41 = 0;
    m_BaseMatrix._42 = 0;
    m_BaseMatrix._43 = 0;

}

PhysicsMeshObject::~PhysicsMeshObject()
{
    delete m_pRenderer;
    DefBulletSystem.RemoveRigidBody(m_pRigitBody);
}

bool PhysicsMeshObject::Update()
{
    Matrix M;

    m_pRigitBody->Get_TransMatrix(M);

    M = m_BaseMatrix*M;

    m_pRenderer->SetMatrix(M);

    return --m_DeleteTime > 0;
}


bool PhysicsMeshObject::Msg(MsgType mt)
{
    return false;
}
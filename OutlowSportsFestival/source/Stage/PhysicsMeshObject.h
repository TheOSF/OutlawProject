#pragma once

#include "iextreme.h"
#include "../Render/MeshRenderer.h"
#include "../Library/Bullet/BulletSystem.h"
#include "../GameSystem/GameObject.h"

//----------------------------------------------------------------------------
//�@�@�@�@ �����v�Z�Ń��b�V�������_���[�𓮂����I�u�W�F�N�g�N���X
//----------------------------------------------------------------------------

class PhysicsMeshObject :public GameObjectBase
{
public:
    PhysicsMeshObject(
        MeshRenderer* pRenderer,
        RigidBody*    pRigitBody,
        UINT          DeleteTime
        );

    ~PhysicsMeshObject();

private:

    MeshRenderer* const m_pRenderer;
    RigidBody*    const m_pRigitBody;
    UINT                m_DeleteTime;
    Matrix              m_BaseMatrix;
    
    bool Update();
    bool Msg(MsgType mt);
};
#pragma once

#include "../GameSystem/GameObject.h"
#include "../Library/Bullet/BulletSystem.h"
#include "../Render/MeshRenderer.h"
#include "../Collision/MeshCollider.h"

//-------------------------------------------------------------------------------//
// �_���[�W���󂯂�ƕ����V���~���[�V�����œ������X�e�[�W�I�u�W�F�N�g�N���X
//-------------------------------------------------------------------------------//

class StageDamagePhysicMoveObject :public GameObjectBase
{
public:

    class RigidBodyCreater
    {
    public:
        virtual ~RigidBodyCreater(){}
        virtual RigidBody* Create(CrVector3 v) = 0;
    };

    StageDamagePhysicMoveObject(
        MeshRenderer* pRenderMesh,       //�`�悷�郁�b�V��
        CrVector3     pos,
        CrVector3     scale,
        CrVector3     angle,
        float         HitScale,           //���������̑傫��
        RigidBodyCreater* pCreater,
        MeshCollider*     pCollider,      //�����蔻�胁�b�V��(�_���[�W���������delete����)
        CrVector3     diff_pos = Vector3Zero
        );

    ~StageDamagePhysicMoveObject();

private:
    RigidBodyCreater*   m_pCreater;
    MeshRenderer*       m_pRenderMesh;
    const float         m_HitScale;
    const Vector3       m_Pos, m_Scale, m_Angle, m_Diffpos;
    MeshCollider* const m_pCollider;
    bool Update();
    bool Msg(GameObjectBase::MsgType type);
};
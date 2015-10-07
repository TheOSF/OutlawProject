#pragma once

#include "../GameSystem/GameObject.h"
#include "../Library/Bullet/BulletSystem.h"
#include "../Render/MeshRenderer.h"

//----------------------------------------------------------//
// �����V���~���[�V�����œ������X�e�[�W�I�u�W�F�N�g�N���X
//----------------------------------------------------------//

class StagePhysicMoveObject :public GameObjectBase
{
public:

    StagePhysicMoveObject(
        MeshRenderer* pRenderMesh,       //�`�悷�郁�b�V��
        RigidBody*    pRigidBody,        //�`��
        const Matrix& InitTransMatrix,   //�����p��
        UINT          FadeOutStartFrame, //�t�F�[�h�A�E�g���n�߂�t���[��
        UINT          FadeOutEndFrame    //�t�F�[�h�A�E�g���ď�����t���[��
        );

    ~StagePhysicMoveObject();


private:

    bool Update();
    bool Msg(GameObjectBase::MsgType type);
};
#ifndef __HIT_STAGE_OBJECT_H__
#define __HIT_STAGE_OBJECT_H__

#include "../GameSystem/GameObject.h"
#include "../Collision/MeshCollider.h"
#include "..//Render/MeshRenderer.h"


//---------------------------------------------------
//  �����蔻��̂���X�e�[�W�I�u�W�F�N�g�N���X
//---------------------------------------------------

class HitStageObject :public GameObjectBase
{
public:

    HitStageObject(
        MeshRenderer*    pMeshRenderer,  //���b�V���`��N���X
        MeshCollider*   pMeshCollider   //���b�V�������蔻��N���X
        );

    ~HitStageObject();

    void SetMatrix(const Matrix& mat);
private:

    MeshRenderer*  const m_pMeshRenderer;  //���b�V���`��N���X
    MeshCollider* const m_pMeshCollider;   //���b�V�������蔻��N���X

    bool Update();
    bool Msg(MsgType mt);
};

#endif
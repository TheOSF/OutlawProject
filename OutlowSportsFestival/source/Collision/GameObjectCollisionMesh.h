#ifndef ___GAMEOBJECT_COLLISION_MESH_H__
#define ___GAMEOBJECT_COLLISION_MESH_H__

#include "../GameSystem/GameObject.h"
#include "MeshCollider.h"

//--------------------------------------------------------------
//        ���b�V������N���X���܂����Q�[���I�u�W�F�N�g
//--------------------------------------------------------------

class GameObjectCollisionMesh :public GameObjectBase
{
public:
    typedef MeshCollider::HitEvent HitEvent;

    GameObjectCollisionMesh(
        LPIEXMESH		pMesh,		//������Ƃ郁�b�V���i�f�X�g���N�^��delete���Ȃ�
        HitEvent*		pHitEvent	//�Փˎ��̃C�x���g�N���X�i�f�X�g���N�^��delete����
        );

    ~GameObjectCollisionMesh();

    //���[���h�ϊ��s����Z�b�g
    void SetWorldMatrix(const Matrix& WorldMat);

private:
    MeshCollider m_MeshCollider;

    bool Update();
    bool Msg(MsgType mt);

};

#endif
#ifndef __MESH_COLLIDER_H__
#define __MESH_COLLIDER_H__

#include "Collision.h"


//************************************************************
//	���b�V���̂����蔻��N���X
//************************************************************
class MeshCollider :public CollisionBase
{
public:
	class HitEvent
	{
	public:
        virtual void Hit(
            RayType     hit_Raytype,
            CrVector3	hit_pos,
            int			hit_material){}
	};

    MeshCollider(
        LPIEXMESH		pMesh,		//�f�X�g���N�^��delete���Ȃ�
        HitEvent*		pHitEvent,	//�f�X�g���N�^��delete����
        bool            MeshDelete = false, 
        DWORD           hitFlags = 0xFFFFFFFF
		);

	~MeshCollider();

	bool RayPick(
		LPVECTOR3	pOut,		//���C�����������ꍇ���̏ꏊ���i�[����
		LPVECTOR3	pPos,		//���C��ł��n�߂�ꏊ
		LPVECTOR3	pVec,		//���C��ł���
		float*		pDist,		//���C�̒���
		int*		pMaterial,	//���C�����������ꍇ���̃|���S���̍ގ��ԍ����i�[����
		RayType		type		//���C�̃^�C�v(�����𕪂���\�������邽��)
		);

    //�����������̏����֐����Ăяo��
	void Hit(
        RayType     hit_Raytype,
		CrVector3	hit_pos,
		int			hit_material
		);

    //���[���h�ϊ��s����Z�b�g����
    void SetWorldMatrix(const Matrix& mat);
    
private:
	LPIEXMESH		m_pMesh;
	HitEvent*		m_pHitEvent;
    Matrix          m_TransMatrix;
    Matrix          m_TransInvMatrix;
    bool            m_ChangeMatrix;
    bool            m_MeshDelete;
    DWORD           m_HitFlags;
};

#endif
#pragma once


#include "GameSystem/GameObject.h"
#include "GameSystem/ForwardDecl.h"
#include "GameSystem/GameSystem.h"
#include "Damage/Damage.h"
#include "utillity/LocusHDR.h"
#include "Render/LightObject.h"
#include "Library/Bullet/BulletSystem.h"

#include "BaseballPlayer.h"

//*****************************************************
//		�싅�����N���X
//*****************************************************

#define DeleteTimeSet 10;//�@���ł܂ł̎���

class BaseballPlayer;



class BaseballEquip :public GameObjectBase
{
public:
	//�����p�����[�^
	struct PhysicsParam
	{
		float Mass;
		float Friction;
		float Radius;
		float Restitution;
	};

	struct Param
	{
		Matrix  BoneMat;
		Vector3 Forward;
		Vector3 TempPos;
		Vector3 Pos;
		Vector3 Move;
	};


private:
	BaseballPlayer* m_Baseball;
	LPIEXMESH		pHeadMesh;//�@��
	LPIEXMESH	    pArmMesh; //�@�r
	Param param;
	PhysicsParam pParam;
	bool tempflg;
	bool Deleteflg;//�@���Ŏ��Ԍ����p
	bool(BaseballEquip::*       m_pStateFunc)();

	iexMesh* pMesh;

	LpMeshRenderer		        m_pMeshRenderer;
	
	Vector3                     m_RotateSpeed;
	const float         m_Glavity;
	LocusHDR                    m_Locus;

	Matrix					    m;
	Matrix              m_BaseMatrix;
	RigidBody*          m_pRigitBody;
	int                 m_DeleteFrame;

//�@��{
public:
	enum MeshType
	{
		Cap,
		Helmet,
		Grove,
		Bat
	};
	MeshType meshType;
	//�@�R���X�g���N�^
	BaseballEquip(BaseballPlayer* b, int BoneNum, MeshType mt);
	//�@�f�X�g���N�^
	~BaseballEquip();
	//�@����
	void Install(MeshType mt);
	//�@����
	void Takeoff();
	void NormalMove();
	//�@�o�E���h&�t�F�[�h�A�E�g
	void Bound();
	// ����ɗ�����
	void ToPhysicMove();
	bool Update();
	bool Msg(MsgType mt);

public:
	//�@�ʏ�ړ�(��)
	void NormalMove_Head();
	//�@�ʏ�ړ�(�r)
	void NormalMove_Arm();
	//�@�����p�����[�^�Z�b�g
	void SetPhysicsParam();

//�@�X�e�[�g(�����X�V���Ă����)
public:
	bool StateEquip();
	bool StateChangeEquip();
	bool StateHitGround();
	bool StateEnd();

public:
	//�@�F�����݂̐e�L�����N�^�̐F�ɐݒ�
	void UpdateColor();
	//�@��������
	void UpdateMove();
	//�@��������
	void UpdatePhysicMove();
	//�@�������蔻��
	bool UpdateWallCheck();
	
};
//#pragma once
//
//
//#include "GameSystem/GameObject.h"
//#include "GameSystem/ForwardDecl.h"
//#include "GameSystem/GameSystem.h"
//#include "Damage/Damage.h"
//#include "utillity/LocusHDR.h"
//#include "Render/LightObject.h"
//#include "Library/Bullet/BulletSystem.h"
//
//#include "BaseballPlayer.h"
//
////*****************************************************
////		�싅�����N���X
////*****************************************************
//
//#define DeleteTimeSet 10;//�@���ł܂ł̎���
//
//class BaseballPlayer;
//
//
//
//class BaseballEquip :public GameObjectBase
//{
//public:
//	//�����p�����[�^
//	struct PhysicsParam
//	{
//		float Mass;
//		float Friction;
//		float Radius;
//		float Restitution;
//	};
//
//	struct Param
//	{
//		Matrix  BoneMat;
//		Vector3 Forward;
//		Vector3 TempPos;
//		Vector3 Pos;
//		Vector3 Move;
//	};
//
//
//private:
//	BaseballPlayer* m_Baseball;
//	LPIEXMESH		pHedMesh;
//	//LPIEXMESH	    pHelmetMesh;
//	Param param;
//	PhysicsParam pParam;
//	bool tempflg;
//	bool Deleteflg;//�@���Ŏ��Ԍ����p
//	bool(BaseballEquip::*       m_pStateFunc)();
//
//	iexMesh* pMesh;
//
//	LpMeshRenderer		        m_pHedMeshRenderer;
//	LpMeshRenderer		        m_pHatMeshRenderer;
//	Vector3                     m_RotateSpeed;
//	const float         m_Glavity;
//	LocusHDR                    m_Locus;
//
//	Matrix					    m;
//	Matrix              m_BaseMatrix;
//	RigidBody*          m_pRigitBody;
//	int                 m_DeleteFrame;
//
////�@��{
//public:
//	enum MeshType
//	{
//		Cap,
//		Helmet,
//		Grove,
//		Bat
//	};
//	MeshType meshType;
//	//�@�R���X�g���N�^
//	BaseballEquip(BaseballPlayer* b, int BoneNum, MeshType mt);
//	//�@�f�X�g���N�^
//	~BaseballEquip();
//	//�@����
//	void Install(MeshType mt);
//	//�@����
//	void Takeoff();
//	//�@�ʏ�ړ�
//	void NormalMove_Hed();
//	//�@�o�E���h&�t�F�[�h�A�E�g
//	void Bound();
//	// ����ɗ�����
//	void ToPhysicMove();
//
//	bool Update();
//	bool Msg(MsgType mt);
//
////�@��{���Ŏg��
//public:
//
//
//	void SetPhysicsParam();
////�@�X�e�[�g(�����X�V���Ă����)
//public:
//	bool StateEquip();
//	bool StateChangeEquip();
//	bool StateEnd();
//
//
//public:
//
//	void UpdateColor();//�F�����݂̐e�L�����N�^�̐F�ɐݒ�
//	void UpdateMove();
//	bool UpdateWallCheck();
//	
//public:
//	Matrix RendererGetMatrix();
//	void RendererSetMatrix();
//	void RendererHdr();
//	void RenderDelete();
//
//};
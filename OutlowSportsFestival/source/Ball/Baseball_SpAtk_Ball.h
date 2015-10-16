#ifndef __BASEBALL_SPATK_BALL_H__
#define __BASEBALL_SPATK_BALL_H__

#include "Ball.h"
#include "../GameSystem/GameObject.h"
#include "../GameSystem/ForwardDecl.h"
#include "../GameSystem/GameSystem.h"
#include "../Damage/Damage.h"
#include "../Render/LightObject.h"
#include "../Library/Bullet/BulletSystem.h"

//*****************************************************
//		�ʏ�ʃN���X
//*****************************************************

class Baseball_SpAtk_Ball :public GameObjectBase, public BallBase
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


	//�R���X�g���N�^
	Baseball_SpAtk_Ball(
		BallBase::Params	params,			//�{�[���p�����[�^
		DamageBase::Type	damage_type,	//�_���[�W����̃^�C�v
		float				damage_val,		//�_���[�W��
		UINT                hit_num = 1   //�q�b�g��
		);
	~Baseball_SpAtk_Ball();

	//�{�[���̃��b�V����Ԃ�
	static bool GetBallMesh(
		CharacterType::Value	type,	//�{�[���̃L�����N�^�^�C�v
		LPIEXMESH*				ppOut	//�߂�l
		);

	//�{�[���̃��b�V���X�P�[����Ԃ�
	static float GetBallScale(
		CharacterType::Value	type    //�{�[���̃L�����N�^�^�C�v
		);

	//�{�[���̕����p�����[�^��Ԃ�
	static PhysicsParam GetBallPhysics(
		CharacterType::Value	type	//�{�[���̃L�����N�^�^�C�v
		);

	bool Update();
	bool Msg(MsgType mt);

private:

	bool(Baseball_SpAtk_Ball::*m_pStateFunc)();
	LpMeshRenderer		m_pMeshRenderer;
	DamageCapsure		m_Damage;
	int                 m_DeleteFrame;
	Matrix              m_BaseMatrix;
	RigidBody*          m_pRigitBody;
	const UINT          m_HitNum;
	UINT                m_HitCountSave;
	UINT                m_HitStopFrame;


	bool isOutofField()const;  //�t�B�[���h�O�ɏo�Ă��邩

	void UpdateDamageClass();  //�_���[�W����̈ʒu�����݂̈ʒu�ɍX�V
	
	bool UpdateWallCheck(Vector3& outNewMove);    //�ǂƂ̔�������A�ڐG���Ă����Ȃ�ړ��l�𔽎˂��ăX�e�[�g�ڍs������
	
	void ToNoWork();           //�U������̂Ȃ���Ԃɂ���

	bool StateFlyMove();
	bool StatePhysicMove();
};

#endif
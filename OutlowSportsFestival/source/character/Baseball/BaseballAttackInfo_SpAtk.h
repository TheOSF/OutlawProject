#pragma once

#include "BaseballSpAttackClass.h"



//****************************************************************
//		�싅�̍U�����N���X
//****************************************************************

class BaseballAttackInfo_SpAtk :public BaseballSpAttackClass::AttackInfo
{
public:
	struct Param
	{
		float              DamageValue;               //�_���[�W�l
		float              DamageSize;                //�����蔻��傫��
		float              DamagePosLength;           //�{�[�����瓖���蔻��܂ł̋���
		DamageBase::Type   DamageType;                //�_���[�W�^�C�v

		int                DamageEnableStart;         //�_���[�W�L���X�^�[�g�t���[��
		int                DamageEnableEnd;           //�_���[�W�L���G���h�t���[��

		float              MoveSpeed;
		int                MoveStartFrame;
		int                MoveEndFrame;

		int                Motion;

		int                AllFrame;



		int                AngleControllStartFrame;
		int                AngleControllEndFrame;
		RADIAN             AngleControllValue;

		int                LocusBoneNum;
	};

	Param m_Param;

	BaseballAttackInfo_SpAtk(BaseballPlayer* pOwner);
	~BaseballAttackInfo_SpAtk();

private:

	BaseballPlayer * const  m_pOwner;
	Vector3               m_LocusPos, m_LocusVec;

	//�_���[�W�p�����[�^���Z�b�g
	void DamageParamSet(DamageShpere* pDmg);

	//�_���[�W�ʒu���Z�b�g
	void DamagePosSet(DamageShpere* pDmg, BaseballPlayer* pTennis);

	//���[�V�������Z�b�g
	void MotionSet(BaseballPlayer* pTennis);

	//�����̃t���[�����U���I�����ǂ���
	bool isEnd(int Frame);

	
	//�����̃t���[�����A�p�x�������s����t���[�����ǂ���
	bool isCanAngleControll(int Frame, RADIAN& OutControllVal);

	//�����̃t���[�����A�_���[�W����L���t���[�����ǂ���
	bool isDamageEnable(int Frame);

	//���t���[���Ă΂��
	void Update(int Frame, LocusHDR* pLocus);
};
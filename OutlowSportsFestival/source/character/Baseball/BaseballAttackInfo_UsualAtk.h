#pragma once

#include "BaseballAttackClass.h"



//****************************************************************
//		�싅�̍U�����N���X
//****************************************************************

class BaseballAttackInfo_UsualAtk :public BaseballAttackClass::AttackInfo
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

		int                CanComboButtonPushFrame;

		int                ComboSwitchStartFrame;
		int                ComboSwitchEndFrame;

		int                AngleControllStartFrame;
		int                AngleControllEndFrame;
		RADIAN             AngleControllValue;

		int                LocusBoneNum;

		Vector2            VanishValue;

		bool			   changeFlg;

	};

	Param m_Param;

	BaseballAttackInfo_UsualAtk(
		BaseballPlayer* pOwner
		);

	~BaseballAttackInfo_UsualAtk();

private:

	BaseballPlayer * const  m_pOwner;
	Vector3               m_LocusPos, m_LocusVec;
	bool				  VanishFlg;
	//�_���[�W�p�����[�^���Z�b�g
	void DamageParamSet(DamageShpere* pDmg);

	//�_���[�W�ʒu���Z�b�g
	void DamagePosSet(DamageShpere* pDmg, BaseballPlayer* pBaseball);

	//���[�V�������Z�b�g
	void MotionSet(BaseballPlayer* pTennis);

	//�����̃t���[�����U���I�����ǂ���
	bool isEnd(int Frame);

	//�����̃t���[�����R���{�ڍs�t���[�����ǂ���
	bool isComboSwitchFrame(int Frame);

	//�����̃t���[�����A�R���{�̈ڍs����{�^����������t���[�����ǂ���
	bool isComboButtonFrame(int Frame);

	//�����̃t���[�����A�p�x�������s����t���[�����ǂ���
	bool isCanAngleControll(int Frame, RADIAN& OutControllVal);

	//�����̃t���[�����A�_���[�W����L���t���[�����ǂ���
	bool isDamageEnable(int Frame);

	//���t���[���Ă΂��
	void Update(int Frame, LocusHDR* pLocus);

	//�U�������������Ƃ��ɌĂ΂��
	void HitAttack(DamageShpere* pDmg);
    
    //�J�E���^�[�q�b�g���ǂ���
    bool isCounterHitFrame(int Frame);
};
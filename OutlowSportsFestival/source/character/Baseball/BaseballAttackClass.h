#pragma once

#include "iextreme.h"
#include "Damage/Damage.h"
#include "BaseballPlayer.h"
#include "../../utillity/LocusHDR.h"
#include <vector>

//****************************************************************
//		�싅�̍U���N���X(COM,PLAYER����)
//****************************************************************


class BaseballAttackClass
{
public:

	//�U������N���X
	class ControllEvent
	{
	public:
		virtual ~ControllEvent(){}
		virtual bool isDoCombo() = 0;    //�߂�Ƃ���true��Ԃ��ƃR���{���s������
		virtual void AngleControll(RADIAN angle) = 0;//�A���O���ύX���s����t���[���ɌĂ΂��
	};

	//�U�����N���X
	class AttackInfo
	{
	public:

		virtual ~AttackInfo(){}

		//�_���[�W�p�����[�^���Z�b�g
		virtual void DamageParamSet(DamageShpere* pDmg) = 0;

		//�_���[�W�ʒu���Z�b�g
		virtual void DamagePosSet(DamageShpere* pDmg, BaseballPlayer* pTennis) = 0;

		//���[�V�������Z�b�g
		virtual void MotionSet(BaseballPlayer* pTennis) = 0;

		//�����̃t���[�����U���I�����ǂ���
		virtual bool isEnd(int Frame) = 0;

		//�����̃t���[�����R���{�ڍs�t���[�����ǂ���
		virtual bool isComboSwitchFrame(int Frame) = 0;

		//�����̃t���[�����A�R���{�̈ڍs����{�^����������t���[�����ǂ���
		virtual bool isComboButtonFrame(int Frame) = 0;

		//�����̃t���[�����A�p�x�������s����t���[�����ǂ���
		virtual bool isCanAngleControll(int Frame, RADIAN& OutControllVal) = 0;

		//�����̃t���[�����A�_���[�W����L���t���[�����ǂ���
		virtual bool isDamageEnable(int Frame) = 0;

		//���t���[���Ă΂��
		virtual void Update(int Frame, LocusHDR* pLocus) = 0;

		//�U�������������Ƃ��ɌĂ΂��
		virtual void HitAttack(DamageShpere* pDmg) = 0;

        //�J�E���^�[�q�b�g���ǂ���
        virtual bool isCounterHitFrame(int Frame) = 0;
	};

	typedef std::vector<AttackInfo*> AttackInfoArray;

	AttackInfoArray       m_AttackInfoArray; //�U�����N���X�z��(���ꂽ�������R���{�ł���)


	BaseballAttackClass(
		BaseballPlayer*   pOwner,  //�싅�v���C���[�ւ̃|�C���^
		ControllEvent*  pEvent   //�C�x���g�N���X(�I������delete����)
		);

	~BaseballAttackClass();

	bool getDoHit()
	{
		return m_DoHit;
	}

	void Update();	//�X�V

	bool isEnd()const;   //�I���m�F



private:

	BaseballPlayer*  const  m_pOwner;
	ControllEvent* const  m_pEvent;
	DamageShpere	      m_Damage;
	int                   m_Timer;
	int                   m_ComboCount;
	bool                  m_DoCombo;
	bool                  m_DoHit;
	LocusHDR              m_Locus;
	int                   m_DamageHitCount;
	int                   m_HitStopCount;

	void(BaseballAttackClass::*m_pStateFunc)();

	void State_Attack();
	void State_NextAtk();
	void State_End();

	bool isLastAtk();
};

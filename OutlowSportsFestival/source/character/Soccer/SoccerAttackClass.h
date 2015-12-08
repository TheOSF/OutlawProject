#pragma once

#include "iextreme.h"
#include "Damage/Damage.h"
#include "SoccerPlayer.h"
#include "../../utillity/Locus.h"
#include <vector>

//****************************************************************
//		�T�b�J�[�̍U���N���X(COM,PLAYER����)
//****************************************************************


class SoccerAttackClass
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
		virtual void DamagePosSet(DamageShpere* pDmg, SoccerPlayer* pSoccer) = 0;

		//���[�V�������Z�b�g
		virtual void MotionSet(SoccerPlayer* pSoccer) = 0;

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
		virtual void Update(int Frame, Locus* pLocus) = 0;

		//�U���������������ɌĂ΂��
		virtual void HitAttack(DamageShpere* pDmg) = 0;

		//�q�b�g�X�g�b�v���ǂ���
		virtual bool isHitStopFrame() = 0;

		//�q�b�g�X�g�b�v���X�V
		virtual void HitStopUpdate() = 0;
	};

	typedef std::vector<AttackInfo*> AttackInfoArray;

	AttackInfoArray       m_AttackInfoArray; //�U�����N���X�z��(���ꂽ�������R���{�ł���)


	SoccerAttackClass(
		SoccerPlayer*   pOwner,  //�T�b�J�[�v���C���[�ւ̃|�C���^
		ControllEvent*  pEvent,   //�C�x���g�N���X(�I������delete����)
		int NoDamageFrame
		);

	~SoccerAttackClass();
	bool getDoHit()
	{
		return m_DoHit;
	}
	

	void Update();	//�X�V

	bool isEnd()const;   //�I���m�F



private:

	SoccerPlayer*  const  m_pOwner;
	ControllEvent* const  m_pEvent;
	DamageShpere	      m_Damage;
	int                   m_Timer;
	int                   m_ComboCount;
	int                   NoDamageFrame;
	bool                  m_DoCombo;
	bool                  m_DoHit;
	Locus                 m_Locus;
	int                   m_DamageHitCount;
	int                   m_HitStopCount;

	void(SoccerAttackClass::*m_pStateFunc)();

	void State_Attack();
	void State_NextAtk();
	void State_End();

	bool isLastAtk();
};

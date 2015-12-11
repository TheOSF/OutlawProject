#ifndef __SOCCER_PLAYER_STATE_H__
#define __SOCCER_PLAYER_STATE_H__

#include "SoccerSliding.h"
#include "SoccerDash.h"
#include "SoccerPlayer.h"

#include "SoccerAttackInfo_UsualAttack.h"

#include "../Attack/CharacterAttack.h"
#include "../../Ball/UsualBall.h"
#include "../CharacterMoveClass.h"
#include "../CharacterSliding.h"
#include "../Attack/CharacterAttack.h"
#include "../CharacterMoveClass.h"

#include "../CharacterShotAttackClass.h"
#include "../CharacterDamageMotion.h"


//****************************************************
//	�T�b�J�[�v���C���[�̑���N���X�w�b�_�[
//****************************************************


//�v���C���[����̈ړ��N���X
class SoccerState_PlayerControll_Move :public SoccerState
{
private:
	CharacterUsualMove*		m_pMoveClass;
	void ActionStateSwitch(SoccerPlayer* s);
public:

	static SoccerState* GetPlayerControllMove(SoccerPlayer* ps);
	static bool SwitchGameState(SoccerPlayer* ps);
	void Enter(SoccerPlayer* s);
	void Execute(SoccerPlayer* s);
	void Exit(SoccerPlayer* s);
};
//�v���C���[����̃X���C�f�B���O�N���X
class SoccerState_PlayerControll_Sliding :public SoccerState
{
	//�U������N���X
	class PlayerControllEvent :public SoccerAttackClass::ControllEvent
	{
	public:
		PlayerControllEvent(SoccerPlayer*const pSoccer);

		bool isDoCombo();
		void AngleControll(RADIAN angle);

	private:
		SoccerPlayer*const m_pSoccer;
		
		const CharacterBase* GetFrontTargetEnemy();
	};
public:
	SoccerState_PlayerControll_Sliding(SoccerPlayer* s);
	~SoccerState_PlayerControll_Sliding();

	// �X�e�[�g�J�n
	void Enter(SoccerPlayer* t)override;

	// �X�e�[�g���s
	void Execute(SoccerPlayer* t)override;

	// �X�e�[�g�I��
	void Exit(SoccerPlayer* t)override;

private:
	int m_Timer;
	SoccerAttackClass   m_Attack;        //�U���N���X�ւ̃|�C���^
};
//�v���C���[����̍U���N���X
class SoccerState_PlayerControll_Attack :public SoccerState
{
	//�U������N���X
	class PlayerControllEvent :public SoccerAttackClass::ControllEvent
	{
	public:
		PlayerControllEvent(SoccerPlayer*const pSoccer);

		bool isDoCombo();
		void AngleControll(RADIAN angle);

	private:
		SoccerPlayer*const m_pSoccer;

		const CharacterBase* GetFrontTargetEnemy();
	};
public:
	SoccerState_PlayerControll_Attack(SoccerPlayer* s);
	~SoccerState_PlayerControll_Attack();

	// �X�e�[�g�J�n
	void Enter(SoccerPlayer* t)override;

	// �X�e�[�g���s
	void Execute(SoccerPlayer* t)override;

	// �X�e�[�g�I��
	void Exit(SoccerPlayer* t)override;

private:
	SoccerAttackClass   m_Attack;        //�U���N���X�ւ̃|�C���^
};
//�v���C���[����̎ˌ��N���X
class SoccerState_PlayerControll_Shot :public SoccerState
{
private:
	CharacterShotAttack*		m_pShotClass;
public:
	void Enter(SoccerPlayer* s);
	void Execute(SoccerPlayer* s);
	void Exit(SoccerPlayer* s);
};
//�v���C���[����̃J�E���^�[�N���X
class SoccerState_PlayerControll_Counter : public SoccerState
{
private:
	CharacterDefaultCounter* m_pCounter; // �J�E���^�[�N���X
public:
	void Enter(SoccerPlayer* s);
	void Execute(SoccerPlayer* s);
	void Exit(SoccerPlayer* s);

};
//�v���C���[����̌ŗL�Z(�_�b�V��)�N���X
class SoccerState_PlayerControll_Dash : public SoccerState
{
private:
	SoccerDash*		m_pMoveClass;
	int m_timer;
public:
	void Enter(SoccerPlayer* s);
	void Execute(SoccerPlayer* s);
	void Exit(SoccerPlayer* s);
};
//�v���C���[����̕K�E�Z�N���X
class SoccerState_PlayerControll_Finisher : public SoccerState
{
private:
	//�@�������N���X�쐬
	CharacterShotAttack* SnakeShotClass(SoccerPlayer* s);
private:
	//�@�������N���X
	CharacterShotAttack* m_pSnakeShotClass;
	
public:
	int m_Timer;
	bool timeflg;
	//�@�R���X�g���N�^
	SoccerState_PlayerControll_Finisher();
	void Enter(SoccerPlayer* s);
	void Execute(SoccerPlayer* s);
	void Exit(SoccerPlayer* s);

};



#endif
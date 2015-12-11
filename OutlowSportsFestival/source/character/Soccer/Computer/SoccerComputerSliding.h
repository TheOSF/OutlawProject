#pragma once

#include "../SoccerSliding.h"
#include "../SoccerDash.h"
#include "../SoccerPlayer.h"

#include "../SoccerAttackInfo_UsualAttack.h"

#include "../../Attack/CharacterAttack.h"
#include "../../../Ball/UsualBall.h"
#include "../../CharacterMoveClass.h"
#include "../../CharacterSliding.h"
#include "../../Attack/CharacterAttack.h"
#include "../../CharacterMoveClass.h"

#include "../../CharacterShotAttackClass.h"

#include "../../CharacterDamageMotion.h"

//�v���C���[����̃X���C�f�B���O�N���X
class SoccerState_ComputerControll_Sliding :public SoccerState
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
	SoccerState_ComputerControll_Sliding(SoccerPlayer* s);
	~SoccerState_ComputerControll_Sliding();

	// �X�e�[�g�J�n
	void Enter(SoccerPlayer* s)override;

	// �X�e�[�g���s
	void Execute(SoccerPlayer* s)override;

	// �X�e�[�g�I��
	void Exit(SoccerPlayer* s)override;

private:
	int m_Timer;
	SoccerAttackClass   m_Attack;        //�U���N���X�ւ̃|�C���^
};
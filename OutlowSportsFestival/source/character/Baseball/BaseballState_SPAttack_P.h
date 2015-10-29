#pragma once

#include "BaseballPlayer.h"

//***************************************************
//		�싅(�s�b�`���[)�K�E�Z�X�e�[�g�N���X
//***************************************************
class CharacterShotAttack;
class BaseballState_SPAttack_P : public BaseballState
{
//public:
//	//�U������N���X
//	class PlayerControllEvent :public BaseballAttackClass::ControllEvent
//	{
//	public:
//		PlayerControllEvent(BaseballPlayer*const pBaseball);
//
//		void AngleControll(RADIAN angle);
//
//	private:
//		BaseballPlayer*const m_pBaseball;
//
//		const CharacterBase* GetFrontTargetEnemy();
//	};
//
private:
	CharacterShotAttack* CreateSpAttack_P(BaseballPlayer* b);
	CharacterShotAttack* m_pSpAttack_P;
	BaseballPlayer*        m_pBaseBall;

	int                     m_Timer;
	bool					timeflg;


public:

	BaseballState_SPAttack_P();
	
	
	// �X�e�[�g�J�n
	void Enter(BaseballPlayer* b)override;

	// �X�e�[�g���s
	void Execute(BaseballPlayer* b)override;

	// �X�e�[�g�I��
	void Exit(BaseballPlayer* b)override;

	void FreezeGame(UINT frame); //�@The World
};


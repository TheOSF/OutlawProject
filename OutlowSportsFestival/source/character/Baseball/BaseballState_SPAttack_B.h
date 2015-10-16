#pragma once

#include "BaseballPlayer.h"
#include "BaseballSpAttackClass.h"


//***************************************************
//		�v���C���[����� �ߋ����U���N���X
//***************************************************

class BaseballState_SPAttack_B : public BaseballState
{
public:

	//�U������N���X
	class PlayerControllEvent :public BaseballSpAttackClass::ControllEvent
	{
	public:
		PlayerControllEvent(BaseballPlayer*const pBaseball);

		bool isDoCombo();
		void AngleControll(RADIAN angle);

	private:
		BaseballPlayer*const m_pBaseball;

		const CharacterBase* GetFrontTargetEnemy();
	};

	BaseballState_SPAttack_B(BaseballPlayer* b);
	~BaseballState_SPAttack_B();

	// �X�e�[�g�J�n
	void Enter(BaseballPlayer* b)override;

	// �X�e�[�g���s
	void Execute(BaseballPlayer* b)override;

	// �X�e�[�g�I��
	void Exit(BaseballPlayer* b)override;

private:
	BaseballSpAttackClass   m_Attack;        //�U���N���X�ւ̃|�C���^
};


#ifndef __BASEBALL_PLAYER_CONTROLL_ATTACK_B_H__
#define __BASEBALL_PLAYER_CONTROLL_ATTACK_B_H__
										 
#include "BaseballPlayer.h"
#include "BaseballAttackClass.h"

//***************************************************
//		�v���C���[����� �ߋ����U���N���X
//***************************************************

class Baseball_PlayerControll_Attack_B : public BaseballState
{
public:

	//�U������N���X
	class PlayerControllEvent :public BaseballAttackClass::ControllEvent
	{
	public:
		PlayerControllEvent(BaseballPlayer*const pBaseball, BaseballAttackClass* Attack);

		bool isDoCombo();
		void AngleControll(RADIAN angle);
		bool ComDoCombo();

	private:
		BaseballPlayer*const m_pBaseball;
		BaseballAttackClass* m_Attack;

		const CharacterBase* GetFrontTargetEnemy();
	};

	Baseball_PlayerControll_Attack_B(BaseballPlayer* b);
	~Baseball_PlayerControll_Attack_B();

	// �X�e�[�g�J�n
	void Enter(BaseballPlayer* b)override;

	// �X�e�[�g���s
	void Execute(BaseballPlayer* b)override;

	// �X�e�[�g�I��
	void Exit(BaseballPlayer* b)override;

private:
	BaseballAttackClass   m_Attack;        //�U���N���X�ւ̃|�C���^
};

#endif
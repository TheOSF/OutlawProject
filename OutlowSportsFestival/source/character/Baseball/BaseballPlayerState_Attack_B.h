#ifndef __BASEBALL_PLAYER_CONTROLL_ATTACK_B_H__
#define __BASEBALL_PLAYER_CONTROLL_ATTACK_B_H__
										 
#include "BaseballPlayer.h"
#include "BaseballAttackClass.h"

//***************************************************
//		プレイヤー操作の 近距離攻撃クラス
//***************************************************

class Baseball_PlayerControll_Attack_B : public BaseballState
{
public:

	//攻撃操作クラス
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

	// ステート開始
	void Enter(BaseballPlayer* b)override;

	// ステート実行
	void Execute(BaseballPlayer* b)override;

	// ステート終了
	void Exit(BaseballPlayer* b)override;

private:
	BaseballAttackClass   m_Attack;        //攻撃クラスへのポインタ
};

#endif
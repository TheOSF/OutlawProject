#ifndef __BASEBALL_PLAYER_CONTROLL_ATTACK_P_H__
#define __BASEBALL_PLAYER_CONTROLL_ATTACK_P_H__

#include "BaseballPlayer.h"
#include "BaseballAttackClass.h"

//***************************************************
//		プレイヤー操作の 近距離攻撃クラス
//***************************************************

class Baseball_PlayerControll_Attack_P : public BaseballState
{
public:

	//攻撃操作クラス
	class PlayerControllEvent :public BaseballAttackClass::ControllEvent
	{
	public:
		PlayerControllEvent(BaseballPlayer*const pBaseball);

		bool isDoCombo();
		void AngleControll(RADIAN angle);

	private:
		BaseballPlayer*const m_pBaseball;

		const CharacterBase* GetFrontTargetEnemy();
	};

	Baseball_PlayerControll_Attack_P(BaseballPlayer* b);
	~Baseball_PlayerControll_Attack_P();

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
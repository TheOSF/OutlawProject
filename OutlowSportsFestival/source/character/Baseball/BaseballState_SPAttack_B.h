#pragma once

#include "BaseballPlayer.h"
#include "BaseballSpAttackClass.h"


//***************************************************
//		プレイヤー操作の 近距離攻撃クラス
//***************************************************

class BaseballState_SPAttack_B : public BaseballState
{
public:

	//攻撃操作クラス
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

	// ステート開始
	void Enter(BaseballPlayer* b)override;

	// ステート実行
	void Execute(BaseballPlayer* b)override;

	// ステート終了
	void Exit(BaseballPlayer* b)override;

private:
	BaseballSpAttackClass   m_Attack;        //攻撃クラスへのポインタ
};


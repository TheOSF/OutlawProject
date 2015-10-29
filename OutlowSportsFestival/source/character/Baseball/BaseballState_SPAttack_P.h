#pragma once

#include "BaseballPlayer.h"

//***************************************************
//		野球(ピッチャー)必殺技ステートクラス
//***************************************************
class CharacterShotAttack;
class BaseballState_SPAttack_P : public BaseballState
{
//public:
//	//攻撃操作クラス
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
	
	
	// ステート開始
	void Enter(BaseballPlayer* b)override;

	// ステート実行
	void Execute(BaseballPlayer* b)override;

	// ステート終了
	void Exit(BaseballPlayer* b)override;

	void FreezeGame(UINT frame); //　The World
};


#pragma once

#include "BaseballPlayer.h"

//***************************************************
//		野球(ピッチャー)必殺技ステートクラス
//***************************************************
class CharacterShotAttack;
class BaseballState_SPAttack_P : public BaseballState
{

private:
	CharacterShotAttack* CreateSpAttack_P(BaseballPlayer* b);
	CharacterShotAttack* m_pSpAttack_P;
	BaseballPlayer*      m_pBaseBall;
	CharacterBase*		 target;
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

	//　The World
	void FreezeGame(UINT frame); 

	//　ターゲット選定
	CharacterBase* CalcTarget()const;
};


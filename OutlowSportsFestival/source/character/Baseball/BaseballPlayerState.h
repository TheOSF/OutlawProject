#ifndef __BASEBALL_PLAYER_STATE_H__
#define __BASEBALL_PLAYER_STATE_H__

#include "BaseballPlayer.h"
#include "BaseballState_PlayerControll_ShotAttack_B.h"

#include "../CharacterMoveClass.h"
#include "../CharacterShotAttackClass.h"

//****************************************************
//	野球プレイヤーの操作クラスヘッダー
//****************************************************

//プレイヤー操作の移動クラス
class BaseballState_PlayerControll_Move :public BaseballState
{
private:
	CharacterUsualMove* m_pMoveClass;

private:
	bool batterflg;//　(true:バッター,false:投手)

public:
	//　コンストラクタ
	BaseballState_PlayerControll_Move() :batterflg(true){}
	//　ステート開始
	void Enter(BaseballPlayer* b)override;
	//　ステート実行
	void Execute(BaseballPlayer* b)override;
	//　ステート終了
	void Exit(BaseballPlayer* b)override;
public:
	static bool SwitchGameState(BaseballPlayer* pb);

	static BaseballState* GetPlayerControllMove(BaseballPlayer* pt);

	//　バッター時実行
	void Batter(BaseballPlayer* b);
	//　投手時実行
	void Pitcher(BaseballPlayer* b);
	//　セッター
	void SetBatterFlg(BaseballPlayer* b){ batterflg = b->getBatterFlg(); }
};


class PlayerShotControllClass_B :public BaseballState_PlayerControll_ShotAttack_B::ControllClass
{
	BaseballPlayer* const   m_pBaseball;
public:
	PlayerShotControllClass_B(BaseballPlayer* pBaseball) :
		m_pBaseball(pBaseball){}

	Vector3 GetVec();

	bool DoOtherAction();
	bool DoOtherAction_Com();
	bool DoShotAfterAction();
};

#endif
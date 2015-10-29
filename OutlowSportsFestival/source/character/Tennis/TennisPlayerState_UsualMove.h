#ifndef __TENNIS_PLAYER_STATE_USUALMOVE_H__
#define __TENNIS_PLAYER_STATE_USUALMOVE_H__

#include "TennisPlayer.h"
#include "../../GameSystem/ForwardDecl.h"


//****************************************************
//	テニスプレイヤーの操作クラスヘッダー
//****************************************************


//プレイヤー操作の移動クラス
class TennisState_PlayerControll_Move :public TennisState
{

public:
	static TennisState* GetPlayerControllMove(TennisPlayer* pt);
    static bool SwitchGameState(TennisPlayer* pt);

	void Enter(TennisPlayer* t)override;
	void Execute(TennisPlayer* t)override;
	void Exit(TennisPlayer* t)override;

private:
    CharacterUsualMove*		m_pMoveClass;

    void ActionStateSwitch(TennisPlayer* t);
};




#endif
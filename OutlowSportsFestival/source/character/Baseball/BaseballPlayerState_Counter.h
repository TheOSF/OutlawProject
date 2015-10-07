#ifndef __TENNIS_PLAYER_STATE_COUNTER_H__
#define __TENNIS_PLAYER_STATE_COUNTER_H__

#include "BaseballPlayer.h"
#include "../../GameSystem/ForwardDecl.h"

//***************************************************
//		プレイヤー操作の カウンタークラス
//***************************************************


class BaseballState_PlayerControll_Counter : public BaseballState
{
private:
	int areasize;
public:
	BaseballState_PlayerControll_Counter(int size);
	// ステート開始
	void Enter(BaseballPlayer* b)override;

	// ステート実行
	void Execute(BaseballPlayer* b)override;

	// ステート終了
	void Exit(BaseballPlayer* b)override;

private:
	// カウンタークラス作成
	CharacterDefaultCounter* m_pCounter; // カウンタークラス
};



#endif
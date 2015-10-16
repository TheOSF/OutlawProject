#pragma once

#include "BaseballPlayer.h"

class BaseballState_Change : public BaseballState
{
protected:
	bool batterflg;//　(true:バッター,false:投手)

public:
	BaseballState_Change();
	~BaseballState_Change();

public:
	// ステート開始
	void Enter(BaseballPlayer* b)override;

	// ステート実行
	void Execute(BaseballPlayer* b)override;

	// ステート終了
	void Exit(BaseballPlayer* b)override;
};
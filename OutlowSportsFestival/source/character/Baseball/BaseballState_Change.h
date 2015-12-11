#pragma once

#include "BaseballPlayer.h"

class BaseballState_Change : public BaseballState
{
private:
	bool batterflg;//　(true:バッター,false:投手)
	int m_Timer;
	BaseballPlayer* m_pBaseball;
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

private:
	void(BaseballState_Change::*m_pStateFunc)();

	void State_Change();
	void State_End();

};
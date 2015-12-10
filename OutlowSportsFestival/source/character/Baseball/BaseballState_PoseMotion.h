#pragma once

#include "BaseballPlayer.h"
#include "../CharacterEvasionClass.h"

//***************************************************
//		野球ポーズモーションクラス
//***************************************************

class BaseballState_PoseMotion : public BaseballState
{
public:

	BaseballState_PoseMotion(
		baseball_player::MotionType    Motion,
		RADIAN                      Speed,
		int                         Frame
		);

	~BaseballState_PoseMotion();

	// ステート開始
	void Enter(BaseballPlayer* b)override;

	// ステート実行
	void Execute(BaseballPlayer* b)override;

	// ステート終了
	void Exit(BaseballPlayer* b)override;

private:
	const  baseball_player::MotionType    m_Motion;
	const RADIAN                      m_Speed;
	int                               m_Frame;
};


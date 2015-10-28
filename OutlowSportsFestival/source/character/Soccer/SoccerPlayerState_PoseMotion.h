#pragma once
#include "SoccerPlayer.h"
#include "../CharacterEvasionClass.h"

//***************************************************
//		サッカーポーズモーションクラス
//***************************************************

class SoccerState_PoseMotion : public SoccerState
{
public:

	SoccerState_PoseMotion(
		SoccerPlayer::MotionType    Motion,
		RADIAN                      Speed,
		int                         Frame
		);

	~SoccerState_PoseMotion();

	// ステート開始
	void Enter(SoccerPlayer* t)override;

	// ステート実行
	void Execute(SoccerPlayer* t)override;

	// ステート終了
	void Exit(SoccerPlayer* t)override;

private:
	const SoccerPlayer::MotionType    m_Motion;
	const RADIAN                      m_Speed;
	int                               m_Frame;
};


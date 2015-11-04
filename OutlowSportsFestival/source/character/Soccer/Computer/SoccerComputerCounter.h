#pragma once
#include "../SoccerPlayer.h"
#include "../../CharacterDefaultCounterClass.h"


class SoccerState_ComputerControll_Counter : public SoccerState
{
private:
	CharacterDefaultCounter* m_pCounter; // カウンタークラス
public:
	void Enter(SoccerPlayer* s);
	void Execute(SoccerPlayer* s);
	void Exit(SoccerPlayer* s);

};
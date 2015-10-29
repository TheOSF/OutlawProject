#pragma once
#include "../SoccerPlayer.h"
#include "../SoccerHitEvent.h"
#include "../../CharacterShotAttackClass.h"
#include "../../../character/Computer/CharacterComputerMove.h"

class SoccerState_ComputerControll_Shot :public SoccerState
{
private:
	CharacterShotAttack*		m_pShotClass;
public:
	void Enter(SoccerPlayer* s)override;
	void Execute(SoccerPlayer* s)override;
	void Exit(SoccerPlayer* s)override;
};
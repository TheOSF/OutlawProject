#pragma once

#include "../SoccerRolling.h"

#include "../SoccerPlayerState.h"
#include "../SoccerPlayer.h"
class SocceComputerrUtillityClass
{
public:
	//���[�����O�̕�������N���X
	class ComputerRollingControll :public SoccerState_Rolling::CallBackClass
	{
	public:
		SoccerPlayer*const cs;
		ComputerRollingControll(SoccerPlayer* ps, Vector3 vec) :cs(cs), stick(vec) {}
		Vector3 stick;

		Vector3 GetVec()override;
	};

};
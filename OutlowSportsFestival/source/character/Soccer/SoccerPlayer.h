#ifndef __SOCCER_PLAYER_H__
#define __SOCCER_PLAYER_H__

#include "../CharacterBase.h"
#include "../CharacterStateTemplate.h"
#include "../../Ball/UsualBall.h"

class SoccerPlayer;

//ステートマシン型名付け

typedef ChrStateMachine<SoccerPlayer, CharacterBase::MsgType> SoccerStateMachine;
typedef ChrState<SoccerPlayer, CharacterBase::MsgType>SoccerState;

//*************************************************************
//		サッカープレイヤークラス
//*************************************************************

class SoccerPlayer :public CharacterBase
{
public:
	enum MotionType
	{
		_ms_Stand,
		_ms_Run,
		_ms_Atk1,
		_ms_Shot,
		_ms_Guard,
		_ms_Rolling,
		_ms_Damage,
		_ms_Blowing,
		_ms_Scrub,
		_ms_GetUp,
		_ms_Shot2,
		_ms_Win,
		_ms_Lose,
		_ms_Command,
		_ms_Atk2,
		_ms_Atk3,
		_ms_Counter,
		_ms_Crap
	};
protected:
	
	SoccerStateMachine* m_pStateMachine;

	
public:
	SoccerPlayer(const CharacterBase::PlayerInfo& info);
	~SoccerPlayer();

	struct SkillParam
	{
		float Attack1;
		float Attack2;
		float Attack3;
		float shot;
	};
	static const SkillParam skillparams;
	const float m_ModelSize;

	void SetState(SoccerState* state, bool Important=false);
	bool Update();
    bool CharacterMsg(MsgType mt);


};


#endif
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
        _ms_Stand = 1,
        _ms_Run = 3,
        _ms_RunStop = 4,
        _ms_DushStop = 6,
        _ms_Sliding_Start = 8,
        _ms_Sliding_End = 9,
        _ms_Rolling = 11,

        _ms_Atk1 = 13,
        _ms_Atk2 = 15,
        _ms_Atk3 = 17,

        _ms_CounterPose = 19,
        _ms_Counter_Right = 21,
        _ms_Counter_Left = 23,

        _ms_SpecialAtk = 25,
        _ms_Damage = 27,
        _ms_Vanish_Fly = 29,
        _ms_Vanish_Down = 31,
        _ms_StandUp = 32,
        _ms_HitWallAndFall = 36,
        _ms_FallAndDown = 38,

        _ms_Counter_Failed = _ms_RunStop, 
        _ms_Shot = 39,

        _ms_Win = 40,
        _ms_Lose = 41,

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

    bool SetState(SoccerState* state, int Important = 0);
	bool Update();
    bool CharacterMsg(MsgType mt);


};


#endif
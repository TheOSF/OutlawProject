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
        _ms_Atk2_Continue = 16,
        _ms_Atk3 = 18,

        _ms_CounterPose = 20,
        _ms_Counter_Right = 22,
        _ms_Counter_Left = 24,

        _ms_SpecialAtk = 26,
        _ms_Damage = 28,
        _ms_Vanish_Fly = 30,
        _ms_Vanish_Down = 32,
        _ms_StandUp = 33,
        _ms_HitWallAndFall = 37,
        _ms_FallAndDown = 39,

        _ms_Counter_Failed = _ms_RunStop, 
        _ms_Shot = 40,

        _ms_Win = 41,
        _ms_Lose = 42,

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
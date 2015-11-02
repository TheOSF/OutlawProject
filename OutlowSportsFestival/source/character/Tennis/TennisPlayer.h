#ifndef __TENNIS_PLAYER_H__
#define __TENNIS_PLAYER_H__

#include "../CharacterBase.h"
#include "../CharacterRenderer.h"
#include "../CharacterStateTemplate.h"	

class TennisPlayer;

//ステートマシン型名付け

typedef ChrStateMachine<TennisPlayer, CharacterBase::MsgType> TennisStateMachine;
typedef ChrState<TennisPlayer, CharacterBase::MsgType>TennisState;

//*************************************************************
//		テニスプレイヤークラス
//*************************************************************

class TennisPlayer:public CharacterBase
{
public:
    enum MotionType
    {
        _mt_Stand = 1,
        _mt_Run = 2,
        _mt_RunEnd=4,

        _mt_Attack1 = 11,
        _mt_Shot = 17,
        _mt_CounterRight = 17,
        _mt_CounterLeft = 18,
        _mt_Smash = 5,
        _mt_Guard = 6,
        _mt_Rolling = 10,
        _mt_BoundSmash = 3, 
        _mt_Damage_Weak = 21,

        _mt_Damage_Vanish_Fly = 22,
        _mt_Damage_Vanish_Down = 23,
        _mt_Damage_Vanish_StandUp = 24,

        _mt_WinPose = 31,
        _mt_LosePose = 33,

        _mt_Attack2 = 13,
        _mt_Attack3 = 15,

        _mt_CounterPose = 16,
        _mt_Hakushu = 21,

    };

	TennisPlayer(const CharacterBase::PlayerInfo& info);
	~TennisPlayer();

	
	CharacterRenderer	m_Renderer;
	const float         m_ModelSize;

	void SetState(TennisState* state);

	bool Update();
    bool CharacterMsg(MsgType mt);
    

    bool isCanBoundBallAtk();
    void SetDontBoundBallAtkTimer();

private:

	TennisStateMachine*		m_pStateMachine;
    UINT                    m_DontBoundBallAtkTimer;
};




#endif
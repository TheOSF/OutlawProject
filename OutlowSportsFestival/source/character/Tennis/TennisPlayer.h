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
        _mt_Stand = 0,
        _mt_Run = 1,

        _mt_Attack1 = 2,
        _mt_Shot = 5,
        _mt_CounterRight = 3,
        _mt_CounterLeft = 4,
        _mt_Smash = 5,
        _mt_Guard = 6,
        _mt_Rolling = 7,
        _mt_BoundSmash = 3, 
        _mt_Damage_Weak = 20,

        _mt_Damage_Vanish_Fly = 9,
        _mt_Damage_Vanish_Down = 10,
        _mt_Damage_Vanish_StandUp = 11,

        _mt_WinPose = 13,
        _mt_LosePose = 14,

        _mt_Attack2 = 16,
        _mt_Attack3 = 17,

        _mt_CounterPose = 18,
        _mt_Hakushu = 21,

    };

	TennisPlayer(const CharacterBase::PlayerInfo& info);
	~TennisPlayer();

	
	CharacterRenderer	m_Renderer;
	const float m_ModelSize;

	void SetState(TennisState* state);
	bool Update();
	bool Msg(MsgType mt);

private:

	TennisStateMachine*		m_pStateMachine;

};




#endif
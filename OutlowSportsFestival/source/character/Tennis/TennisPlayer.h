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
        _mt_RunEnd = 4,

        _mt_UpBall = 5,
        _mt_Smash = 6,
        _mt_CutShot = 7,
        _mt_ShotCancel = 9,

        _mt_EscapeStep = 10,

        _mt_Attack1 = 11,
        _mt_Attack2 = 13,
        _mt_Attack3 = 33,

        _mt_CounterPose = 15,

        _mt_CounterRight = 17,
        _mt_CounterLeft = 18,

        _mt_SpecialAtk = 19,

        _mt_Damage_Weak = 21,

        _mt_Damage_Vanish_Fly = 22, 
        _mt_Damage_Vanish_Down = 23,
        _mt_Damage_Vanish_StandUp = 24,

        _mt_Damage_Vanish_HitWallAndDown = 25,
        _mt_Damage_Vanish_HitFloor = 27,
        _mt_Damage_Vanish_HitFloorAndStandUp = 28, 


        _mt_WinPose = 29,
        _mt_LosePose = 31,

        _mt_Shot = 35, //無いモーション


    };

	TennisPlayer(const CharacterBase::PlayerInfo& info);
	~TennisPlayer();

	
	const float         m_ModelSize;

    void SetState(TennisState* state, bool Important = false);

	bool Update();
    bool CharacterMsg(MsgType mt);
    

    bool isCanBoundBallAtk();
    void SetDontBoundBallAtkTimer();

private:

	TennisStateMachine*		m_pStateMachine;
    UINT                    m_DontBoundBallAtkTimer;
};




#endif
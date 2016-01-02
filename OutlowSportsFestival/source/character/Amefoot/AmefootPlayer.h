#pragma once

#include "../CharacterBase.h"
#include "../CharacterStateTemplate.h"
#include "../CharacterRenderer.h"

class AmefootPlayer;
typedef ChrStateMachine<AmefootPlayer , CharacterBase::MsgType> AmefootStateMachine;
typedef ChrState<AmefootPlayer , CharacterBase::MsgType> AmefootState;


//-----------------------------------------------------------------------------------------
// AmefootPlayer
// [ アメフト ] キャラクタークラス
//-----------------------------------------------------------------------------------------
class AmefootPlayer : public CharacterBase
{
public:
     AmefootPlayer(const PlayerInfo& info);

     ~AmefootPlayer();

     bool Update()override;

     bool CharacterMsg(MsgType msg)override;

     void SetState(AmefootState* state , bool important = false);


public:
     enum MotionType
     {
          Motion_Stand = 0,
          Motion_Run = 1,
          Motion_CounterPose = 2,
          Motion_CounterCatch = 3,
          Motion_Counter = 4,
          Motion_CounterFailed = 5,

          Motion_WeakDamage = 6,
          Motion_Vanishing = 7,
          Motion_Down = 8,
          Motion_StandUp = 9,

          Motion_EvasionStandUp = 10,
          Motion_EvasionStart = 11,
     };

private:
     AmefootStateMachine* m_pStateMachine;


};
//-----------------------------------------------------------------------------------------


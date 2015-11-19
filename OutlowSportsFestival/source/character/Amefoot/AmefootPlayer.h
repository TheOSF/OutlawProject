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


     const float m_ModelSize;

public:
     enum MotionType
     {
          Motion_Stand,
          Motion_Run,
          Motion_Dummy_1,
          Motion_Dummy_2,
          Motion_Dummy_3,
          Motion_Dummy_4,
          Motion_WeakDamage,
          Motion_Vanishing,
          Motion_Down,
          Motion_StandUp,
     };

private:
     AmefootStateMachine* m_pStateMachine;


};
//-----------------------------------------------------------------------------------------


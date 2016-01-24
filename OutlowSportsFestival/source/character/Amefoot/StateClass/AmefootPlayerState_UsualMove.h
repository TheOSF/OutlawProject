#pragma once

#include "../AmefootPlayer.h"
#include "../../CharacterMoveClass.h"

//-----------------------------------------------------------------------------------------
// AmefootPlayerState_UsualMove
// [ アメフト ] 通常移動するステートクラス
//-----------------------------------------------------------------------------------------
class AmefootPlayerState_UsualMove : public AmefootState
{
public:
     AmefootPlayerState_UsualMove();

     void Enter(AmefootPlayer* pCharacter)override;

     void Execute(AmefootPlayer* pCharacter)override;

     void Exit(AmefootPlayer* pCharacter)override;

     static AmefootState* GetPlayerControllMove(AmefootPlayer* player);
     static bool SwitchGameState(AmefootPlayer* player);

private:
     void ActionStateSwitch(AmefootPlayer* pCharacter);

private:
     CharacterUsualMove* m_pCharacterUsualMove;

};

//-----------------------------------------------------------------------------------------
// MoveEvent
//-----------------------------------------------------------------------------------------
class AmefootUsualMove_MoveEvent : public CharacterUsualMove::MoveEvent
{
public:
    AmefootUsualMove_MoveEvent(AmefootPlayer* pAmefoot);

    void Update(bool isRun, RATIO speed)override;

    void RunStart()override;

    void StandStart()override;

    void RunEnd()override;

private:
    AmefootPlayer* m_pAmefoot;

};
//-----------------------------------------------------------------------------------------
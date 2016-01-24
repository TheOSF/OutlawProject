#pragma once

#include "../AmefootPlayer.h"
#include"../../Computer/CharacterComputerMove.h"
#include "../../Computer/CharacterComputerDoAction.h"
#include "../../Computer/CharacterComputerReaction.h"

//-----------------------------------------------------------------------------------------
// AmefootPlayerState_UsualMove
// [ アメフト ] 通常移動するステートクラス
//-----------------------------------------------------------------------------------------
class AmefootComputerState_UsualMove : public AmefootState
{
public:
    AmefootComputerState_UsualMove();

    void Enter(AmefootPlayer* pCharacter)override;

    void Execute(AmefootPlayer* pCharacter)override;

    void Exit(AmefootPlayer* pCharacter)override;

private:

    CharacterUsualMove*		    m_pMoveClass;
    CharacterComputerMove*      m_pCharacterComputerMove;
    CharacterComputerDoAction*  m_pCharacterComputerDoAction;
    CharacterComputerReaction*  m_pCharacterComputerReaction;

};
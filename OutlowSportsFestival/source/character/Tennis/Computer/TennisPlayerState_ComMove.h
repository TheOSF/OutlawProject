#pragma once

#include "../TennisPlayer.h"
#include"../../Computer/CharacterComputerMove.h"
#include "../../Computer/CharacterComputerDoAction.h"
#include "../../Computer/CharacterComputerReaction.h"

//****************************************************
//	�e�j�X�b�n�l�̑���N���X�w�b�_�[
//****************************************************

class TennisState_ComputerControll_Move :public TennisState
{
public:

    void Enter(TennisPlayer* t)override;
    void Execute(TennisPlayer* t)override;
    void Exit(TennisPlayer* t)override;

private:

    CharacterUsualMove*		    m_pMoveClass;
    CharacterComputerMove*      m_pCharacterComputerMove;
    CharacterComputerDoAction*  m_pCharacterComputerDoAction;
    CharacterComputerReaction*  m_pCharacterComputerReaction;
};

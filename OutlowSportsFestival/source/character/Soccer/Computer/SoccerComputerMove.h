#pragma once

#include "../SoccerPlayer.h"
#include "../../../GameSystem/ForwardDecl.h"
#include "../../../character/Computer/CharacterComputerMove.h"
#include "../../../character/Computer/CharacterComputerDoAction.h"
#include "../../../character/Computer/CharacterComputerReaction.h"


//****************************************************
//	サッカープレイヤーのコンピュータ移動操作クラスヘッダー
//****************************************************


//コンピュータ操作の移動クラス
class SoccerState_ComputerControll_Move :public SoccerState
{
public:

	void Enter(SoccerPlayer* s)override;
	void Execute(SoccerPlayer* s)override;
	void Exit(SoccerPlayer* s)override;

private:
    CharacterUsualMove*		    m_pMoveClass;
    CharacterComputerMove*      m_pCharacterComputerMove;
    CharacterComputerDoAction*  m_pCharacterComputerDoAction;
    CharacterComputerReaction*  m_pCharacterComputerReaction;
};

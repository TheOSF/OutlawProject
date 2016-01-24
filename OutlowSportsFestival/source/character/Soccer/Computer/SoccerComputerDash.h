#pragma once

#include "../SoccerPlayer.h"
#include "../../../GameSystem/ForwardDecl.h"
#include "../../../character/Computer/CharacterComputerMove.h"
#include "../../../character/Computer/CharacterComputerDoAction.h"
#include "../../../character/Computer/CharacterComputerReaction.h"
#include "../SoccerDash.h"

//****************************************************
//	サッカープレイヤーのコンピュータ移動操作クラスヘッダー
//****************************************************
class SoccerDash;

//コンピュータ操作の移動クラス
class SoccerState_ComputerControll_Dash :public SoccerState
{
public:
    //目的
    enum class MoveType
    {
        TargetAttack,
        Escape,
        SaveDistance
    };
	
    SoccerState_ComputerControll_Dash(
        SoccerPlayer* s,
        MoveType type,
        CharacterBase* pTarget);

	void Enter(SoccerPlayer* s)override;
	void Execute(SoccerPlayer* s)override;
	void Exit(SoccerPlayer* s)override;

private:

    CharacterBase* const m_pTarget;
    SoccerPlayer*  const m_pChr;
    Vector3(SoccerState_ComputerControll_Dash::*m_pStateFunc)();

    SoccerDash*		     m_pMoveClass;
    bool                 m_isRightTurn;
    int                  m_Timer;

    Vector3 State_Attack();
    Vector3 State_Escape();
    Vector3 State_SaveDistance();
};
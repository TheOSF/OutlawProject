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
	
	SoccerState_ComputerControll_Dash(SoccerPlayer* s);
	void Enter(SoccerPlayer* s)override;
	void Execute(SoccerPlayer* s)override;
	void Exit(SoccerPlayer* s)override;

	Vector2 StateMoveFront(SoccerPlayer* s);//　ターゲットに前進
	Vector2 StateMoveDistance(SoccerPlayer* s);//　ターゲットとの距離をとる
	Vector2 StateStop(SoccerPlayer* s);//　立ち止まる

	static bool SwitchGameState(SoccerPlayer* ps);
	Vector2 SwitchAction(SoccerPlayer* s);
private:

	//分岐用enum
	enum MoveMode
	{
		Stop,
		Forward,
		Distance,
	};
	CharacterComputerMove::Param m_cParam;
	MoveMode movemode;
	int       m_Count;
	Vector3   m_MoveTargetPos;
	SoccerDash*		m_pDashClass;

	SoccerPlayer* const m_cSoccer;
	int m_timer;
	
};
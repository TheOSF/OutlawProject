#pragma once

#include "../SoccerPlayer.h"
#include "../../Attack/CharacterAttack.h"
#include "../../Soccer/SoccerAttackInfo_UsualAttack.h"
#include "../../../GameSystem/ForwardDecl.h"
#include "../../../character/Computer/CharacterComputerMove.h"

//****************************************************
//	サッカープレイヤーのコンピュータ必殺技クラスヘッダー
//****************************************************
//コンピュータ操作の必殺技クラス
class SoccerState_ComputerControll_Finisher :public SoccerState
{

private:
	//　遠距離クラス作成
	CharacterShotAttack* SnakeShotClass(SoccerPlayer* s);
private:
	//　遠距離クラス
	CharacterShotAttack* m_pSnakeShotClass;

public:
	int m_Timer;
	bool timeflg;
	//　コンストラクタ
	SoccerState_ComputerControll_Finisher();

	static void FreezeGame(UINT frame, SoccerPlayer* s);
	
	void Enter(SoccerPlayer* s);
	void Execute(SoccerPlayer* s);
	void Exit(SoccerPlayer* s);

};
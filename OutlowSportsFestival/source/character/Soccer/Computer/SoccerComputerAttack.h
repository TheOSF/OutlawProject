#pragma once

#include "../SoccerPlayer.h"
#include "../../Attack/CharacterAttack.h"
#include "../../Soccer/SoccerAttackInfo_UsualAttack.h"
#include "../../../GameSystem/ForwardDecl.h"
#include "../../../character/Computer/CharacterComputerMove.h"

//****************************************************
//	サッカープレイヤーのコンピュータ攻撃操作クラスヘッダー
//****************************************************


//コンピュータ操作の攻撃クラス
class SoccerState_ComputerControll_Attack :public SoccerState
{
public:
	//攻撃操作クラス
	class ComputerControllEvent :public SoccerAttackClass::ControllEvent
	{
	public:
		ComputerControllEvent(SoccerPlayer*const pSoccer);

		bool isDoCombo();
		void AngleControll(RADIAN angle);

	private:
		SoccerPlayer*const m_pSoccer;

		const CharacterBase* GetFrontTargetEnemy();
	};
	SoccerState_ComputerControll_Attack(SoccerPlayer* s);
	~SoccerState_ComputerControll_Attack();

	void Enter(SoccerPlayer* s)override;
	void Execute(SoccerPlayer* s)override;
	void Exit(SoccerPlayer* s)override;
private:
	SoccerAttackClass m_Attack;
};
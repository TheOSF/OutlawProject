#pragma once

#include "../SoccerSliding.h"
#include "../SoccerDash.h"
#include "../SoccerPlayer.h"

#include "../SoccerAttackInfo_UsualAttack.h"

#include "../../Attack/CharacterAttack.h"
#include "../../../Ball/UsualBall.h"
#include "../../CharacterMoveClass.h"
#include "../../CharacterSliding.h"
#include "../../Attack/CharacterAttack.h"
#include "../../CharacterMoveClass.h"

#include "../../CharacterShotAttackClass.h"

#include "../../CharacterDamageMotion.h"

//プレイヤー操作のスライディングクラス
class SoccerState_ComputerControll_Sliding :public SoccerState
{
	//攻撃操作クラス
	class PlayerControllEvent :public SoccerAttackClass::ControllEvent
	{
	public:
		PlayerControllEvent(SoccerPlayer*const pSoccer);

		bool isDoCombo();
		void AngleControll(RADIAN angle);

	private:
		SoccerPlayer*const m_pSoccer;

		const CharacterBase* GetFrontTargetEnemy();
	};
public:
	SoccerState_ComputerControll_Sliding(SoccerPlayer* s);
	~SoccerState_ComputerControll_Sliding();

	// ステート開始
	void Enter(SoccerPlayer* s)override;

	// ステート実行
	void Execute(SoccerPlayer* s)override;

	// ステート終了
	void Exit(SoccerPlayer* s)override;

private:
	int m_Timer;
	SoccerAttackClass   m_Attack;        //攻撃クラスへのポインタ
};
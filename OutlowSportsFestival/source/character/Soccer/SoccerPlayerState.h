#ifndef __SOCCER_PLAYER_STATE_H__
#define __SOCCER_PLAYER_STATE_H__

#include "SoccerSliding.h"
#include "SoccerDash.h"
#include "SoccerPlayer.h"

#include "SoccerAttackInfo_UsualAttack.h"

#include "../Attack/CharacterAttack.h"
#include "../../Ball/UsualBall.h"
#include "../CharacterMoveClass.h"
#include "../CharacterSliding.h"
#include "../Attack/CharacterAttack.h"
#include "../CharacterMoveClass.h"

#include "../CharacterShotAttackClass.h"
#include "../CharacterDamageMotion.h"


//****************************************************
//	サッカープレイヤーの操作クラスヘッダー
//****************************************************


//プレイヤー操作の移動クラス
class SoccerState_PlayerControll_Move :public SoccerState
{
private:
	CharacterUsualMove*		m_pMoveClass;
	void ActionStateSwitch(SoccerPlayer* s);
public:

	static SoccerState* GetPlayerControllMove(SoccerPlayer* ps);
	static bool SwitchGameState(SoccerPlayer* ps);
	void Enter(SoccerPlayer* s);
	void Execute(SoccerPlayer* s);
	void Exit(SoccerPlayer* s);
};
//プレイヤー操作のスライディングクラス
class SoccerState_PlayerControll_Sliding :public SoccerState
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
	SoccerState_PlayerControll_Sliding(SoccerPlayer* s);
	~SoccerState_PlayerControll_Sliding();

	// ステート開始
	void Enter(SoccerPlayer* t)override;

	// ステート実行
	void Execute(SoccerPlayer* t)override;

	// ステート終了
	void Exit(SoccerPlayer* t)override;

private:
	int m_Timer;
	SoccerAttackClass   m_Attack;        //攻撃クラスへのポインタ
};
//プレイヤー操作の攻撃クラス
class SoccerState_PlayerControll_Attack :public SoccerState
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
	SoccerState_PlayerControll_Attack(SoccerPlayer* s);
	~SoccerState_PlayerControll_Attack();

	// ステート開始
	void Enter(SoccerPlayer* t)override;

	// ステート実行
	void Execute(SoccerPlayer* t)override;

	// ステート終了
	void Exit(SoccerPlayer* t)override;

private:
	SoccerAttackClass   m_Attack;        //攻撃クラスへのポインタ
};
//プレイヤー操作の射撃クラス
class SoccerState_PlayerControll_Shot :public SoccerState
{
private:
	CharacterShotAttack*		m_pShotClass;
public:
	void Enter(SoccerPlayer* s);
	void Execute(SoccerPlayer* s);
	void Exit(SoccerPlayer* s);
};
//プレイヤー操作のカウンタークラス
class SoccerState_PlayerControll_Counter : public SoccerState
{
private:
	CharacterDefaultCounter* m_pCounter; // カウンタークラス
public:
	void Enter(SoccerPlayer* s);
	void Execute(SoccerPlayer* s);
	void Exit(SoccerPlayer* s);

};
//プレイヤー操作の固有技(ダッシュ)クラス
class SoccerState_PlayerControll_Dash : public SoccerState
{
private:
	SoccerDash*		m_pMoveClass;
	int m_timer;
public:
	void Enter(SoccerPlayer* s);
	void Execute(SoccerPlayer* s);
	void Exit(SoccerPlayer* s);
};
//プレイヤー操作の必殺技クラス
class SoccerState_PlayerControll_Finisher : public SoccerState
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
	SoccerState_PlayerControll_Finisher();
	void Enter(SoccerPlayer* s);
	void Execute(SoccerPlayer* s);
	void Exit(SoccerPlayer* s);

};



#endif
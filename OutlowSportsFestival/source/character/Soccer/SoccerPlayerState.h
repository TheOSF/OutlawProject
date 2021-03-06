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
#include "SoccerRolling.h"
#include "../../GameSystem/GameController.h"

//****************************************************
//	サッカープレイヤーの操作クラスヘッダー
//****************************************************


class SoccerUtillityClass
{
public:
    //ローリングの方向制御クラス
    class PlayerRollingControll :public SoccerState_Rolling::CallBackClass
    {
    public:
        SoccerPlayer*const ps;

        PlayerRollingControll(SoccerPlayer* ps) :ps(ps){}


        Vector3 GetVec()override
        {
            Vector2 stick = controller::GetStickValue(controller::stick::left, ps->m_PlayerInfo.number);
            Vector3 vec(stick.x, 0, stick.y);

            if (vec.Length() < 0.25f)
            {
                return Vector3Zero;
            }

            vec = DefCamera.GetRight()*vec.x + DefCamera.GetForward()*vec.z;
            vec.y = 0;
            vec.Normalize();

            return vec;
        }
    };

};

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

class SoccerMoveEvent :public CharacterUsualMove::MoveEvent
{
    SoccerPlayer* m_pSoccer;
public:
    SoccerMoveEvent(SoccerPlayer* pSoccer) :
        m_pSoccer(pSoccer){}

    //アニメーションの更新
    void Update(bool isRun, RATIO speed_ratio)
    {
        m_pSoccer->m_Renderer.Update(0.65f);
    }
    //走り始めにモーションをセット
    void RunStart()
    {
        m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_Run);
    }
    //立ちはじめにモーションをセット
    void StandStart()
    {
        m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_Stand);
    }

    //走り終わり
    void RunEnd()
    {
        m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_RunStop);
    }
};

//プレイヤー操作のスライディングクラス
class SoccerState_Sliding :public SoccerState
{
public:
    SoccerState_Sliding(SoccerPlayer* s);
	// ステート開始
	void Enter(SoccerPlayer* t)override;

	// ステート実行
	void Execute(SoccerPlayer* t)override;

	// ステート終了
	void Exit(SoccerPlayer* t)override;

private:
    typedef void(SoccerState_Sliding::*StateFunc)();

    StateFunc               m_pStateFunc;
    SoccerPlayer* const     m_pChr;
    DamageShpere            m_Damage;
	int                     m_Timer;
    float                   m_SaveChrSize;
    
    void SetState(StateFunc State);
    void UpdateDamage();
    void NoBallDamageUpdate();
    void StartAngleControll();
    void AutoAngleControll();
    void SetMoveByAngle();
    void SetEffect(RATIO alpha);

    void State_Start();
    void State_Sliding();
    void State_End();

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

class SoccerDashEvent
{
    SoccerPlayer* m_pSoccer;
public:
    SoccerDashEvent(SoccerPlayer* pSoccer) :
        m_pSoccer(pSoccer){}

    void Update(bool isRun, RATIO speed_ratio)
    {
        m_pSoccer->m_Renderer.Update(1.5f);
    }

    void RunStart()
    {
        m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_Run);
    }

    void StandStart()
    {
        m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_Stand);
    }
};

//プレイヤー操作の必殺技クラス
class SoccerState_PlayerControll_Finisher : public SoccerState
{
public:
	int m_Timer;
	//　コンストラクタ
	SoccerState_PlayerControll_Finisher();
	void Enter(SoccerPlayer* s);
	void Execute(SoccerPlayer* s);
	void Exit(SoccerPlayer* s);

};



#endif
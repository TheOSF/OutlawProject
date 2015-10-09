#include "TennisPlayerState_UsualMove.h"
#include "TennisState_Shot.h"
#include "TennisPlayerState_Counter.h"
#include "../../GameSystem/GameController.h"
#include "../CharacterFunction.h"
#include "../../Ball/UsualBall.h"
#include "../../Damage/Damage.h"
#include "../CharacterCounterClass.h"
#include "../CharacterMoveClass.h"
#include "../CharacterShotAttackClass.h"
#include "Tennis_HitEvent.h"
#include "TennisPlayerState_Attack.h"
#include "TennisState_Rolling.h"
#include "TennisState_BoundBallAtk.h"

#include "Computer\TennisPlayerState_ComMove.h"
#include "../../Camera/Camera.h"


//ローリングの高校制御クラス
class PlayerRollingControll :public TennisState_Rolling::CallBackClass
{
public:
    TennisPlayer*const pt;

    PlayerRollingControll(TennisPlayer* pt) :pt(pt){}


    Vector3 GetVec()override
    {
        Vector2 stick = controller::GetStickValue(controller::stick::left, pt->m_PlayerInfo.number);
        Vector3 vec(stick.x, 0, stick.y);

        if (vec.Length() < 0.25f)
        {
            return Vector3Zero;
        }

        vec = DefCamera.GetRight()*vec.x + DefCamera.GetUp()*vec.z;
        vec.Normalize();

        return vec;
    }
};


//バウンド攻撃の方向制御クラス
class PlayerBoundBallControll :public TennisState_BoundBallAtk::ControllClass
{
public:
    TennisPlayer*const pt;

    PlayerBoundBallControll(TennisPlayer* pt) :pt(pt){}

    Vector3 GetBoundVec()override
    {
        Vector2 stick = controller::GetStickValue(controller::stick::left, pt->m_PlayerInfo.number);
        Vector3 vec(stick.x, 0, stick.y);

        if (vec.Length() < 0.25f)
        {
            return Vector3Zero;
        }

        vec = DefCamera.GetRight()*vec.x + DefCamera.GetUp()*vec.z;
        vec.Normalize();

        return vec;
    }
};


//ショット中のコントロールクラス
class PlayerShotControllClass :public TennisState_Shot::ControllClass
{
    TennisPlayer* const   m_pTennis;
public:
    PlayerShotControllClass(TennisPlayer* pTennis) :
        m_pTennis(pTennis){}

    Vector3 GetVec()
    {
        Vector2 stick = controller::GetStickValue(controller::stick::left, m_pTennis->m_PlayerInfo.number);
        Vector3 vec(stick.x, 0, stick.y);

        if (vec.Length() < 0.25f)
        {
            return Vector3Zero;
        }

        vec = DefCamera.GetRight()*vec.x + DefCamera.GetUp()*vec.z;
        vec.Normalize();

        return vec;
    }

    bool DoOtherAction()
    {
        TennisPlayer * const t = m_pTennis;

        if (controller::GetTRG(controller::button::shikaku, t->m_PlayerInfo.number))
        {// [□] で [近距離攻撃]
            t->SetState(new TennisState_PlayerControll_Attack(t));
            return true;
        }

        if (controller::GetTRG(controller::button::batu, t->m_PlayerInfo.number))
        {// [×] で [ローリング]
            t->SetState(new TennisState_Rolling(new PlayerRollingControll(t)));
            return true;
        }

        if (controller::GetTRG(controller::button::_R1, t->m_PlayerInfo.number))
        {// [R1] で [カウンター]
            t->SetState(new TennisState_PlayerControll_Counter());
            return true;
        }

        if (controller::GetTRG(controller::button::_L1, t->m_PlayerInfo.number))
        {// [L1] で [バウンドボール攻撃
            t->SetState(new TennisState_BoundBallAtk(new PlayerBoundBallControll(t)));
            return true;
        }

        return false;
    }

    bool DoShotAfterAction()
    {
        TennisPlayer * const t = m_pTennis;

        if (controller::GetTRG(controller::button::shikaku, t->m_PlayerInfo.number))
        {// [□] で [近距離攻撃]
            t->SetState(new TennisState_PlayerControll_Attack(t));
            return true;
        }

        return false;
    }
};

//****************************************************
//	テニスプレイヤーの操作クラス
//****************************************************


TennisState* TennisState_PlayerControll_Move::GetPlayerControllMove(
	TennisPlayer* pt)
{
	switch (pt->m_PlayerInfo.player_type)
	{
	case PlayerType::_Player:
		return new TennisState_PlayerControll_Move();

	case PlayerType::_Computer:

        return new TennisState_ComputerControll_Move();

		switch (pt->m_PlayerInfo.strong_type)
		{
		case StrongType::_Weak:
			//return new 弱い通常移動
		case StrongType::_Usual:
			//return new 普通の通常移動
		case StrongType::_Strong:
			//return new 強い通常移動
		default:break;
		}

	default:break;
	}

	assert("通常ステートが作成できないキャラクタタイプです TennisState_PlayerControll_Move::GetPlayerControllMove" && 0);
	return nullptr;
}

//ステート開始
void TennisState_PlayerControll_Move::Enter(TennisPlayer* t)
{
	//移動イベントクラス
	class TennisMoveEvent :public CharacterUsualMove::MoveEvent
	{
		TennisPlayer* m_pTennis;
	public:
		TennisMoveEvent(TennisPlayer* pTennis) :
			m_pTennis(pTennis){}

		//アニメーションの更新
		void Update(bool isRun, RATIO speed_ratio)
		{
			m_pTennis->m_Renderer.Update(1);
		}
		//走り始めにモーションをセット
		void RunStart()
		{
			m_pTennis->m_Renderer.SetMotion(TennisPlayer::_mt_Run);
		}
		//立ちはじめにモーションをセット
		void StandStart()
		{
			m_pTennis->m_Renderer.SetMotion(TennisPlayer::_mt_Stand);
		}
	};

	//移動パラメータを代入
	CharacterUsualMove::Params p;

	p.Acceleration = 0.2f;
	p.MaxSpeed = 0.2f;
	p.TurnSpeed = 0.3f;
	p.DownSpeed = 0.2f;

	//移動クラスの作成
	m_pMoveClass = new CharacterUsualMove(
		t,
		p,
		new TennisMoveEvent(t),
		new TennisHitEvent(t)
		);

    //初期のたちモーションセット
    t->m_Renderer.SetMotion(TennisPlayer::_mt_Stand);
}


void TennisState_PlayerControll_Move::Execute(TennisPlayer* t)
{



    if (controller::GetTRG(controller::button::sankaku, t->m_PlayerInfo.number))
    {// [△] でボール発射
        t->SetState(new TennisState_Shot(new PlayerShotControllClass(t)));
        return;
    }

    if (controller::GetTRG(controller::button::shikaku, t->m_PlayerInfo.number))
    {// [□] で [近距離攻撃]
        t->SetState(new TennisState_PlayerControll_Attack(t));
        return;
    }

    if (controller::GetTRG(controller::button::batu, t->m_PlayerInfo.number))
    {// [×] で [ローリング]
        t->SetState(new TennisState_Rolling(new PlayerRollingControll(t)));
        return;
    }

    if (controller::GetTRG(controller::button::_R1, t->m_PlayerInfo.number))
    {// [R1] で [カウンター]
        t->SetState(new TennisState_PlayerControll_Counter());
        return;
    }

    if (controller::GetTRG(controller::button::_L1, t->m_PlayerInfo.number) &&
        t->isCanBoundBallAtk())
    {// [L1] で [バウンドボール攻撃
        t->SetState(new TennisState_BoundBallAtk(new PlayerBoundBallControll(t)));
        return;
    }

    {
        //スティックの値をセット
        Vector2 st = controller::GetStickValue(controller::stick::left, t->m_PlayerInfo.number);
        Vector3 st_vec3;

        //ビュー空間に変換
        st_vec3 = DefCamera.GetRight()*st.x + DefCamera.GetForward()*st.y;
        st.x = st_vec3.x;
        st.y = st_vec3.z;

        //スティックの値セット
        m_pMoveClass->SetStickValue(st);

    }

    //更新
    m_pMoveClass->Update();


    //モデルのワールド変換行列を更新
    chr_func::CreateTransMatrix(t, t->m_ModelSize, &t->m_Renderer.m_TransMatrix);
}

void TennisState_PlayerControll_Move::Exit(TennisPlayer* t)
{
	delete m_pMoveClass;
}



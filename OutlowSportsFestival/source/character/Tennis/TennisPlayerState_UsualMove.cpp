#include "TennisPlayerState_UsualMove.h"
#include "TennisState_BoundShot.h"
#include "TennisPlayerState_Counter.h"
#include "../../GameSystem/GameController.h"
#include "../CharacterFunction.h"
#include "../../Ball/UsualBall.h"
#include "../../Damage/Damage.h"
#include "../CharacterMoveClass.h"
#include "../CharacterShotAttackClass.h"
#include "Tennis_HitEvent.h"
#include "TennisPlayerState_Attack.h"
#include "TennisState_Rolling.h"

#include "Computer\TennisPlayerState_ComMove.h"
#include "../../Camera/Camera.h"
#include "TennisPlayerState_DamageMotionDie.h"
#include "TennisPlayerState_PoseMotion.h"

#include "../../Render/LightObject.h"
#include "TennisPlayerState_SlowUpBall.h"
#include "../../Effect/SoccerSpecialHitEffect.h"
#include "TennisSpecialAtk.h"
#include "TennisUtillityClass.h"


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


bool TennisState_PlayerControll_Move::SwitchGameState(TennisPlayer* pt)
{
    Vector3 v;

    switch (pt->GetStateType())
    {
    case CharacterBase::State::Usual:

        return false;

    case CharacterBase::State::Freeze:

        return true;

    case CharacterBase::State::LosePose:
        pt->SetState(new TennisState_PoseMotion(TennisPlayer::_mt_LosePose, 0.2f, 1000));
        return true;

    case CharacterBase::State::WinPose:
        pt->SetState(new TennisState_PoseMotion(TennisPlayer::_mt_WinPose, 0.2f, 1000));

        return true;
    default:
        break;
    }

    return false;


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

        //走り終わりモーションをセット
        void RunEnd()
        {
            m_pTennis->m_Renderer.SetMotion(TennisPlayer::_mt_RunEnd);
        }
	};

	//移動パラメータを代入
	CharacterUsualMove::Params p;

	p.Acceleration = 0.15f;
	p.MaxSpeed = 0.28f;
	p.TurnSpeed = 0.3f;
	p.DownSpeed = 0.08f;
    p.RunEndFrame = 35;

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
    if (SwitchGameState(t) == false)
    {
        //各アクションへ移行可能
        ActionStateSwitch(t);

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
    else
    {
        //スティックの値セット
        m_pMoveClass->SetStickValue(Vector2(0, 0));
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

void TennisState_PlayerControll_Move::ActionStateSwitch(TennisPlayer* t)
{
    if (controller::GetTRG(controller::button::sankaku, t->m_PlayerInfo.number))
    {// [△] でボール発射
        t->SetState(new TennisPlayerState_SlowUpBall(new TennisUtillityClass::PlayerSlowBallControll(t)));
        return;
    }

    if (chr_func::isCanSpecialAttack(t) && controller::GetTRG(controller::button::maru, t->m_PlayerInfo.number))
    {// [○] で必殺技
        t->SetState(new TennisState_SpecialAtk(t));
        chr_func::ResetSkillGauge(t);
        return;
    }

    if (controller::GetTRG(controller::button::shikaku, t->m_PlayerInfo.number))
    {// [□] で [近距離攻撃]
        t->SetState(new TennisState_PlayerControll_Attack(t, false));
        return;
    }

    if (controller::GetTRG(controller::button::batu, t->m_PlayerInfo.number))
    {// [×] で [ローリング]
        t->SetState(new TennisState_Rolling(new TennisUtillityClass::PlayerRollingControll(t)));
        return;
    }

    if (controller::GetTRG(controller::button::_R1, t->m_PlayerInfo.number))
    {// [R1] で [カウンター]
        t->SetState(new TennisState_PlayerControll_Counter());
        return;
    }

    if (controller::GetTRG(controller::button::_L1, t->m_PlayerInfo.number) &&
        t->isCanBoundBallAtk())
    {// [L1] で ボールを上に投げる攻撃
        t->SetState(new TennisState_BoundShot(new TennisUtillityClass::PlayerShotControllClass(t)));
        return;
    }
}
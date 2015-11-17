#include "../Computer/SoccerComputerMove.h"
#include "../../Computer/CharacterComputerReactionHitEvent.h"
#include "../../CharacterFunction.h"
#include "../../CharacterCounterClass.h"
#include "../../CharacterMoveClass.h"
#include "SoccerComputerAttack.h"
#include "SoccerComputerDash.h"
#include "../SoccerPlayerState.h"
#include "../SoccerRolling.h"
#include "SoccerComputerShot.h"
#include "SoccerComputerCounter.h"
#include "SoccerComputerRolling.h"
#include "../SoccerPlayerState_PoseMotion.h"
#include "../../CharacterManager.h"
#include "../../../Camera/Camera.h"
#include "../SoccerHitEvent.h"


class SocceComputerrUtillityClass
{
public:
	//ローリングの方向制御クラス
	class ComputerRollingControll :public SoccerState_ComputerControll_Rolling::CallBackClass
	{
	public:
		SoccerPlayer*const cs;
		ComputerRollingControll(SoccerPlayer* ps,Vector3 vec) :cs(cs), stick(vec) {}
		Vector3 stick;

		Vector3 GetVec()override
		{
			
			Vector3 vec(stick.x, 0, stick.z);

			return vec;
		}
	};

};

bool SoccerState_ComputerControll_Move::SwitchGameState(SoccerPlayer* ps)
{
	Vector3 v;

	switch (ps->GetStateType())
	{
	case CharacterBase::State::Usual:

		return false;

	case CharacterBase::State::Freeze:

		return true;

	case CharacterBase::State::LosePose:
		ps->SetState(new SoccerState_PoseMotion(SoccerPlayer::_ms_Lose, 0.2f, 1000));
		return true;

	case CharacterBase::State::WinPose:
		ps->SetState(new SoccerState_PoseMotion(SoccerPlayer::_ms_Win, 0.2f, 1000));

		return true;
	default:
		break;
	}

	return false;


}

//ステート開始
void SoccerState_ComputerControll_Move::Enter(SoccerPlayer* s)
{
	//移動イベントクラス
	class SoccerMoveEvent :public CharacterUsualMove::MoveEvent
	{
		SoccerPlayer* m_cSoccer;
	public:
		SoccerMoveEvent(SoccerPlayer* cSoccer) :
			m_cSoccer(cSoccer){}

		//アニメーションの更新
		void Update(bool isRun, RATIO speed_ratio)
		{
			m_cSoccer->m_Renderer.Update(1);
		}
		//走り始めにモーションをセット
		void RunStart()
		{

			m_cSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_Run);
		}
		//立ちはじめにモーションをセット
		void StandStart()
		{
			m_cSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_Stand);
		}
	};

	//移動パラメータを代入
	CharacterUsualMove::Params Movep;

	Movep.Acceleration = 0.2f;
	Movep.MaxSpeed = 0.2f;
	Movep.TurnSpeed = 0.3f;
	Movep.DownSpeed = 0.2f;

	//移動コントロールクラスの作成
	m_pMoveControllClass = new CharacterComputerMove(s);

	//移動クラスの作成
	m_pMoveClass = new CharacterUsualMove(
		s,
		Movep,
		new SoccerMoveEvent(s),
		new SoccerHitEvent(s)
		);


	//攻撃イベントクラス
	class SoccerDoEvent :public CharacterComputerDoAction::ActionEvent
	{
		
		SoccerPlayer* m_cSoccer;
	public:
		SoccerDoEvent(SoccerPlayer* cSoccer) :
			m_cSoccer(cSoccer) {}

		//アニメーションの更新
		void Attack(float len)override
		{
			
			if (len < 6.0f)
			{
				m_cSoccer->SetState(new SoccerState_ComputerControll_Attack(m_cSoccer));
			}
			else if (len < 20.0f)
			{
				m_cSoccer->SetState(new SoccerState_ComputerControll_Shot);
			}
		}

	};
	CharacterComputerMove::Param cParam;
	m_pMoveControllClass->GetParams(cParam, s->m_PlayerInfo.strong_type);
	//攻撃クラスの作成
	m_pDoActionClass = new CharacterComputerDoAction(
		s, 
		cParam,
		new SoccerDoEvent(s), 
		new SoccerHitEvent(s)
		);

	//反応イベントクラス
	class SoccerReactionEvent :public CharacterComputerReaction::ActionEvent
	{
		Vector3 Vec;
		SoccerPlayer* m_cSoccer;
	public:
		SoccerReactionEvent(SoccerPlayer* cSoccer) :
			m_cSoccer(cSoccer){}

		//アニメーションの更新
		void Reaction(CharacterComputerReactionHitEvent::HitType hittype, Vector3 vec)override
		{
			if (hittype == CharacterComputerReactionHitEvent::HitType::CanCounter)
			{
				m_cSoccer->SetState(new SoccerState_PlayerControll_Counter);
				
			}
			else
			{
				m_cSoccer->SetState(
					new SoccerState_ComputerControll_Rolling
					(new SocceComputerrUtillityClass::ComputerRollingControll(m_cSoccer,vec),
						false));
			}
		}

	};
	//反応クラスの作成
	m_pReactionClass = new CharacterComputerReaction(
		s,
		cParam,
		new SoccerReactionEvent(s)
		);
}


void SoccerState_ComputerControll_Move::Execute(SoccerPlayer* s)
{
	if (SwitchGameState(s) == false)
	{
		m_pMoveClass->SetStickValue(m_pMoveControllClass->SwitchAction(s));
	}
	if (rand() % 100 == 0)
	{
		//s->SetState(new SoccerState_ComputerControll_Dash(s));
	}
	//更新
	m_pMoveClass->Update();
	m_pDoActionClass->Update();
	m_pReactionClass->Update();

	//モデルのワールド変換行列を更新
	chr_func::CreateTransMatrix(s, s->m_ModelSize, &s->m_Renderer.m_TransMatrix);

}

void SoccerState_ComputerControll_Move::Exit(SoccerPlayer* t)
{
	delete m_pMoveClass;
	delete m_pMoveControllClass; 
	delete m_pDoActionClass;
	delete m_pReactionClass;
}

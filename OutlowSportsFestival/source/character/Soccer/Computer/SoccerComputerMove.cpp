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
#include "SoccerComputerFinisher.h"
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
	m_pMoveControllClass->GetParams(cParam, s->m_PlayerInfo.strong_type);
	Dashpro = 200 - (int)(cParam.ActionFrequence * 100);
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
		CharacterComputerMove::Param cParam;
		CharacterComputerMove*  m_pMoveControllClass;
		int AttackPoint;
	public:
		SoccerDoEvent(SoccerPlayer* cSoccer) :
			m_cSoccer(cSoccer) 
		{
			AttackPoint = rand() % 100;
			m_pMoveControllClass->GetParams(cParam, m_cSoccer->m_PlayerInfo.strong_type);
		}

		//アニメーションの更新
		void Attack(float len)override
		{
			//if (m_cSoccer->m_Params.SP >= 0.6f)
			if (chr_func::isCanSpecialAttack(m_cSoccer))
			{
				//m_cSoccer->SetState(new SoccerState_ComputerControll_Finisher());
			}
			if (len < 6.0f)
			{
				if ((cParam.ActionFrequence * 100) > AttackPoint)
				{
					m_cSoccer->SetState(new SoccerState_ComputerControll_Attack(m_cSoccer));
				}
			}
			else if (len < 20.0f)
			{
				if ((cParam.ActionFrequence * 100) > AttackPoint)
				{
					m_cSoccer->SetState(new SoccerState_ComputerControll_Shot);
				}
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
		CharacterComputerMove::Param cParam;
		CharacterComputerMove*  m_pMoveControllClass;
		int ReactionPoint;
	public:
		SoccerReactionEvent(SoccerPlayer* cSoccer) :
			m_cSoccer(cSoccer)
		{
			m_pMoveControllClass->GetParams(cParam, m_cSoccer->m_PlayerInfo.strong_type);
			ReactionPoint = rand() % 100;
		
		}

		//アニメーションの更新
		void Reaction(CharacterComputerReactionHitEvent::HitType hittype, Vector3 vec)override
		{
			if (hittype == CharacterComputerReactionHitEvent::HitType::CanCounter)
			{
				if ((cParam.BallCounter * 100) > ReactionPoint)
				{
					m_cSoccer->SetState(new SoccerState_PlayerControll_Counter);
				}
				
				
			}
			else
			{
				if ((cParam.BallCounter * 100) > ReactionPoint)
				{
					m_cSoccer->SetState(
						new SoccerState_ComputerControll_Rolling
						(new SocceComputerrUtillityClass::ComputerRollingControll(m_cSoccer, vec),
							false));
				}
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
		m_pDoActionClass->Update();
		m_pReactionClass->Update();
		if (rand() %  Dashpro == 0)
		{
			s->SetState(new SoccerState_ComputerControll_Dash(s));
		}
	}
	m_pMoveClass->Update();
	
	
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

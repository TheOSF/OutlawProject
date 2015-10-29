#include "../Computer/SoccerComputerMove.h"
#include "../../CharacterFunction.h"
#include "../../CharacterCounterClass.h"
#include "../../CharacterMoveClass.h"
#include "SoccerComputerAttack.h"
#include "SoccerComputerShot.h"
#include "../../CharacterManager.h"
#include "../../../Camera/Camera.h"
#include "../SoccerHitEvent.h"

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
	CharacterUsualMove::Params p;

	p.Acceleration = 0.2f;
	p.MaxSpeed = 0.2f;
	p.TurnSpeed = 0.3f;
	p.DownSpeed = 0.2f;

	//移動コントロールクラスの作成
	m_pMoveControllClass = new CharacterComputerMove(s);

	//移動クラスの作成
	m_pMoveClass = new CharacterUsualMove(
		s,
		p,
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
			
			if (len < 5.0f)
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
	m_pDoActionClass = new CharacterComputerDoAction(s, cParam,new SoccerDoEvent(s), new SoccerHitEvent(s));

	
}


void SoccerState_ComputerControll_Move::Execute(SoccerPlayer* s)
{
	//スティック値をセット
	m_pMoveClass->SetStickValue(m_pMoveControllClass->SwitchAction(s));

	//更新
	m_pMoveClass->Update();
	m_pDoActionClass->Update();

	//モデルのワールド変換行列を更新
	chr_func::CreateTransMatrix(s, s->m_ModelSize, &s->m_Renderer.m_TransMatrix);

}

void SoccerState_ComputerControll_Move::Exit(SoccerPlayer* t)
{
	delete m_pMoveClass;
	delete m_pMoveControllClass; 
	delete m_pDoActionClass;
}

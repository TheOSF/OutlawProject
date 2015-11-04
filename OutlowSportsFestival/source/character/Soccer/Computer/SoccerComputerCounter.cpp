#include "SoccerComputerCounter.h"
#include "../../CharacterFunction.h"
#include "SoccerComputerMove.h"
#include "../SoccerPlayerState.h"
#include "../SoccerHitEvent.h"

//-------------カウンターステートクラス-------------
void SoccerState_ComputerControll_Counter::Enter(SoccerPlayer* s)
{
	//カウンターイベントクラス
	class CounterEvent :public CharacterDefaultCounter::Event
	{
		SoccerPlayer* const m_pSoccer;
	public:
		CounterEvent(SoccerPlayer* pTennis) :
			m_pSoccer(pTennis)
		{

		}

		//構え開始
		void Pose()
		{
			m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_Counter);
		}

		//ボールへ移動開始
		void Move(BallBase* pCounterBall)
		{
			//ボールの位置によってモーション分岐
			if (chr_func::isRight(m_pSoccer, pCounterBall->m_Params.pos))
			{
				m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_Shot);
			}
			else
			{
				m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_Shot);
			}
		}

		//打つ
		void Shot(BallBase* pCounterBall)
		{
			chr_func::ResetMove(m_pSoccer);
			chr_func::AddMoveFront(m_pSoccer, -0.15f, 1000);

		}

		//打ち失敗
		void ShotFaild()
		{
			m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_Crap);
		}

		//終了
		void End()
		{
			//通常移動クラスへ
			m_pSoccer->SetState(SoccerState_PlayerControll_Move::GetPlayerControllMove(m_pSoccer));
		}
	};

	//カウンターパラメータ設定
	CharacterDefaultCounter::Param param;

	param.AfterShotFrame = 15;
	param.CanCounterFrame = 30;
	param.CatchAriaSize = 8;
	param.ControllRadian = D3DXToRadian(33);
	param.FailedFrame = 20;
	param.PoseFrame = 60;
	param.ShotFrame = 6;
	param.BallSpeed = 0.85f;

	// カウンタークラス作成
	m_pCounter = new CharacterDefaultCounter(
		s,
		param,
		new CounterEvent(s),
		new SoccerHitEvent(s)
		);
}
void SoccerState_ComputerControll_Counter::Execute(SoccerPlayer* s)
{
	m_pCounter->SetStickValue(controller::GetStickValue(controller::stick::left, s->m_PlayerInfo.number));
	//更新
	m_pCounter->Update();

	//モデルのワールド変換行列を更新
	chr_func::CreateTransMatrix(s, 0.05f, &s->m_Renderer.m_TransMatrix);

	//モデル更新
	s->m_Renderer.Update(1);
}
void SoccerState_ComputerControll_Counter::Exit(SoccerPlayer* s)
{
	delete m_pCounter;
}
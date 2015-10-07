#include "BaseballPlayerState_Counter.h"
#include "BaseballPlayerState.h"
#include "../../GameSystem/GameController.h"
#include "../CharacterFunction.h"
#include "../../Ball/UsualBall.h"
#include "../../Damage/Damage.h"
#include "Baseball_HitEvent.h"

#include "../CharacterCounterClass.h"
#include "../CharacterMoveClass.h"
#include "../CharacterDefaultCounterClass.h"


//***************************************************
//		プレイヤー操作の カウンタークラス
//***************************************************

BaseballState_PlayerControll_Counter::BaseballState_PlayerControll_Counter(int size)
{
	areasize = size;
}

// ステート開始
void BaseballState_PlayerControll_Counter::Enter(BaseballPlayer* b)
{
	//カウンターイベントクラス
	class CounterEvent :public CharacterDefaultCounter::Event
	{
		BaseballPlayer* const m_pBaseball;
	public:
		CounterEvent(BaseballPlayer* pBaseball) :
			m_pBaseball(pBaseball)
		{

		}

		//構え開始
		void Pose()
		{
			m_pBaseball->m_Renderer.SetMotion(baseball_player::_mb_CounterPose);
		}

		//ボールへ移動開始
		void Move(BallBase* pCounterBall)
		{
			//ボールの位置によってモーション分岐
			if (chr_func::isRight(m_pBaseball, pCounterBall->m_Params.pos))
			{
				m_pBaseball->m_Renderer.SetMotion(baseball_player::_mb_CounterRight);
			}
			else
			{
				m_pBaseball->m_Renderer.SetMotion(baseball_player::_mb_CounterLeft);
			}
		}

		//打つ
		void Shot(BallBase* pCounterBall)
		{
			chr_func::ResetMove(m_pBaseball);
			chr_func::AddMoveFront(m_pBaseball, -0.15f, 1000);
		}

		//打ち失敗
		void ShotFaild()
		{
			m_pBaseball->m_Renderer.SetMotion(baseball_player::_mb_LosePose);
		}

		//終了
		void End()
		{
			//通常移動クラスへ
			m_pBaseball->SetState(BaseballState_PlayerControll_Move::GetPlayerControllMove(m_pBaseball));
		}
	};

	//カウンターパラメータ設定
	CharacterDefaultCounter::Param param;

	param.AfterShotFrame = 15;
	param.CanCounterFrame = 30;
	param.CatchAriaSize = areasize;
	param.ControllRadian = D3DXToRadian(33);
	param.FailedFrame = 20;
	param.PoseFrame = 10;
	param.ShotFrame = 6;
	param.BallSpeed = 0.85f;

	// カウンタークラス作成
	m_pCounter = new CharacterDefaultCounter(
		b,
		param,
		new CounterEvent(b),
		new BaseballHitEvent(b)
		);
}


// ステート実行
void BaseballState_PlayerControll_Counter::Execute(BaseballPlayer* b)
{
	m_pCounter->SetStickValue(controller::GetStickValue(controller::stick::left, b->m_PlayerInfo.number));
	//更新
	m_pCounter->Update();

	//モデルのワールド変換行列を更新
	chr_func::CreateTransMatrix(b, 0.05f, &b->m_Renderer.m_TransMatrix);

	//モデル更新
	b->m_Renderer.Update(1);
}


// ステート終了
void BaseballState_PlayerControll_Counter::Exit(BaseballPlayer* b)
{
	delete m_pCounter;
}

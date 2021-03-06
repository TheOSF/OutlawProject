#include "BaseballPlayerState_Counter.h"
#include "BaseballPlayerState.h"
#include "../../GameSystem/GameController.h"
#include "../CharacterFunction.h"
#include "../../Ball/UsualBall.h"
#include "../../Damage/Damage.h"
#include "Baseball_HitEvent.h"

#include "../CharacterMoveClass.h"
#include "../CharacterDefaultCounterClass.h"


//***************************************************
//		プレイヤー操作の カウンタークラス
//***************************************************

BaseballState_PlayerControll_Counter::BaseballState_PlayerControll_Counter()
{

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
			if (m_pBaseball->getBatterFlg())
			{
				m_pBaseball->SetMotion(baseball_player::_mb_CounterPose_B);
			}
			else
			{
                m_pBaseball->SetMotion(baseball_player::_mb_CounterPose_P);
			}
		}

		//ボールへ移動開始
		void Move(BallBase* pCounterBall)
		{
			if (m_pBaseball->getBatterFlg())
			{
                m_pBaseball->SetMotion(baseball_player::_mb_CounterRight_B);
			}
			else
			{
				//ボールの位置によってモーション分岐
				if (chr_func::isRight(m_pBaseball, pCounterBall->m_Params.pos))
				{
                    m_pBaseball->SetMotion(baseball_player::_mb_CounterRight_P);
				}
				else
				{
                    m_pBaseball->SetMotion(baseball_player::_mb_CounterLeft_P);
				}
			}
		}

		//打つ
		void Shot(BallBase* pCounterBall)
		{

		}

        Vector3 ClacLocalOffset(bool Right)
        {
            if (m_pBaseball->isBatter())
            {
                return Vector3(1.5f, 0, 0);
            }

            return Vector3Zero;
        }

		//打ち失敗
		void ShotFaild()
		{
			if (m_pBaseball->getBatterFlg())
			{
                m_pBaseball->SetMotion(baseball_player::_mb_Stand_B);
			}
			else
			{
                m_pBaseball->SetMotion(baseball_player::_mb_Stand_P);
			}
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

    if (b->getBatterFlg())
    {
        param.HitSE = Sound::BaseBall_kaki_n;

        param.AfterShotFrame = 10;
        param.CanCounterFrame = 15;
        param.CatchAriaSize = 8.0f;
        param.ControllRadian = D3DXToRadian(33);
        param.FailedFrame = 20;
        param.PoseFrame = 24;
        param.ShotFrame = 15;
        param.BallSpeed = 1.0f;
        param.HitStop = 3;
   
    }
    else
    {
        param.AfterShotFrame = 15;
        param.CanCounterFrame = 15;
        param.CatchAriaSize = 12.0f;
        param.ControllRadian = D3DXToRadian(33);
        param.FailedFrame = 10;
        param.PoseFrame = 24;
        param.ShotFrame = 7;
        param.BallSpeed = 0.85f;
        param.CatchFrame = 10;
        param.CatchBoneNumber = 32;
    }

	// カウンタークラス作成
	m_pCounter = new CharacterDefaultCounter(
		b,
		param,
		new CounterEvent(b),
		new BaseballHitEvent(b)
		);

    m_pCounter->SetCatchModel(b->getNowModeModel());
}


// ステート実行
void BaseballState_PlayerControll_Counter::Execute(BaseballPlayer* b)
{
	//更新
	m_pCounter->Update();

	//モデルのワールド変換行列を更新
	chr_func::CreateTransMatrix(b, &b->getNowModeModel()->m_TransMatrix);

	//モデル更新
	b->ModelUpdate(1.0f);
}


// ステート終了
void BaseballState_PlayerControll_Counter::Exit(BaseballPlayer* b)
{
	delete m_pCounter;
}

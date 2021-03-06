#include "TennisPlayerState_Counter.h"
#include "TennisPlayerState_UsualMove.h"
#include "../../GameSystem/GameController.h"
#include "../CharacterFunction.h"
#include "../../Ball/UsualBall.h"
#include "../../Damage/Damage.h"
#include "Tennis_HitEvent.h"

#include "../CharacterMoveClass.h"
#include "../CharacterDefaultCounterClass.h"


//***************************************************
//		プレイヤー操作の カウンタークラス
//***************************************************

// ステート開始
void TennisState_Counter::Enter(TennisPlayer* t)
{
    //カウンターイベントクラス
    class CounterEvent :public CharacterDefaultCounter::Event
    {
        TennisPlayer* const m_pTennis;
        bool MotionRight;
    public:
        CounterEvent(TennisPlayer* pTennis) :
            m_pTennis(pTennis),
            MotionRight(false)
        {

        }

        //構え開始
        void Pose()
        {
            m_pTennis->m_Renderer.SetMotion(TennisPlayer::_mt_CounterPose);
        }

        //ボールへ移動開始
        void Move(BallBase* pCounterBall)
        {
            //ボールの位置によってモーション分岐
            if (MotionRight)
            {
                m_pTennis->m_Renderer.SetMotion(TennisPlayer::_mt_CounterRight);
            }
            else
            {
                m_pTennis->m_Renderer.SetMotion(TennisPlayer::_mt_CounterLeft);
            }
        }

        void ModelUpdate(float t)
        {
            m_pTennis->m_Renderer.Update(t);
        }

        //打つ
        void Shot(BallBase* pCounterBall)
        {
            
        }

        //打ち失敗
        void ShotFaild()
        {
            m_pTennis->m_Renderer.SetMotion(TennisPlayer::_mt_LosePose);
        }

        //終了
        void End()
        {
            //通常移動クラスへ
            m_pTennis->SetState(TennisState_PlayerControll_Move::GetPlayerControllMove(m_pTennis));
        }

        Vector3 ClacLocalOffset(bool Right)
        {
            Vector3 ret(0, 0, 0);
            MotionRight = Right;
            //ボールの位置によってモーション分岐
            if (Right)
            {
                ret = Vector3(1.5f, 0, 0);
            }
            else
            {
                ret = Vector3(-2.0f, 0, 0);
            }
            return ret;
        }
    };

    //カウンターパラメータ設定
    CharacterDefaultCounter::Param param;

    param.AfterShotFrame = 13;
    param.CanCounterFrame = 30;
    param.CatchAriaSize = 8;
    param.ControllRadian = D3DXToRadian(33);
    param.FailedFrame = 20;
    param.PoseFrame = 24;
    param.ShotFrame = 8;
    param.BallSpeed = 0.85f;

	// カウンタークラス作成
    m_pCounter = new CharacterDefaultCounter(
        t,
        param,
        new CounterEvent(t),
        new TennisHitEvent(t)
        );
}


// ステート実行
void TennisState_Counter::Execute(TennisPlayer* t)
{
    
    m_pCounter->SetStickValue(controller::GetStickValue(controller::stick::left, t->m_PlayerInfo.number));
	//更新
	m_pCounter->Update();

	//モデルのワールド変換行列を更新
	chr_func::CreateTransMatrix(t, &t->m_Renderer.m_TransMatrix);

    //モデル更新
   // t->m_Renderer.Update(1);
}


// ステート終了
void TennisState_Counter::Exit(TennisPlayer* t)
{
	delete m_pCounter;
}

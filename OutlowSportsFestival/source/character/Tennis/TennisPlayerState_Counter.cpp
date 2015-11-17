#include "TennisPlayerState_Counter.h"
#include "TennisPlayerState_UsualMove.h"
#include "../../GameSystem/GameController.h"
#include "../CharacterFunction.h"
#include "../../Ball/UsualBall.h"
#include "../../Damage/Damage.h"
#include "Tennis_HitEvent.h"

#include "../CharacterCounterClass.h"
#include "../CharacterMoveClass.h"
#include "../CharacterDefaultCounterClass.h"


//***************************************************
//		プレイヤー操作の カウンタークラス
//***************************************************

// ステート開始
void TennisState_PlayerControll_Counter::Enter(TennisPlayer* t)
{
    //カウンターイベントクラス
    class CounterEvent :public CharacterDefaultCounter::Event
    {
        TennisPlayer* const m_pTennis;
    public:
        CounterEvent(TennisPlayer* pTennis):
            m_pTennis(pTennis)
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
            if (chr_func::isRight(m_pTennis, pCounterBall->m_Params.pos))
            {
                m_pTennis->m_Renderer.SetMotion(TennisPlayer::_mt_CounterRight);
            }
            else
            {
                m_pTennis->m_Renderer.SetMotion(TennisPlayer::_mt_CounterLeft);
            }
        }

        //打つ
        void Shot(BallBase* pCounterBall)
        {
            chr_func::ResetMove(m_pTennis);
            chr_func::AddMoveFront(m_pTennis, -0.15f, 1000);
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
void TennisState_PlayerControll_Counter::Execute(TennisPlayer* t)
{
    
    m_pCounter->SetStickValue(controller::GetStickValue(controller::stick::left, t->m_PlayerInfo.number));
	//更新
	m_pCounter->Update();

	//モデルのワールド変換行列を更新
	chr_func::CreateTransMatrix(t, t->m_ModelSize, &t->m_Renderer.m_TransMatrix);

    //モデル更新
    t->m_Renderer.Update(1);
}


// ステート終了
void TennisState_PlayerControll_Counter::Exit(TennisPlayer* t)
{
	delete m_pCounter;
}

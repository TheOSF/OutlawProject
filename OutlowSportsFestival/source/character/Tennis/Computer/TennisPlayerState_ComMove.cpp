#include "TennisPlayerState_ComMove.h"
#include "../TennisState_Shot.h"
#include "../TennisPlayerState_Counter.h"
#include "../../../GameSystem/GameController.h"
#include "../../CharacterFunction.h"
#include "../../../Ball/UsualBall.h"
#include "../../../Damage/Damage.h"
#include "../../CharacterCounterClass.h"
#include "../../CharacterMoveClass.h"
#include "../../CharacterShotAttackClass.h"
#include "../../CharacterManager.h"
#include "../Tennis_HitEvent.h"
#include "../TennisPlayerState_Attack.h"


//ステート開始
void TennisState_ComputerControll_Move::Enter(TennisPlayer* t)
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
}


void TennisState_ComputerControll_Move::Execute(TennisPlayer* t)
{
    {
        //スティックの値としてコンピュータ操作の移動を計算する
        const Vector3 Target = DefCharacterMgr.GetRoundStartPos(t->m_PlayerInfo.number);
        Vector3 v = Target - t->m_Params.pos;

        if (v.Length() < 1.0f)
        {
            v = Vector3Zero;
        }

        //スティックの値セット
        m_pMoveClass->SetStickValue(Vector2Normalize(Vector2(v.x, v.z)));

    }
    //更新
    m_pMoveClass->Update();

    //モデルのワールド変換行列を更新
    chr_func::CreateTransMatrix(t, t->m_ModelSize, &t->m_Renderer.m_TransMatrix);

}

void TennisState_ComputerControll_Move::Exit(TennisPlayer* t)
{
    delete m_pMoveClass;
}



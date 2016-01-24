#include "TennisPlayerState_ComMove.h"
#include "../TennisPlayerState_Counter.h"
#include "../../../GameSystem/GameController.h"
#include "../../CharacterFunction.h"
#include "../../../Ball/UsualBall.h"
#include "../../../Damage/Damage.h"
#include "../../CharacterMoveClass.h"
#include "../../CharacterShotAttackClass.h"
#include "../../CharacterManager.h"
#include "../Tennis_HitEvent.h"
#include "../TennisPlayerState_Attack.h"
#include "../TennisUtillityClass.h"
#include "TennisComputerUtillityClass.h"
#include "TennisComputerReactionEvent.h"
#include "../TennisPlayerState_UsualMove.h"

//ステート開始
void TennisState_ComputerControll_Move::Enter(TennisPlayer* t)
{
    {
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
            new TennisUtillityClass::TennisMoveEvent(t),
            new TennisHitEvent(t)
            );
    }

    //移動ＡＩクラスの生成
    {
        CharacterComputerMove::InParam param;

        param.m_BestLenList.push_back({ 40.0f, 0.6f });
        param.m_BestLenList.push_back({ 20.0f, 0.3f });
        param.m_BestLenList.push_back({ 5.0f, 0.1f });

        m_pCharacterComputerMove = new CharacterComputerMove(
            t,
            param
            );
    }

    //行動ＡＩクラスの生成
    {
        m_pCharacterComputerDoAction = new CharacterComputerDoAction(
            t,
            m_pCharacterComputerMove,
            new TennisComputerUtillityClass::AttackEvent(t)
            );
    }

    //反応ＡＩクラスの生成
    {
        m_pCharacterComputerReaction = new CharacterComputerReaction(
            t,
            TennisComputerReactionEvent::GetCharacterComputerReactionInParam(),
            new TennisComputerReactionEvent(t)
            );
    }
}


void TennisState_ComputerControll_Move::Execute(TennisPlayer* t)
{
    if (TennisState_PlayerControll_Move::SwitchGameState(t) == false)
    {
        //ＡＩ更新
        m_pCharacterComputerMove->Update();

        m_pCharacterComputerDoAction->Update();

        m_pCharacterComputerReaction->Update();

        //スティック値をセット
        m_pMoveClass->SetStickValue(m_pCharacterComputerMove->GetMoveVec());

    }
    else
    {
        //スティックの値セット（移動しない）
        m_pMoveClass->SetStickValue(Vector2(0, 0));
    }
    
    //更新
    m_pMoveClass->Update();

    //モデルのワールド変換行列を更新
    chr_func::CreateTransMatrix(t, &t->m_Renderer.m_TransMatrix);

}

void TennisState_ComputerControll_Move::Exit(TennisPlayer* t)
{
    delete m_pMoveClass;
    delete m_pCharacterComputerMove;
    delete m_pCharacterComputerDoAction;
    delete m_pCharacterComputerReaction;
}


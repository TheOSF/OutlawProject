#include "AmefootComputerState_UsualMove.h"
#include "AmefootPlayerState_UsualMove.h"
#include "../AmefootPlayerState.h"
#include "../../../Damage/Damage.h"
#include "../../CharacterMoveClass.h"
#include "../../../GameSystem/GameController.h"
#include "../../../Camera/Camera.h"
#include "../../CharacterFunction.h"
#include "../../CharacterHitEventFunc.h"
#include "../AmefootUsualHitEvent.h"
#include "../AmefootPlayerState.h"
#include "../UtilityClass/AmefootComputerUtillityClass.h"

//-----------------------------------------------------------------------------------------
// AmefootComputerState_UsualMove
// [ アメフト ] 通常移動するステートクラス
//-----------------------------------------------------------------------------------------
AmefootComputerState_UsualMove::AmefootComputerState_UsualMove()
{};
//-----------------------------------------------------------------------------------------
void AmefootComputerState_UsualMove::Enter(AmefootPlayer* pCharacter)
{
    {
        CharacterUsualMove::Params params;
        params.Acceleration = 0.085f;
        params.DownSpeed = 0.20f;
        params.MaxSpeed = 0.18f;
        params.TurnSpeed = 0.285f;

        m_pMoveClass = new CharacterUsualMove(
            pCharacter,
            params,
            new AmefootUsualMove_MoveEvent(pCharacter),
            new AmefootUsualHitEvent(pCharacter)
            );
    }

    //移動ＡＩクラスの生成
    {
        CharacterComputerMove::InParam param;

        param.m_BestLenList.push_back({ 5.0f, 0.7f });
        param.m_BestLenList.push_back({ 20.0f, 0.3f });

        m_pCharacterComputerMove = new CharacterComputerMove(
            pCharacter,
            param
            );
    }

    //行動ＡＩクラスの生成
    {
        m_pCharacterComputerDoAction = new CharacterComputerDoAction(
            pCharacter,
            m_pCharacterComputerMove,
            new AmefootComputerUtillityClass::AttackEvent(pCharacter)
            );
    }

    //反応ＡＩクラスの生成
    {
        m_pCharacterComputerReaction = new CharacterComputerReaction(
            pCharacter,
            AmefootComputerUtillityClass::ReactionEvent::GetInParam(),
            new AmefootComputerUtillityClass::ReactionEvent(pCharacter)
            );
    }
}
//-----------------------------------------------------------------------------------------
void AmefootComputerState_UsualMove::Execute(AmefootPlayer* pCharacter)
{
    if (AmefootPlayerState_UsualMove::SwitchGameState(pCharacter) == false)
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
    chr_func::CreateTransMatrix(pCharacter, &pCharacter->m_Renderer.m_TransMatrix);

}
//-----------------------------------------------------------------------------------------
void AmefootComputerState_UsualMove::Exit(AmefootPlayer* pCharacter)
{
    delete m_pMoveClass;
    delete m_pCharacterComputerMove;
    delete m_pCharacterComputerDoAction;
    delete m_pCharacterComputerReaction;
}


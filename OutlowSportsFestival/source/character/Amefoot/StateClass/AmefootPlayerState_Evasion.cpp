#include "AmefootPlayerState_Evasion.h"
#include "../MoveClass/AmefootEvasionStandUpClass.h"
#include "../../CharacterEvasionClass.h"
#include "../AmefootUsualHitEvent.h"
#include "../../../GameSystem/GameController.h"
#include "../../CharacterFunction.h"
#include "../AmefootPlayerState.h"

//-----------------------------------------------------------------------------------------
// AmefootPlayerState_Evasion
// [ アメフト ] 回避行動をするステートクラス
//-----------------------------------------------------------------------------------------
AmefootPlayerState_Evasion::AmefootPlayerState_Evasion()
{
    //初期ステートset
    SetState(&AmefootPlayerState_Evasion::State_DownStart);
}

//-----------------------------------------------------------------------------------------
void AmefootPlayerState_Evasion::Enter(AmefootPlayer* pCharacter)
{
    m_pChr = pCharacter;
}
//-----------------------------------------------------------------------------------------
void AmefootPlayerState_Evasion::Execute(AmefootPlayer* pCharacter)
{
    chr_func::XZMoveDown(m_pChr, 0.05f);

    (this->*m_pStateFunc)();

    chr_func::CreateTransMatrix(m_pChr, &m_pChr->m_Renderer.m_TransMatrix);
}
//-----------------------------------------------------------------------------------------
void AmefootPlayerState_Evasion::Exit(AmefootPlayer* pCharacter)
{

}
//-----------------------------------------------------------------------------------------

bool AmefootPlayerState_Evasion::isStandUp()
{
    
    if (m_pChr->m_PlayerInfo.player_type == PlayerType::_Player)
    {
        //プレイヤの場合ボタンで
        return !controller::GetPush(controller::button::batu, m_pChr->m_PlayerInfo.number);
    }
    else
    {
        //コンピュータの場合乱数で
        RATIO DoStandUp = 0.0f;

        //コンピュータの場合適当で
        switch (m_pChr->m_PlayerInfo.strong_type)
        {
        case StrongType::_Weak:
            DoStandUp = 0.01f;
            break;

        case StrongType::_Usual:
            DoStandUp = 0.02f;
            break;

        case StrongType::_Strong:
            DoStandUp = 0.03f;
            break;

        default:
            MyAssert(false, "難易度設定がされていないコンピュータが使われています");
            break;
        }
        
        return frand() <= DoStandUp;
    }

    return false;
}

void AmefootPlayerState_Evasion::SetState( StateFunc  pNewState)
{
    m_Timer = 0;
    m_pStateFunc = pNewState;
}


//-----------------------------------------------------------------------------------------
//                              StateFunc↓
//-----------------------------------------------------------------------------------------
void AmefootPlayerState_Evasion::State_DownStart()
{
    const int DownFrame = 10;

    //モーションセット
    if (m_Timer++ == 0)
    {
        m_pChr->m_Renderer.SetMotion(AmefootPlayer::Motion_Evasion_Start);
    }

    if (m_Timer > DownFrame)
    {
        //次ステートをセット
        SetState(&AmefootPlayerState_Evasion::State_Downing);
    }

    //アニメーション更新
    m_pChr->m_Renderer.Update(1);

    //更新
    chr_func::UpdateAll(m_pChr, &DamageManager::HitEventBase());
}



void AmefootPlayerState_Evasion::State_Downing()
{
    const int CanDownFrame = 70;

    //寝すぎの場合
    if (++m_Timer > CanDownFrame)
    {
        //次ステートをセット
        SetState(&AmefootPlayerState_Evasion::State_ViewAround);
    }

    //途中でやめた場合
    if (isStandUp())
    {
        //次ステートをセット
        SetState(&AmefootPlayerState_Evasion::State_StandUp);
    }

    //アニメーション更新
    m_pChr->m_Renderer.Update(1);

    //更新
    chr_func::UpdateAll(m_pChr, &DamageManager::HitEventBase());
}

void AmefootPlayerState_Evasion::State_StandUp()
{
    const int DownFrame = 30;

    //モーションセット
    if (m_Timer++ == 0)
    {
        m_pChr->m_Renderer.SetMotion(AmefootPlayer::Motion_Evasion_Standup);
    }

    if (m_Timer > DownFrame)
    {
        //次ステートをセット
        m_pChr->SetState(AmefootPlayerState_UsualMove::GetPlayerControllMove(m_pChr));
    }

    //アニメーション更新
    m_pChr->m_Renderer.Update(1);

    //更新
    chr_func::UpdateAll(m_pChr, &AmefootUsualHitEvent(m_pChr));
}

void AmefootPlayerState_Evasion::State_ViewAround()
{
    const int DownFrame = 28;

    //モーションセット
    if (m_Timer++ == 0)
    {
        m_pChr->m_Renderer.SetMotion(AmefootPlayer::Motion_Evasion_ViewAround);
    }

    if (m_Timer > DownFrame)
    {
        //次ステートをセット
        SetState(&AmefootPlayerState_Evasion::State_StandUp);
    }

    //アニメーション更新
    m_pChr->m_Renderer.Update(2);

    //更新
    chr_func::UpdateAll(m_pChr, &DamageManager::HitEventBase());
}
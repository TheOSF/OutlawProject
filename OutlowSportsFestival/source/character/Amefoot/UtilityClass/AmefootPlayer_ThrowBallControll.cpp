#include "AmefootPlayer_ThrowBallControll.h"
#include "../AmefootPlayerState.h"
#include "../../../GameSystem/GameController.h"
#include "../../../Camera/Camera.h"


AmefootPlayer_ThrowBallControll::AmefootPlayer_ThrowBallControll(AmefootPlayer* pAmefootPlayer) :
    m_pAmefootPlayer(pAmefootPlayer)
{}


Vector3 AmefootPlayer_ThrowBallControll::GetVec()
{
    Vector2 stick = controller::GetStickValue(controller::stick::left, m_pAmefootPlayer->m_PlayerInfo.number);
    Vector3 vec(stick.x, 0, stick.y);

    if ( vec.Length() < 0.25f )
    {
        return Vector3Zero;
    }

    vec = DefCamera.GetRight()*vec.x + DefCamera.GetForward()*vec.z;
    vec.Normalize();

    return vec;
}


bool AmefootPlayer_ThrowBallControll::DoOtherAction()
{
    //if ( controller::GetTRG(controller::button::shikaku, m_pAmefootPlayer->m_PlayerInfo.number) )
    //{// [ ] ‚Å [‹ß‹——£UŒ‚]
    //    m_pAmefootPlayer->SetState(new TennisState_PlayerControll_Attack(m_pTennis, true));
    //    return true;
    //}

    if ( controller::GetTRG(controller::button::batu, m_pAmefootPlayer->m_PlayerInfo.number) )
    {// [~] ‚Å [‰ñ”ð]
        m_pAmefootPlayer->SetState(new AmefootPlayerState_Evasion());
        return true;
    }

    if ( controller::GetTRG(controller::button::_R1, m_pAmefootPlayer->m_PlayerInfo.number) )
    {// [R1] ‚Å [ƒJƒEƒ“ƒ^[]
        m_pAmefootPlayer->SetState(new AmefootPlayerState_Counter());
        return true;
    }

    return false;
}


bool AmefootPlayer_ThrowBallControll::DoShotAfterAction()
{
    //if ( controller::GetTRG(controller::button::shikaku, m_pAmefootPlayer->m_PlayerInfo.number) )
    //{// [ ] ‚Å [‹ß‹——£UŒ‚]
    //    m_pAmefootPlayer->SetState(new TennisState_PlayerControll_Attack(m_pAmefootPlayer, true));
    //    return true;
    //}

    return false;
}




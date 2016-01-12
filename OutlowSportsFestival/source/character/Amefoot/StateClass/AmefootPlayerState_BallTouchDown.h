
#pragma once

#include "../AmefootPlayer.h"

//-----------------------------------------------------------------------------------------
// AmefootPlayerState_Counter
// [ アメフト ] 特殊カウンターをするステートクラス
//-----------------------------------------------------------------------------------------
class AmefootPlayerState_BallTouchDown : public AmefootState
{
public:
    AmefootPlayerState_BallTouchDown(AmefootPlayer* pChr);

    void Enter(AmefootPlayer* pChr)override;

    void Execute(AmefootPlayer* pChr)override;

    void Exit(AmefootPlayer* pChr)override;

private:
    typedef void(AmefootPlayerState_BallTouchDown::*StateFunc)();

    AmefootPlayer* const    m_pChr;
    int                     m_Timer;

    void State_CounterPose();
    void State_Jump();
    void State_Down();

};



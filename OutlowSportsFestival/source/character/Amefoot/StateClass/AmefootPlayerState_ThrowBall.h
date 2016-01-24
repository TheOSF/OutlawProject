#pragma once


#include "../AmefootPlayer.h"


//-----------------------------------------------------------------------------------------
// AmefootPlayerState_ThrowBall
// [ アメフト ] 遠距離攻撃(ボール投げ)をするステートクラス
//-----------------------------------------------------------------------------------------
class AmefootPlayerState_ThrowBall : public AmefootState
{
public:
    AmefootPlayerState_ThrowBall();


    ~AmefootPlayerState_ThrowBall();


    void Enter(AmefootPlayer* pCharacter)override;

    void Execute(AmefootPlayer* pCharacter)override;

    void Exit(AmefootPlayer* pCharacter)override;

private:
    int m_Timer;
};

//-----------------------------------------------------------------------------------------


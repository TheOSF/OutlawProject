#pragma once


#include "../AmefootPlayer.h"

class AmefootThrowBall;

//-----------------------------------------------------------------------------------------
// AmefootPlayerState_ThrowBall
// [ アメフト ] 遠距離攻撃(ボール投げ)をするステートクラス
//-----------------------------------------------------------------------------------------
class AmefootPlayerState_ThrowBall : public AmefootState
{
public:
    void Enter(AmefootPlayer* pCharacter)override;

    void Execute(AmefootPlayer* pCharacter)override;

    void Exit(AmefootPlayer* pCharacter)override;

private:
    AmefootThrowBall* m_pAmefootThrowBall;
};
//-----------------------------------------------------------------------------------------


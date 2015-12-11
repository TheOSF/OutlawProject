#pragma once


#include "../AmefootPlayer.h"

class AmefootThrowBall;

//-----------------------------------------------------------------------------------------
// AmefootPlayerState_ThrowBall
// [ �A���t�g ] �������U��(�{�[������)������X�e�[�g�N���X
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


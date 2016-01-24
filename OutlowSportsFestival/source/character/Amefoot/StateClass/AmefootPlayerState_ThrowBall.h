#pragma once


#include "../AmefootPlayer.h"


//-----------------------------------------------------------------------------------------
// AmefootPlayerState_ThrowBall
// [ �A���t�g ] �������U��(�{�[������)������X�e�[�g�N���X
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


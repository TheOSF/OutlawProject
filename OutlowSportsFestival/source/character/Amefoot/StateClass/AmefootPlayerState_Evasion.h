#pragma once


#include "../AmefootPlayer.h"

//-----------------------------------------------------------------------------------------
// AmefootPlayerState_Evasion
// [ �A���t�g ] ����s��������X�e�[�g�N���X
//-----------------------------------------------------------------------------------------
class AmefootPlayerState_Evasion : public AmefootState
{
public:
    AmefootPlayerState_Evasion();

     void Enter(AmefootPlayer* pCharacter)override;

     void Execute(AmefootPlayer* pCharacter)override;

     void Exit(AmefootPlayer* pCharacter)override;

private:

    typedef void(AmefootPlayerState_Evasion::*StateFunc)();

    AmefootPlayer*  m_pChr;
    StateFunc       m_pStateFunc;
    int             m_Timer;
    float           m_StandUpSpeed;

    bool isStandUp();
    void SetState(StateFunc pNewState);

    void State_DownStart();
    void State_Downing();
    void State_StandUp();
    void State_ViewAround();
};


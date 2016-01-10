#pragma once

#include "../AmefootPlayer.h"


class AmefootAttackClass;


class AmefootPlayerState_Tackle : public AmefootState
{
public:
    class PlayerControllEvent;

    AmefootPlayerState_Tackle(AmefootPlayer*const pCharacter);

    ~AmefootPlayerState_Tackle();


    void Enter(AmefootPlayer* pCharacter)override;

    void Execute(AmefootPlayer* pCharacter)override;

    void Exit(AmefootPlayer* pCharacter)override;


private:

    AmefootPlayer* const m_pAmefoot;
    AmefootAttackClass* m_pAmefootAttackClass;

};


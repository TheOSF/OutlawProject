#pragma once


#include "../AmefootPlayer.h"

class AmefootPlayerState_ReceiveControllVanish : public AmefootState
{
public:
    AmefootPlayerState_ReceiveControllVanish(AmefootPlayer* pCharacter);

    ~AmefootPlayerState_ReceiveControllVanish();

    void Enter(AmefootPlayer* pCharacter)override;

    void Execute(AmefootPlayer* pCharacter)override;

    void Exit(AmefootPlayer* pCharacter)override;
};





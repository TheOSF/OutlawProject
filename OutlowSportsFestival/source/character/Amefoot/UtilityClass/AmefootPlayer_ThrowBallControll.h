#pragma once


#include "../StateClass/AmefootPlayerState_ThrowBall.h"

class AmefootPlayer_ThrowBallControll : public AmefootPlayerState_ThrowBall::ControllClass
{
    AmefootPlayer* const m_pAmefootPlayer;
public:
    AmefootPlayer_ThrowBallControll(AmefootPlayer* pAmefootPlayer);

    Vector3 GetVec()override;


    bool DoOtherAction()override;


    bool DoShotAfterAction()override;

};




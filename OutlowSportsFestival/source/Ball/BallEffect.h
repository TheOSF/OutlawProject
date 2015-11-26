#pragma once

#include "Ball.h"


//---------------------------------------------------//
//  ボールのエフェクトクラス
//---------------------------------------------------//

class BallEffect
{
public:
    BallEffect();
    ~BallEffect();

    void SetParent(BallBase* pBall);
    void Counter();   //カウンターをしたときのエフェクトをセット
    void Update();

private:
    BallBase*       m_pBall;
    int             m_EffectFrameCount;
};
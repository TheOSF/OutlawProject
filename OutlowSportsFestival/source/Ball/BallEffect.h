#pragma once

#include "Ball.h"


//---------------------------------------------------//
//  �{�[���̃G�t�F�N�g�N���X
//---------------------------------------------------//

class BallEffect
{
public:
    BallEffect();
    ~BallEffect();

    void SetParent(BallBase* pBall);
    void Counter();   //�J�E���^�[�������Ƃ��̃G�t�F�N�g���Z�b�g
    void Update();

private:
    BallBase*       m_pBall;
    int             m_EffectFrameCount;
};
#pragma once

#include "../AmefootPlayer.h"

//------------------------------------------------
//		 �A���t�g�|�[�Y���[�V�����N���X
//------------------------------------------------

class AmefootCommonState_PoseMotion : public AmefootState
{
public:

    AmefootCommonState_PoseMotion(
        AmefootPlayer::MotionType   MotionType,
        RADIAN                      Speed,
        int                         Frame
        );

    ~AmefootCommonState_PoseMotion();

    // �X�e�[�g�J�n
    void Enter(AmefootPlayer* p)override;

    // �X�e�[�g���s
    void Execute(AmefootPlayer* p)override;

    // �X�e�[�g�I��
    void Exit(AmefootPlayer* p)override;

private:
    const AmefootPlayer::MotionType   m_Motion;
    const RADIAN                      m_Speed;
    int                               m_Frame;
};


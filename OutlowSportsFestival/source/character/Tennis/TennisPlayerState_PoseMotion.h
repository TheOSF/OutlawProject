#pragma once

#include "TennisPlayer.h"
#include "../CharacterEvasionClass.h"

//***************************************************
//		�e�j�X�|�[�Y���[�V�����N���X
//***************************************************

class TennisState_PoseMotion : public TennisState
{
public:

    TennisState_PoseMotion(
        TennisPlayer::MotionType    Motion,
        RADIAN                      Speed,
        int                         Frame
        );

    ~TennisState_PoseMotion();

    // �X�e�[�g�J�n
    void Enter(TennisPlayer* t)override;

    // �X�e�[�g���s
    void Execute(TennisPlayer* t)override;

    // �X�e�[�g�I��
    void Exit(TennisPlayer* t)override;

private:
    const TennisPlayer::MotionType    m_Motion;
    const RADIAN                      m_Speed;
    int                               m_Frame;
};


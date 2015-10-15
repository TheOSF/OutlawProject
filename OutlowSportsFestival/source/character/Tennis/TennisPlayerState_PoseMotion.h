#pragma once

#include "TennisPlayer.h"
#include "../CharacterEvasionClass.h"

//***************************************************
//		テニスポーズモーションクラス
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

    // ステート開始
    void Enter(TennisPlayer* t)override;

    // ステート実行
    void Execute(TennisPlayer* t)override;

    // ステート終了
    void Exit(TennisPlayer* t)override;

private:
    const TennisPlayer::MotionType    m_Motion;
    const RADIAN                      m_Speed;
    int                               m_Frame;
};


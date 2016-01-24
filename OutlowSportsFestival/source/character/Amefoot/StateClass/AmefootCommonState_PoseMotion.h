#pragma once

#include "../AmefootPlayer.h"

//------------------------------------------------
//		 アメフトポーズモーションクラス
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

    // ステート開始
    void Enter(AmefootPlayer* p)override;

    // ステート実行
    void Execute(AmefootPlayer* p)override;

    // ステート終了
    void Exit(AmefootPlayer* p)override;

private:
    const AmefootPlayer::MotionType   m_Motion;
    const RADIAN                      m_Speed;
    int                               m_Frame;
};


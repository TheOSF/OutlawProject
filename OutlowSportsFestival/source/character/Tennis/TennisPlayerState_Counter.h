#pragma once

#include "TennisPlayer.h"
#include "../../GameSystem/ForwardDecl.h"

//***************************************************
//		テニスカウンターステートクラス
//***************************************************


class TennisState_Counter : public TennisState
{
public:
    // ステート開始
    void Enter(TennisPlayer* t)override;

    // ステート実行
    void Execute(TennisPlayer* t)override;

    // ステート終了
    void Exit(TennisPlayer* t)override;

private:
    // カウンタークラス作成
    CharacterDefaultCounter* m_pCounter; // カウンタークラス
};

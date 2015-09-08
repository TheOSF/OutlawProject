#pragma once

#include "TennisPlayer.h"
#include "../CharacterEvasionClass.h"

//***************************************************
//		テニスプレイヤー操作回避クラス
//***************************************************

class TennisState_PlayerControll_Rolling : public TennisState
{
public:

    TennisState_PlayerControll_Rolling(CrVector2 vec);
    ~TennisState_PlayerControll_Rolling();

    // ステート開始
    void Enter(TennisPlayer* t)override;

    // ステート実行
    void Execute(TennisPlayer* t)override;

    // ステート終了
    void Exit(TennisPlayer* t)override;

private:


};
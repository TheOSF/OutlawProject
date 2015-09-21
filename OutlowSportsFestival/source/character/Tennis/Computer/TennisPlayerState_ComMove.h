#pragma once

#include "../TennisPlayer.h"
#include "../../../GameSystem/ForwardDecl.h"

//****************************************************
//	テニスプレイヤーの操作クラスヘッダー
//****************************************************


//プレイヤー操作の移動クラス
class TennisState_ComputerControll_Move :public TennisState
{
private:
    CharacterUsualMove*		m_pMoveClass;
public:

    void Enter(TennisPlayer* t)override;
    void Execute(TennisPlayer* t)override;
    void Exit(TennisPlayer* t)override;
};


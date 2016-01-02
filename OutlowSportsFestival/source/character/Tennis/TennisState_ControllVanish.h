#pragma once

#include "TennisPlayer.h"
#include "../CharacterDamageControll.h"

//***************************************************
//		コントロール吹き飛びクラス
//***************************************************
class TennisState_ControllVanish : public TennisState
{
public:
    TennisState_ControllVanish(TennisPlayer*  pTennis);
    ~TennisState_ControllVanish();

    //ゲッタ
    inline CharacterDamageControll*    GetControllClass()
    {
        return m_pMoveClass;
    }

    // ステート開始
    void Enter(TennisPlayer* t)override;

    // ステート実行
    void Execute(TennisPlayer* t)override;

    // ステート終了
    void Exit(TennisPlayer* t)override;

private:
    TennisPlayer*  const        m_pTennis;
    CharacterDamageControll*    m_pMoveClass;
};

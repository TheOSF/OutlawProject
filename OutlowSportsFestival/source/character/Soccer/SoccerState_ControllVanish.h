#pragma once

#include "SoccerPlayer.h"
#include "../CharacterDamageControll.h"

//***************************************************
//		コントロール吹き飛びクラス
//***************************************************
class SoccerState_ControllVanish : public SoccerState
{
public:
    SoccerState_ControllVanish(SoccerPlayer*  pSoccer);
    ~SoccerState_ControllVanish();

    //ゲッタ
    inline CharacterDamageControll*    GetControllClass()
    {
        return m_pMoveClass;
    }

    // ステート開始
    void Enter(SoccerPlayer* p)override;

    // ステート実行
    void Execute(SoccerPlayer* p)override;

    // ステート終了
    void Exit(SoccerPlayer* p)override;

private:
    SoccerPlayer*  const        m_pSoccer;
    CharacterDamageControll*    m_pMoveClass;
};

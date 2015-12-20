#pragma once

#include "../../Computer/CharacterComputerReaction.h"
#include "../TennisPlayer.h"

//-----------------------------------------------------------------------------------
//  テニス、コンピュータ操作のダメージに対する反応イベントクラス
//-----------------------------------------------------------------------------------

//反応イベントクラス
class TennisComputerReactionEvent :public CharacterComputerReaction::ActionEvent
{
    TennisPlayer*                   m_pTennis;
    CharacterComputerMove::Param    m_Param;

    bool isDoAction(RATIO value)
    {
        return value >= frand();
    }

public:

    TennisComputerReactionEvent(TennisPlayer* pTennis) :
        m_pTennis(pTennis)
    {
        //反応パラメータを取得
        CharacterComputerMove::GetParams(m_Param, m_pTennis->m_PlayerInfo.strong_type);
    }

    //ダメージに対して反応する
    void Reaction(CharacterComputerReactionHitEvent::HitType hittype, Vector3 vec)override;
};
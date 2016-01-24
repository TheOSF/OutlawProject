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

    bool isDoAction(RATIO value)
    {
        return value >= frand();
    }

public:

    TennisComputerReactionEvent(TennisPlayer* pTennis) :
        m_pTennis(pTennis)
    {

    }

    static CharacterComputerReaction::InParam GetCharacterComputerReactionInParam();

    //ダメージに対して反応する
    void Reaction(const TypeParam& param, CrVector3 vec)override;
};
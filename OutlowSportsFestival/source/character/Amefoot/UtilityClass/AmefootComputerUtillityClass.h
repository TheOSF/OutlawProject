#pragma once


#include "../AmefootPlayer.h"
#include "../StateClass/AmefootPlayerState_Evasion.h"
#include "../../Computer/CharacterComputerDoAction.h"
#include "../../Computer/CharacterComputerReaction.h"

class AmefootComputerUtillityClass
{
public:

    //攻撃イベント呼び出しクラス
    class AttackEvent :public CharacterComputerDoAction::ActionEvent
    {
    public:
        AttackEvent(AmefootPlayer* pAmefoot);

        bool Action(CharacterBase* pTarget, float len);

    private:
        AmefootPlayer* const m_pAmefoot;
    };


    //反応イベント呼び出しクラス
    class ReactionEvent :public CharacterComputerReaction::ActionEvent
    {
    public:
        ReactionEvent(AmefootPlayer* pAmefoot);

        void Reaction(const TypeParam& param, CrVector3 vec);

        static CharacterComputerReaction::InParam GetInParam();

    private:
        AmefootPlayer* const m_pAmefoot;
    };

};
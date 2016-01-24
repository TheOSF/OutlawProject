#pragma once


#include "../AmefootPlayer.h"
#include "../StateClass/AmefootPlayerState_Evasion.h"
#include "../../Computer/CharacterComputerDoAction.h"
#include "../../Computer/CharacterComputerReaction.h"

class AmefootComputerUtillityClass
{
public:

    //�U���C�x���g�Ăяo���N���X
    class AttackEvent :public CharacterComputerDoAction::ActionEvent
    {
    public:
        AttackEvent(AmefootPlayer* pAmefoot);

        bool Action(CharacterBase* pTarget, float len);

    private:
        AmefootPlayer* const m_pAmefoot;
    };


    //�����C�x���g�Ăяo���N���X
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
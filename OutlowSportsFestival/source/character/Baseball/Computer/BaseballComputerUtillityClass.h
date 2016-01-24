#pragma once

#include "../BaseballPlayer.h"
#include "../../Computer/CharacterComputerDoAction.h"
#include "../../Computer/CharacterComputerReaction.h"

#include "../BaseballState_PlayerControll_ShotAttack_B.h"

class BaseballComputerUtillityClass
{
public:

    // �U���C�x���g�Ăяo���N���X
    class AttackEvent :public CharacterComputerDoAction::ActionEvent
    {
    public:
        AttackEvent(BaseballPlayer* pChr);

        bool Action(CharacterBase* pTarget, float len);

    private:
        BaseballPlayer* const m_pChr;

        bool Action_B(CharacterBase* pTarget, float len);
        bool Action_P(CharacterBase* pTarget, float len);
    };


    //�����C�x���g�Ăяo���N���X
    class ReactionEvent :public CharacterComputerReaction::ActionEvent
    {
    public:
        ReactionEvent(BaseballPlayer* pChr);

        void Reaction(const TypeParam& param, CrVector3 vec);

        static CharacterComputerReaction::InParam GetInParam();

    private:
        BaseballPlayer* const m_pChr;

        void Reaction_B(const TypeParam& param, CrVector3 vec);
        void Reaction_P(const TypeParam& param, CrVector3 vec);
    };

    //�o�b�^�[�������U���̑���N���X
    class ShotControllClass_B :public BaseballState_PlayerControll_ShotAttack_B::ControllClass
    {
        BaseballPlayer* const       m_pBaseball;
        CharacterComputerReaction*  m_pReaction;
    public:
        ShotControllClass_B(BaseballPlayer* pBaseball);
        ~ShotControllClass_B();

        Vector3 GetVec();

        bool DoOtherAction();
        bool DoShotAfterAction();
       
    };
};
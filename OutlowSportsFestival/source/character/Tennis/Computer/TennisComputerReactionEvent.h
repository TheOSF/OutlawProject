#pragma once

#include "../../Computer/CharacterComputerReaction.h"
#include "../TennisPlayer.h"

//-----------------------------------------------------------------------------------
//  �e�j�X�A�R���s���[�^����̃_���[�W�ɑ΂��锽���C�x���g�N���X
//-----------------------------------------------------------------------------------

//�����C�x���g�N���X
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

    //�_���[�W�ɑ΂��Ĕ�������
    void Reaction(const TypeParam& param, CrVector3 vec)override;
};
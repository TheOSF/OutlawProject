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
    CharacterComputerMove::Param    m_Param;

    bool isDoAction(RATIO value)
    {
        return value >= frand();
    }

public:

    TennisComputerReactionEvent(TennisPlayer* pTennis) :
        m_pTennis(pTennis)
    {
        //�����p�����[�^���擾
        CharacterComputerMove::GetParams(m_Param, m_pTennis->m_PlayerInfo.strong_type);
    }

    //�_���[�W�ɑ΂��Ĕ�������
    void Reaction(CharacterComputerReactionHitEvent::HitType hittype, Vector3 vec)override;
};
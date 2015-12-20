#pragma once

#include "TennisPlayer.h"
#include "../../GameSystem/ForwardDecl.h"

//***************************************************
//		�e�j�X�J�E���^�[�X�e�[�g�N���X
//***************************************************


class TennisState_Counter : public TennisState
{
public:
    // �X�e�[�g�J�n
    void Enter(TennisPlayer* t)override;

    // �X�e�[�g���s
    void Execute(TennisPlayer* t)override;

    // �X�e�[�g�I��
    void Exit(TennisPlayer* t)override;

private:
    // �J�E���^�[�N���X�쐬
    CharacterDefaultCounter* m_pCounter; // �J�E���^�[�N���X
};

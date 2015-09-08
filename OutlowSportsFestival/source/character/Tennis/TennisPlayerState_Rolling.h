#pragma once

#include "TennisPlayer.h"
#include "../CharacterEvasionClass.h"

//***************************************************
//		�e�j�X�v���C���[�������N���X
//***************************************************

class TennisState_PlayerControll_Rolling : public TennisState
{
public:

    TennisState_PlayerControll_Rolling(CrVector2 vec);
    ~TennisState_PlayerControll_Rolling();

    // �X�e�[�g�J�n
    void Enter(TennisPlayer* t)override;

    // �X�e�[�g���s
    void Execute(TennisPlayer* t)override;

    // �X�e�[�g�I��
    void Exit(TennisPlayer* t)override;

private:


};
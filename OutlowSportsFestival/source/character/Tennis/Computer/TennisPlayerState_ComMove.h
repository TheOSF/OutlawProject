#pragma once

#include "../TennisPlayer.h"
#include "../../../GameSystem/ForwardDecl.h"

//****************************************************
//	�e�j�X�v���C���[�̑���N���X�w�b�_�[
//****************************************************


//�v���C���[����̈ړ��N���X
class TennisState_ComputerControll_Move :public TennisState
{
private:
    CharacterUsualMove*		m_pMoveClass;
public:

    void Enter(TennisPlayer* t)override;
    void Execute(TennisPlayer* t)override;
    void Exit(TennisPlayer* t)override;
};


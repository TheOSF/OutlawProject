#pragma once

#include "../BaseballPlayer.h"
#include "../../../GameSystem/ForwardDecl.h"
#include "../../../character/Computer/CharacterComputerMove.h"
#include "../../../character/Computer/CharacterComputerDoAction.h"
#include "../../../character/Computer/CharacterComputerReaction.h"

//****************************************************
//	�싅�v���C���[(Com)�̒ʏ�ړ��N���X�w�b�_�[
//****************************************************
//�R���s���[�^�[�̈ړ��N���X
class BaseballPlayerState_ComMove :public BaseballState
{
public:
	//�@�X�e�[�g�J�n
	void Enter(BaseballPlayer* b)override;
	//�@�X�e�[�g���s
	void Execute(BaseballPlayer* b)override;
	//�@�X�e�[�g�I��
    void Exit(BaseballPlayer* b)override;

private:
	CharacterUsualMove*		    m_pMoveClass;
	CharacterComputerMove*      m_pMoveControllClass;
	CharacterComputerDoAction*  m_pDoActionClass;
	CharacterComputerReaction*  m_pReactionClass;
};


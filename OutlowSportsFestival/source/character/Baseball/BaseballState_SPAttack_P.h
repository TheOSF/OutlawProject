#pragma once

#include "BaseballPlayer.h"

//***************************************************
//		�싅(�s�b�`���[)�K�E�Z�X�e�[�g�N���X
//***************************************************
class CharacterShotAttack;
class BaseballState_SPAttack_P : public BaseballState
{
    int m_Timer;

public:

	BaseballState_SPAttack_P();
	
	
	// �X�e�[�g�J�n
	void Enter(BaseballPlayer* b)override;

	// �X�e�[�g���s
	void Execute(BaseballPlayer* b)override;

	// �X�e�[�g�I��
	void Exit(BaseballPlayer* b)override;

	//�@The World
	void FreezeGame(UINT frame); 

};


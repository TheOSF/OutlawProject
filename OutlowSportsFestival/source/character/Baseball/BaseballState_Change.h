#pragma once

#include "BaseballPlayer.h"

class BaseballState_Change : public BaseballState
{
protected:
	bool batterflg;//�@(true:�o�b�^�[,false:����)

public:
	BaseballState_Change();
	~BaseballState_Change();

public:
	// �X�e�[�g�J�n
	void Enter(BaseballPlayer* b)override;

	// �X�e�[�g���s
	void Execute(BaseballPlayer* b)override;

	// �X�e�[�g�I��
	void Exit(BaseballPlayer* b)override;
};
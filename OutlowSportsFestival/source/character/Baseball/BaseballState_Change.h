#pragma once

#include "BaseballPlayer.h"

class BaseballState_Change : public BaseballState
{
private:
	bool batterflg;//�@(true:�o�b�^�[,false:����)
	int m_Timer;
	BaseballPlayer* m_pBaseball;
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

private:
	void(BaseballState_Change::*m_pStateFunc)();

	void State_Change();
	void State_End();

};
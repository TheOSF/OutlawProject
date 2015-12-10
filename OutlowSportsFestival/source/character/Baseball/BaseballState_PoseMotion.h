#pragma once

#include "BaseballPlayer.h"
#include "../CharacterEvasionClass.h"

//***************************************************
//		�싅�|�[�Y���[�V�����N���X
//***************************************************

class BaseballState_PoseMotion : public BaseballState
{
public:

	BaseballState_PoseMotion(
		baseball_player::MotionType    Motion,
		RADIAN                      Speed,
		int                         Frame
		);

	~BaseballState_PoseMotion();

	// �X�e�[�g�J�n
	void Enter(BaseballPlayer* b)override;

	// �X�e�[�g���s
	void Execute(BaseballPlayer* b)override;

	// �X�e�[�g�I��
	void Exit(BaseballPlayer* b)override;

private:
	const  baseball_player::MotionType    m_Motion;
	const RADIAN                      m_Speed;
	int                               m_Frame;
};


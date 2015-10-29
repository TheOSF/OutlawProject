#pragma once
#include "SoccerPlayer.h"
#include "../CharacterEvasionClass.h"

//***************************************************
//		�T�b�J�[�|�[�Y���[�V�����N���X
//***************************************************

class SoccerState_PoseMotion : public SoccerState
{
public:

	SoccerState_PoseMotion(
		SoccerPlayer::MotionType    Motion,
		RADIAN                      Speed,
		int                         Frame
		);

	~SoccerState_PoseMotion();

	// �X�e�[�g�J�n
	void Enter(SoccerPlayer* t)override;

	// �X�e�[�g���s
	void Execute(SoccerPlayer* t)override;

	// �X�e�[�g�I��
	void Exit(SoccerPlayer* t)override;

private:
	const SoccerPlayer::MotionType    m_Motion;
	const RADIAN                      m_Speed;
	int                               m_Frame;
};


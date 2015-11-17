#pragma once

#include "../SoccerPlayer.h"
#include "../../../GameSystem/ForwardDecl.h"
#include "../../../character/Computer/CharacterComputerMove.h"
#include "../../../character/Computer/CharacterComputerDoAction.h"
#include "../../../character/Computer/CharacterComputerReaction.h"
#include "../SoccerDash.h"

//****************************************************
//	�T�b�J�[�v���C���[�̃R���s���[�^�ړ�����N���X�w�b�_�[
//****************************************************
class SoccerDash;

//�R���s���[�^����̈ړ��N���X
class SoccerState_ComputerControll_Dash :public SoccerState
{
public:
	
	SoccerState_ComputerControll_Dash(SoccerPlayer* s);
	void Enter(SoccerPlayer* s)override;
	void Execute(SoccerPlayer* s)override;
	void Exit(SoccerPlayer* s)override;

	Vector2 StateMoveFront(SoccerPlayer* s);//�@�^�[�Q�b�g�ɑO�i
	Vector2 StateMoveDistance(SoccerPlayer* s);//�@�^�[�Q�b�g�Ƃ̋������Ƃ�
	Vector2 StateStop(SoccerPlayer* s);//�@�����~�܂�

	static bool SwitchGameState(SoccerPlayer* ps);
	Vector2 SwitchAction(SoccerPlayer* s);
private:

	//����penum
	enum MoveMode
	{
		Stop,
		Forward,
		Distance,
	};
	CharacterComputerMove::Param m_cParam;
	MoveMode movemode;
	int       m_Count;
	Vector3   m_MoveTargetPos;
	SoccerDash*		m_pDashClass;

	SoccerPlayer* const m_cSoccer;
	int m_timer;
	
};
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

private:
	
};
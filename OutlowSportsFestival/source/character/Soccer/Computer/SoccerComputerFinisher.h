#pragma once

#include "../SoccerPlayer.h"
#include "../../Attack/CharacterAttack.h"
#include "../../Soccer/SoccerAttackInfo_UsualAttack.h"
#include "../../../GameSystem/ForwardDecl.h"
#include "../../../character/Computer/CharacterComputerMove.h"

//****************************************************
//	�T�b�J�[�v���C���[�̃R���s���[�^�K�E�Z�N���X�w�b�_�[
//****************************************************
//�R���s���[�^����̕K�E�Z�N���X
class SoccerState_ComputerControll_Finisher :public SoccerState
{

private:
	//�@�������N���X�쐬
	CharacterShotAttack* SnakeShotClass(SoccerPlayer* s);
private:
	//�@�������N���X
	CharacterShotAttack* m_pSnakeShotClass;

public:
	int m_Timer;
	bool timeflg;
	//�@�R���X�g���N�^
	SoccerState_ComputerControll_Finisher();

	static void FreezeGame(UINT frame, SoccerPlayer* s);
	
	void Enter(SoccerPlayer* s);
	void Execute(SoccerPlayer* s);
	void Exit(SoccerPlayer* s);

};
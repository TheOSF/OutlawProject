#pragma once

#include "../SoccerPlayer.h"
#include "../../../character/CharacterEvasionClass.h"

//***************************************************
//		�T�b�J�[�R���s���[�^����N���X
//***************************************************

class SoccerState_ComputerControll_Rolling : public SoccerState
{
public:



	class CallBackClass
	{
	public:
		virtual ~CallBackClass() {}
		virtual Vector3 GetVec() = 0;
	};

	SoccerState_ComputerControll_Rolling(
		CallBackClass* pCallBackClass,  //���[�����O�����R���g���[���N���X(�I������delete����)
		bool dash);
	~SoccerState_ComputerControll_Rolling();

	// �X�e�[�g�J�n
	void Enter(SoccerPlayer* t)override;

	// �X�e�[�g���s
	void Execute(SoccerPlayer* t)override;

	// �X�e�[�g�I��
	void Exit(SoccerPlayer* t)override;

private:
	CallBackClass * const m_pCallBackClass;
	Vector3               m_Vec;
	int                   m_Timer;
	bool                  Dash;
};

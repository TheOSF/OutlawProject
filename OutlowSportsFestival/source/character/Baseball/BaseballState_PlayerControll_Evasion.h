#pragma once

#include "BaseballPlayer.h"
#include "../CharacterEvasionClass.h"

//***************************************************
//		�e�j�X����N���X
//***************************************************

class BaseballState_Rolling : public BaseballState
{
public:

	// ���p�p�����[�^
	struct EvasionParams
	{
		int AllFrame;                          // �S�t���[��
		int NoDamageStartFrame;     // ���G�J�n�t���[��
		int NoDamageEndFrame;      // ���G�I���t���[��
		float MoveDownSpeed;          // ��������
		float MoveSpeed;                   // �ړ��X�s�[�h
		RADIAN MaxTurnRadian;             // �����␳�̊p�x����
	};

	class CallBackClass
	{
	public:
		virtual ~CallBackClass(){}
		virtual Vector3 GetVec() = 0;
	};

	BaseballState_Rolling(
		CallBackClass* pCallBackClass  //���[�����O�����R���g���[���N���X(�I������delete����)
		);
	~BaseballState_Rolling();

	// �X�e�[�g�J�n
	void Enter(BaseballPlayer* b)override;

	// �X�e�[�g���s
	void Execute(BaseballPlayer* b)override;

	// �X�e�[�g�I��
	void Exit(BaseballPlayer* b)override;

private:
	CallBackClass * const m_pCallBackClass;
	Vector3               m_Vec;
	int                   m_Timer;
};


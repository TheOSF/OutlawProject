#pragma once

#include "TennisPlayer.h"
#include "../CharacterEvasionClass.h"

//***************************************************
//		�e�j�X����N���X
//***************************************************

class TennisState_Rolling : public TennisState
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

    TennisState_Rolling(
        CallBackClass* pCallBackClass  //���[�����O�����R���g���[���N���X(�I������delete����)
        );
    ~TennisState_Rolling();

    // �X�e�[�g�J�n
    void Enter(TennisPlayer* t)override;

    // �X�e�[�g���s
    void Execute(TennisPlayer* t)override;

    // �X�e�[�g�I��
    void Exit(TennisPlayer* t)override;

private:
    CallBackClass * const m_pCallBackClass;
    Vector3               m_Vec;
    int                   m_Timer;
};

#pragma once

#include "BaseballPlayer.h"
#include "../CharacterEvasionClass.h"

//***************************************************
//		�e�j�X����N���X
//***************************************************

class BaseballState_Rolling : public BaseballState
{
public:

    BaseballState_Rolling();
    BaseballState_Rolling(CrVector3 Vec);

	~BaseballState_Rolling();

	// �X�e�[�g�J�n
	void Enter(BaseballPlayer* b)override;

	// �X�e�[�g���s
	void Execute(BaseballPlayer* b)override;

	// �X�e�[�g�I��
	void Exit(BaseballPlayer* b)override;

private:

    // ���p�p�����[�^
    struct EvasionParams
    {
        int         AllFrame;
        int         Motion;
        int         NoDamageFrame;
        int         StandStartFrame;

        float       MoveValue;
        float       MoveDownFrame;
    };

	int                   m_Timer;
    EvasionParams         m_Param;
    Vector3               m_Vec;

    Vector3 GetPlayerControllMoveVec(BaseballPlayer* b);

    void SetParam(BaseballPlayer* b);
};


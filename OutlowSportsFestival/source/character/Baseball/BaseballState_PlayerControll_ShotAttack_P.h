#pragma once
#include "../../Damage/Damage.h"
#include "BaseballPlayer.h"



//�v���C���[����̉������N���X(�s�b�`���[)

class BaseballState_PlayerControll_ShotAttack_P :public BaseballState
{
public:

	//�@�X�e�[�g�J�n
	void Enter(BaseballPlayer* b)override;
	//�@�X�e�[�g���s
	void Execute(BaseballPlayer* b)override;
	//�@�X�e�[�g�I��
	void Exit(BaseballPlayer* b)override;

private:

    typedef void(BaseballState_PlayerControll_ShotAttack_P::*StateFunc)();
   
    StateFunc       m_pStateFunc;
    BaseballPlayer* m_pChr;
    int             m_StateTimer;

    void State_Throw1();
    void State_Throw2();
    void State_Throw3();
    void State_Throw4();
    void State_Throw5();

    void CreateBall(CrVector3 pos, CrVector3 move, bool Strong = false, float Damage = 4.0f);

    void AngleControll();

    StateFunc GetStateFunc(int ChargeCount)const;
};


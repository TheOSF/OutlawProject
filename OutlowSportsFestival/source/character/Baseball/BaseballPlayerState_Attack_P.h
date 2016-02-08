#ifndef __BASEBALL_PLAYER_CONTROLL_ATTACK_P_H__
#define __BASEBALL_PLAYER_CONTROLL_ATTACK_P_H__

#include "BaseballPlayer.h"
#include "BaseballAttackClass.h"

//***************************************************
//		�v���C���[����� �ߋ����U���N���X
//***************************************************

class Baseball_PlayerControll_Attack_P : public BaseballState
{
private:

	Baseball_PlayerControll_Attack_P(BaseballPlayer* b);
	~Baseball_PlayerControll_Attack_P();

	// �X�e�[�g�J�n
	void Enter(BaseballPlayer* b)override;

	// �X�e�[�g���s
	void Execute(BaseballPlayer* b)override;

	// �X�e�[�g�I��
	void Exit(BaseballPlayer* b)override;

private:
    typedef void(Baseball_PlayerControll_Attack_P::*StateFunc)();

    enum class NextType
    {
        DoCombo,
        DoFarAtk,

        None,
        _PreSelect
    };

    static const int m_sMaxCombo;

    BaseballPlayer* const   m_pChr;
    StateFunc               m_pStateFunc;
    NextType                m_NextType;
    int                     m_StateTimer;
    int                     m_ComboCount;


    void SetState(StateFunc state);
    void UpdateNextType();
    void AngleControll(RADIAN angle);

    void State_Atk();
};

#endif
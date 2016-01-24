#ifndef __TENNIS_PLAYER_STATE_ATTACK_H__
#define __TENNIS_PLAYER_STATE_ATTACK_H__

#include "TennisPlayer.h"
#include "TennisAttackClass.h"

//***************************************************
//		�v���C���[����� �ߋ����U���N���X
//***************************************************

class TennisState_PlayerControll_Attack : public TennisState
{
public:

    //�U������N���X
    class PlayerControllEvent :public TennisAttackClass::ControllEvent
    {
    public:
        PlayerControllEvent(TennisPlayer*const pTennis);

        bool isDoCombo(bool isHit, int CheckFrame);
        void AngleControll(RADIAN angle);

    private:
        TennisPlayer*const m_pTennis;

        const CharacterBase* GetFrontTargetEnemy();
    };

    TennisState_PlayerControll_Attack(TennisPlayer* t, bool StartSecond);
    ~TennisState_PlayerControll_Attack();

	// �X�e�[�g�J�n
	void Enter(TennisPlayer* t)override;

	// �X�e�[�g���s
	void Execute(TennisPlayer* t)override;

	// �X�e�[�g�I��
	void Exit(TennisPlayer* t)override;

private:
    TennisAttackClass   m_Attack;        //�U���N���X�ւ̃|�C���^
    bool                m_StartSecond;
};

#endif
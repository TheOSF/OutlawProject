#pragma once

#include "../TennisPlayer.h"
#include "../TennisAttackClass.h"

//***************************************************
//		�v���C���[����� �ߋ����U���N���X
//***************************************************

class TennisComputerState_Attack : public TennisState
{
public:

    //�U������N���X
    class ComputerControllEvent :public TennisAttackClass::ControllEvent
    {
    public:
        ComputerControllEvent(TennisPlayer*const pTennis);

        bool isDoCombo(bool isHit, int CheckFrame);
        void AngleControll(RADIAN angle);

    private:
        TennisPlayer*const m_pTennis;

        const CharacterBase* GetFrontTargetEnemy();
    };

    TennisComputerState_Attack(TennisPlayer* t, bool StartSecond);
    ~TennisComputerState_Attack();

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

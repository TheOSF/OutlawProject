#include "Tennis_DoCancelAction.h"
#include "../CharacterFunction.h"
#include "../../GameSystem/GameController.h"

#include "TennisSpecialAtk.h"
#include "TennisUtillityClass.h"

//�x�[�X�N���X
Tennis_DoCancelAction::Tennis_DoCancelAction(TennisPlayer* pTennis) :
m_pTennis(pTennis),
m_Flags(0xFFFFFFFF)
{

}

void Tennis_DoCancelAction::SetDontActions(Action ac)
{
    m_Flags.set((size_t)ac);
}

bool Tennis_DoCancelAction::isCanAction(Action ac)const
{
    return m_Flags.test((size_t)ac);
}



//�v���C���[�p
Tennis_DoCancelAction_Player::Tennis_DoCancelAction_Player(TennisPlayer* pTennis) :
Tennis_DoCancelAction(pTennis)
{

}

bool Tennis_DoCancelAction_Player::DoAction()
{
    TennisPlayer* const t = m_pTennis;

    if (isCanAction(FarAtk) && controller::GetTRG(controller::button::sankaku, t->m_PlayerInfo.number))
    {// [��] �Ń{�[������
        t->SetState(new TennisPlayerState_SlowUpBall(new TennisUtillityClass::PlayerSlowBallControll(t)));
        return true;
    }

    if (isCanAction(SkillAtk) &&  chr_func::isCanSpecialAttack(t) && controller::GetTRG(controller::button::maru, t->m_PlayerInfo.number))
    {// [��] �ŕK�E�Z
        t->SetState(new TennisState_SpecialAtk(t));
        chr_func::ResetSkillGauge(t);
        return true;
    }

    if (isCanAction(NearAtk) && controller::GetTRG(controller::button::shikaku, t->m_PlayerInfo.number))
    {// [��] �� [�ߋ����U��]
        t->SetState(new TennisState_PlayerControll_Attack(t));
        return true;
    }

    if (isCanAction(Step) && controller::GetTRG(controller::button::batu, t->m_PlayerInfo.number))
    {// [�~] �� [���[�����O]
        t->SetState(new TennisState_Rolling(new TennisUtillityClass::PlayerRollingControll(t)));
        return true;
    }

    if (isCanAction(Counter) && controller::GetTRG(controller::button::_R1, t->m_PlayerInfo.number))
    {// [R1] �� [�J�E���^�[]
        t->SetState(new TennisState_PlayerControll_Counter());
        return true;
    }

    if (isCanAction(NearAtk) && controller::GetTRG(controller::button::_L1, t->m_PlayerInfo.number) &&
        t->isCanBoundBallAtk())
    {// [L1] �� �{�[������ɓ�����U��
        t->SetState(new TennisState_BoundShot(new TennisUtillityClass::PlayerShotControllClass(t)));
        return true;
    }

    return false;
}


//�R���s���[�^�[�p
Tennis_DoCancelAction_Computer::Tennis_DoCancelAction_Computer(TennisPlayer* pTennis) :
Tennis_DoCancelAction(pTennis)
{

}


bool Tennis_DoCancelAction_Computer::DoAction()
{

    return false;
}
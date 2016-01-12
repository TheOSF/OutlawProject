#include "AmefootPlayerState_Evasion.h"
#include "../MoveClass/AmefootEvasionStandUpClass.h"
#include "../../CharacterEvasionClass.h"
#include "../AmefootUsualHitEvent.h"
#include "../../../GameSystem/GameController.h"
#include "../../CharacterFunction.h"
#include "../AmefootPlayerState.h"

//-----------------------------------------------------------------------------------------
// AmefootPlayerState_Evasion
// [ �A���t�g ] ����s��������X�e�[�g�N���X
//-----------------------------------------------------------------------------------------
AmefootPlayerState_Evasion::AmefootPlayerState_Evasion()
{
    //�����X�e�[�gset
    SetState(&AmefootPlayerState_Evasion::State_DownStart);
}

//-----------------------------------------------------------------------------------------
void AmefootPlayerState_Evasion::Enter(AmefootPlayer* pCharacter)
{
    m_pChr = pCharacter;
}
//-----------------------------------------------------------------------------------------
void AmefootPlayerState_Evasion::Execute(AmefootPlayer* pCharacter)
{
    chr_func::XZMoveDown(m_pChr, 0.05f);

    (this->*m_pStateFunc)();

    chr_func::CreateTransMatrix(m_pChr, &m_pChr->m_Renderer.m_TransMatrix);
}
//-----------------------------------------------------------------------------------------
void AmefootPlayerState_Evasion::Exit(AmefootPlayer* pCharacter)
{

}
//-----------------------------------------------------------------------------------------

bool AmefootPlayerState_Evasion::isStandUp()
{
    
    if (m_pChr->m_PlayerInfo.player_type == PlayerType::_Player)
    {
        //�v���C���̏ꍇ�{�^����
        return !controller::GetPush(controller::button::batu, m_pChr->m_PlayerInfo.number);
    }
    else
    {
        //�R���s���[�^�̏ꍇ������
        RATIO DoStandUp = 0.0f;

        //�R���s���[�^�̏ꍇ�K����
        switch (m_pChr->m_PlayerInfo.strong_type)
        {
        case StrongType::_Weak:
            DoStandUp = 0.01f;
            break;

        case StrongType::_Usual:
            DoStandUp = 0.02f;
            break;

        case StrongType::_Strong:
            DoStandUp = 0.03f;
            break;

        default:
            MyAssert(false, "��Փx�ݒ肪����Ă��Ȃ��R���s���[�^���g���Ă��܂�");
            break;
        }
        
        return frand() <= DoStandUp;
    }

    return false;
}

void AmefootPlayerState_Evasion::SetState( StateFunc  pNewState)
{
    m_Timer = 0;
    m_pStateFunc = pNewState;
}


//-----------------------------------------------------------------------------------------
//                              StateFunc��
//-----------------------------------------------------------------------------------------
void AmefootPlayerState_Evasion::State_DownStart()
{
    const int DownFrame = 10;

    //���[�V�����Z�b�g
    if (m_Timer++ == 0)
    {
        m_pChr->m_Renderer.SetMotion(AmefootPlayer::Motion_Evasion_Start);
    }

    if (m_Timer > DownFrame)
    {
        //���X�e�[�g���Z�b�g
        SetState(&AmefootPlayerState_Evasion::State_Downing);
    }

    //�A�j���[�V�����X�V
    m_pChr->m_Renderer.Update(1);

    //�X�V
    chr_func::UpdateAll(m_pChr, &DamageManager::HitEventBase());
}



void AmefootPlayerState_Evasion::State_Downing()
{
    const int CanDownFrame = 70;

    //�Q�����̏ꍇ
    if (++m_Timer > CanDownFrame)
    {
        //���X�e�[�g���Z�b�g
        SetState(&AmefootPlayerState_Evasion::State_ViewAround);
    }

    //�r���ł�߂��ꍇ
    if (isStandUp())
    {
        //���X�e�[�g���Z�b�g
        SetState(&AmefootPlayerState_Evasion::State_StandUp);
    }

    //�A�j���[�V�����X�V
    m_pChr->m_Renderer.Update(1);

    //�X�V
    chr_func::UpdateAll(m_pChr, &DamageManager::HitEventBase());
}

void AmefootPlayerState_Evasion::State_StandUp()
{
    const int DownFrame = 30;

    //���[�V�����Z�b�g
    if (m_Timer++ == 0)
    {
        m_pChr->m_Renderer.SetMotion(AmefootPlayer::Motion_Evasion_Standup);
    }

    if (m_Timer > DownFrame)
    {
        //���X�e�[�g���Z�b�g
        m_pChr->SetState(AmefootPlayerState_UsualMove::GetPlayerControllMove(m_pChr));
    }

    //�A�j���[�V�����X�V
    m_pChr->m_Renderer.Update(1);

    //�X�V
    chr_func::UpdateAll(m_pChr, &AmefootUsualHitEvent(m_pChr));
}

void AmefootPlayerState_Evasion::State_ViewAround()
{
    const int DownFrame = 28;

    //���[�V�����Z�b�g
    if (m_Timer++ == 0)
    {
        m_pChr->m_Renderer.SetMotion(AmefootPlayer::Motion_Evasion_ViewAround);
    }

    if (m_Timer > DownFrame)
    {
        //���X�e�[�g���Z�b�g
        SetState(&AmefootPlayerState_Evasion::State_StandUp);
    }

    //�A�j���[�V�����X�V
    m_pChr->m_Renderer.Update(2);

    //�X�V
    chr_func::UpdateAll(m_pChr, &DamageManager::HitEventBase());
}
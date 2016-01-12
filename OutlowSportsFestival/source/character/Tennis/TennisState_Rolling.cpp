#include "TennisState_Rolling.h"
#include "../CharacterFunction.h"
#include "TennisPlayerState_UsualMove.h"
#include "Tennis_HitEvent.h"
#include "../../Effect/EffectFactory.h"
#include "../../Sound/Sound.h"
#include "../../Camera/Camera.h"
#include "../../Effect/SpeedEffect.h"

TennisState_Rolling::TennisState_Rolling(CallBackClass* pCallBackClass) :
m_pCallBackClass(pCallBackClass),
m_SpeedEffect(1)
{

}

TennisState_Rolling::~TennisState_Rolling()
{
    delete m_pCallBackClass;
}


// �X�e�[�g�J�n
void TennisState_Rolling::Enter(TennisPlayer* t)
{
    m_Timer = 0;

    //�f�t�H���g�̃��[�����O�����Ƃ��ăL�����̑O�x�N�g��������
    chr_func::GetFront(t, &m_Vec);

    //�L�����N�^�̈ړ��ʂ�������
    chr_func::ResetMove(t);
}

// �X�e�[�g���s
void TennisState_Rolling::Execute(TennisPlayer* t)
{
    const int EndFrame = 27;          //�I���܂ł̃t���[��
    const int CanControllFrame = 4;   //�ړ��������R���g���[���ł���t���[��
    const int NoDamageFrame = 15;     //���G����
    const int CanCancel = 15;         //�L�����Z���\�t���[��

    const float MoveValue = 0.5f;    //�ړ���
    const float DownValue = 0.135f;     //������

    //�t���[���J�E���g�X�V
    ++m_Timer;

    //���[�V�����Z�b�g
    if (m_Timer == 1)
    {
        t->m_Renderer.SetMotion(TennisPlayer::_mt_EscapeStep);
    }

    //�ړ��������R���g���[��
    if (m_Timer < CanControllFrame)
    {
        Vector3 Vec = m_pCallBackClass->GetVec();

        //�l���������ꍇ�͍X�V
        if (Vec != Vector3Zero)
        {
            m_Vec = Vec;
            chr_func::AngleControll(t, t->m_Params.pos + m_Vec, 0.3f);
        }
    }

    //�R���g���[���ł���t���[�����I������ꍇ�����ƈړ���ݒ�
    if (m_Timer == CanControllFrame)
    {
        chr_func::AngleControll(t, t->m_Params.pos + m_Vec);

        chr_func::AddMoveFront(t, MoveValue, MoveValue);
    }

    //�G�t�F�N�g
    if (m_Timer < 13 && m_Timer > CanControllFrame)
    {
        m_SpeedEffect.Update(t->m_Params.pos + Vector3(0, 2, 0) + chr_func::GetFront(t)*2.8f, -chr_func::GetFront(t)*0.35f);
    }

    //�L�����Z���s��
    if (CanCancel < m_Timer)
    {
        m_pCallBackClass->CancelUpdate();
    }

    //�I��
    if (m_Timer == EndFrame)
    {
        //�ʏ�X�e�[�g�Ɉڍs
        t->SetState(TennisState_PlayerControll_Move::GetPlayerControllMove(t));
    }

    //��{�I�ȍX�V
    {
        DamageManager::HitEventBase NoDmgHitEvent;   //�m�[�_���[�W
        TennisHitEvent              UsualHitEvent(t);//�ʏ�


        //�ړ��ʂ̌���
        if (m_Timer > EndFrame - 13)
        {
            chr_func::XZMoveDown(t, DownValue);
        }
        

        //���G�t���[�����ɂ���ăq�b�g�C�x���g�N���X�̕���
        if (m_Timer < NoDamageFrame)
        {
            chr_func::UpdateAll(t, &NoDmgHitEvent);
        }
        else
        {
            chr_func::UpdateAll(t, &UsualHitEvent);
        }

        //���f���X�V
        t->m_Renderer.Update(1);

        //�s��X�V
        chr_func::CreateTransMatrix(t, &t->m_Renderer.m_TransMatrix);
    }

    //�O��
    //{
    //    EffectFactory::SmokeParticle(
    //        t->m_Params.pos + Vector3(0, 2, 0) + Vector3Rand() * 0.2f,
    //        Vector3Zero,
    //        25,
    //        0.5f + frand()*0.5f,
    //        0x10FFA080
    //        );
    //}
}

// �X�e�[�g�I��
void TennisState_Rolling::Exit(TennisPlayer* t)
{

}
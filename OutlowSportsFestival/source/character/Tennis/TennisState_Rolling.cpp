#include "TennisState_Rolling.h"
#include "../CharacterFunction.h"
#include "TennisPlayerState_UsualMove.h"
#include "Tennis_HitEvent.h"
#include "../../Effect/EffectFactory.h"
#include "../../Sound/Sound.h"

TennisState_Rolling::TennisState_Rolling(CallBackClass* pCallBackClass) :
m_pCallBackClass(pCallBackClass)
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
    const int EndFrame = 52;          //�I���܂ł̃t���[��
    const int CanControllFrame = 2;   //�ړ��������R���g���[���ł���t���[��
    const int NoDamageFrame = 10;     //���G����

    const float MoveValue = 0.7f;    //�ړ���
    const float DownValue = 0.05f;     //������

    //�t���[���J�E���g�X�V
    ++m_Timer;

    //���[�V�����Z�b�g
    if (m_Timer == 1)
    {
        t->m_Renderer.SetMotion(TennisPlayer::_mt_Rolling);
    }

    //�ړ��������R���g���[��
    if (m_Timer < CanControllFrame)
    {
        Vector3 Vec = m_pCallBackClass->GetVec();

        //�l���������ꍇ�͍X�V
        if (Vec != Vector3Zero)
        {
            m_Vec = Vec;
        }
    }

    //�R���g���[���ł���t���[�����I������ꍇ�����ƈړ���ݒ�
    if (m_Timer == CanControllFrame)
    {
        m_Vec.y = 0;
        m_Vec.Normalize();

        chr_func::AngleControll(t, t->m_Params.pos + m_Vec);

        chr_func::AddMoveFront(t, MoveValue, MoveValue);
    }

    //�I��
    if (m_Timer == EndFrame)
    {
        //�ʏ�X�e�[�g�Ɉڍs
        t->SetState(TennisState_PlayerControll_Move::GetPlayerControllMove(t));
    }

    //���G�t�F�N�g
    {
        //�X�^�[�g��
        if (m_Timer == 2)
        {
            for (int i = 0; i < 2; ++i)
            {
                EffectFactory::Smoke(
                    t->m_Params.pos + Vector3(frand() - 0.5f, frand(), frand() - 0.5f)*2.0f,
                    Vector3Zero,
                    1.5f,
                    0xFFFFA080,
                    true
                    );
            }
        }

        //�O��
        if (m_Timer < 5)
        {
            for (int i = 0; i < 2; ++i)
            {
                EffectFactory::Smoke(
                    t->m_Params.pos + Vector3(0, 2, 0) + Vector3Rand() * 0.2f, 
                    Vector3Zero,
                    1.0f + frand()*0.5f,
                    0x20FFA080
                    );
            }
        }

        //���n��
        if (m_Timer == 43)
        {
            for (int i = 0; i < 2; ++i)
            {
                EffectFactory::Smoke(
                    t->m_Params.pos + Vector3(frand() - 0.5f, frand(), frand() - 0.5f)*2.0f,
                    Vector3Zero,
                    2.5f,
                    0xFFFFA080,
                    true
                    );
            }
        }

    }

    //�T�E���h
    if (m_Timer == EndFrame - 25)
    {
        //�Y�U�[��
        Sound::Play(Sound::Sand1);
    }

    //��{�I�ȍX�V
    {
        DamageManager::HitEventBase NoDmgHitEvent;   //�m�[�_���[�W
        TennisHitEvent              UsualHitEvent(t);//�ʏ�


        //�ړ��ʂ̌���
        chr_func::XZMoveDown(t, DownValue);

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
        chr_func::CreateTransMatrix(t, t->m_ModelSize, &t->m_Renderer.m_TransMatrix);
    }
}

// �X�e�[�g�I��
void TennisState_Rolling::Exit(TennisPlayer* t)
{

}
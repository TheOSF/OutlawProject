#include "CharacterDamageVanish.h"
#include "CharacterFunction.h"
#include "CharacterBase.h"

#include "../Effect/EffectFactory.h"
#include "../Sound/Sound.h"

CharacterDamageVanish::CharacterDamageVanish(
    CharacterBase*    pCharacter,//������ԃL�����N�^
    const Param&      param, //�����Ƃуp�����[�^�[
    Event*            pEvent, //�C�x���g�N���X�ւ̃|�C���^(�f�X�g���N�^��delete����)
    DamageManager::HitEventBase* pHitEvent
    ) :
    m_pCharacter(pCharacter),
    m_pEvent(pEvent),
    m_pStateFunc(&CharacterDamageVanish::Initialize),
    m_Count(0),
    m_Rotate(0,0,0),
    m_pHitEvent(pHitEvent)
{
    m_Param = param;
}


CharacterDamageVanish::~CharacterDamageVanish()
{
    delete m_pEvent;
    delete m_pHitEvent;
}


// �X�V
void CharacterDamageVanish::Update()
{
    (this->*m_pStateFunc)();
}


void CharacterDamageVanish::Initialize()
{
    //�L�����N�^���_���[�W�̕����Ɍ�������
    chr_func::AngleControll(
        m_pCharacter,
        m_pCharacter->m_Params.pos - m_Param.move
        );

    //�ړ�������ݒ�
    m_pCharacter->m_Params.move = m_Param.move;

    //�X�^�[�g�C�x���g�Ăяo��
    m_pEvent->FlyStart();

    //������уX�e�[�g��
    m_pStateFunc = &CharacterDamageVanish::Flying;

    //�ړ��X�V
    chr_func::UpdateAll(m_pCharacter, &DamageManager::HitEventBase());

    //�T�E���h
    Sound::Play(Sound::Damage2);
}

void CharacterDamageVanish::Flying()
{
    ++m_Count;

    //�G�t�F�N�g
    {
        EffectFactory::Smoke(
            m_pCharacter->m_Params.pos + Vector3(0,2,0) +Vector3Rand()*0.2f, 
            Vector3Zero,
            1.0f + frand()*0.5f,
            0x20FFFFFF
            );
    }

    {
        //��s���̉�]�s����쐬����
        Matrix R;

        m_Rotate += m_Param.rotate_speed;

        D3DXMatrixRotationYawPitchRoll(
            &R,
            m_Rotate.y, m_Rotate.x, m_Rotate.z
            );

        //������ђ��֐��Ăяo��
        m_pEvent->Flying(R);
    }

    {
        //�ړ��X�V
        chr_func::UpdateAll(m_pCharacter, &DamageManager::HitEventBase());
    }


    //�n�ʂɂ��Ă����ꍇ�̓X�e�[�g�ڍs
    if (chr_func::isTouchGround(m_pCharacter) &&
        m_pCharacter->m_Params.move.y <= 0)
    {
        m_pEvent->DownStart();
        m_pStateFunc = &CharacterDamageVanish::Dowing;
        m_Count = 0;

        //�Y�U�[��
        Sound::Play(Sound::Sand2);
    }
}


void CharacterDamageVanish::Dowing()
{
    ++m_Count;

    chr_func::XZMoveDown(m_pCharacter, 0.08f);

    m_pEvent->Downing();


    //�G�t�F�N�g
    if (m_Count > 4 && m_Count < 10)
    {
        EffectFactory::Smoke(
            m_pCharacter->m_Params.pos + Vector3(frand() - 0.5f, frand(), frand() - 0.5f)*2.0f,
            Vector3(frand() - 0.5f, frand()*0.1f, frand() - 0.5f)*0.05f, 
            2.0f + frand()*2.0f,
            0xFFFFA080,
            true
            );
    }

    //�ړ��X�V
    if (m_Count > m_Param.down_muteki_frame)
    {
        //�ʏ퓖���蔻��
        chr_func::UpdateAll(m_pCharacter, m_pHitEvent);
    }
    else
    {
        //�m�[�_���[�W��
        chr_func::UpdateAll(m_pCharacter, &DamageManager::HitEventBase());
    }


    //�X�e�[�g�ڍs
    if (m_Count > m_Param.down_frame)
    {
        m_pEvent->StandUpStart();
        m_pStateFunc = &CharacterDamageVanish::StandUping;
        m_Count = 0;
    }
}


void CharacterDamageVanish::StandUping()
{
    ++m_Count;

    chr_func::XZMoveDown(m_pCharacter, 0.08f);

    m_pEvent->StandUping();


    //�ړ��X�V
    if (m_Count > m_Param.standup_muteki_frame)
    {
        //�ʏ퓖���蔻��
        chr_func::UpdateAll(m_pCharacter, m_pHitEvent);
    }
    else
    {
        //�m�[�_���[�W��
        chr_func::UpdateAll(m_pCharacter, &DamageManager::HitEventBase());
    }



    //�X�e�[�g�ڍs
    if (m_Count > m_Param.standup_frame)
    {
        m_pEvent->End();
        m_pStateFunc = &CharacterDamageVanish::End;
        m_Count = 0;
    }
}


void CharacterDamageVanish::End()
{
    //�ړ��X�V
    {
        chr_func::UpdateAll(m_pCharacter, m_pHitEvent);
    }

    if (m_Count == 0)
    {
        m_Count = 1;
        m_pEvent->End();
    }
}
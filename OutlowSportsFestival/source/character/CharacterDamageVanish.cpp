#include "CharacterDamageVanish.h"
#include "CharacterFunction.h"
#include "CharacterBase.h"

CharacterDamageVanish::CharacterDamageVanish(
    CharacterBase*    pCharacter,//������ԃL�����N�^
    const Param&      param, //�����Ƃуp�����[�^�[
    Event*            pEvent //�C�x���g�N���X�ւ̃|�C���^(�f�X�g���N�^��delete����)
    ) :
    m_pCharacter(pCharacter),
    m_Start(false),
    m_End(false),
    m_pEvent(pEvent),
    m_pStateFunc(&CharacterDamageVanish::Flying),
    m_Count(0),
    m_Rotate(0,0,0)
{
    m_Param = param;
}


CharacterDamageVanish::~CharacterDamageVanish()
{
    delete m_pEvent;
}


// �X�V
void CharacterDamageVanish::Update()
{
    (this->*m_pStateFunc)();
}

void CharacterDamageVanish::Flying()
{
    if (m_Start == false)
    {
        m_Start = true;

        //�L�����N�^���_���[�W�̕����Ɍ�������
        chr_func::AngleControll(
            m_pCharacter,
            m_pCharacter->m_Params.pos - m_Param.move
            );

        //�ړ�������ݒ�
        m_pCharacter->m_Params.move = m_Param.move;

        //�X�^�[�g�C�x���g�Ăяo��
        m_pEvent->Start();
    }

    //��s���̉�]�s����쐬����
    Matrix R;

    m_Rotate += m_Param.rotate_speed;

    D3DXMatrixRotationYawPitchRoll(
        &R,
        m_Rotate.y, m_Rotate.x, m_Rotate.z
        );

    m_pEvent->Flying(R);

    {
        //�ړ��X�V
        chr_func::UpdateMoveY(m_pCharacter);
        chr_func::PositionUpdate(m_pCharacter);
        chr_func::CheckGround(m_pCharacter);
    }


    //�n�ʂɂ��Ă����ꍇ�̓X�e�[�g�ڍs
    if (chr_func::isTouchGround(m_pCharacter) &&
        m_pCharacter->m_Params.move.y <= 0)
    {
        m_pEvent->StandUpStart();
        m_pStateFunc = &CharacterDamageVanish::StandUp;
    }
}



void CharacterDamageVanish::StandUp()
{
    //�t���[���J�E���g
    ++m_Count;

    chr_func::XZMoveDown(m_pCharacter, 0.2f);

    //�I������
    if (m_End == false &&
        m_Param.standup_frame <= m_Count)
    {
        m_End = true;
        m_pEvent->End();
    }
}
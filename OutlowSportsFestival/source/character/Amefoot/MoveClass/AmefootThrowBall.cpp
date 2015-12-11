#include "AmefootThrowBall.h"
#include "../AmefootPlayer.h"
#include "../../CharacterFunction.h"
#include "../AmefootPlayerState.h"


//-----------------------------------------------------------------------------------------
// AmefootThrowBall
// [ �A���t�g ] �������U��(�{�[������)�N���X
// �A���t�g��p�Ȃ̂ŃC�x���g���g�p���Ȃ�
//-----------------------------------------------------------------------------------------
AmefootThrowBall::AmefootThrowBall(
    AmefootPlayer* pAmefoot,
    const Params& params,
    DamageManager::HitEventBase* pHitEventBase
    ) :
    m_pAmefoot(pAmefoot),
    m_Params(params),
    m_pHitEventBase(pHitEventBase),
    m_pStateFunc(&AmefootThrowBall::Begin)
{}
//-----------------------------------------------------------------------------------------
// �X�e�B�b�N�̒l�Z�b�g(��������������߂�)
void AmefootThrowBall::SetStickValue(CrVector2 stick)
{
    m_StickValue = stick;

    //�J������ɕϊ�
    Vector3 temp(m_StickValue.x, m_StickValue.y, 0);

    temp = Vector3MulMatrix3x3(temp, matView);

    m_StickValue.x = temp.x;
    m_StickValue.y = temp.y;
}
//-----------------------------------------------------------------------------------------
// �X�V
bool AmefootThrowBall::Update()
{
    if ( m_pStateFunc )
    {
        (this->*m_pStateFunc)();
    }
    return true;
}
//-----------------------------------------------------------------------------------------
// �J�n
void AmefootThrowBall::Begin()
{
    // �J�E���^�X�V
    ++m_Counter;

    if ( m_Counter == 1 )
    {

        // �{�[������


        // ���[�V�����Z�b�g
        m_pAmefoot->m_Renderer.SetMotion(m_Params.ThrowMotionNumber);
    }

    // �����X�e�[�g��
    if ( m_Counter < m_Params.ThrowFrame )
    {
        SetState(&AmefootThrowBall::Throw);
    }

    // ��{�I�ȍX�V
    chr_func::UpdateAll(m_pAmefoot, m_pHitEventBase);
}
//-----------------------------------------------------------------------------------------
// ����
void AmefootThrowBall::Throw()
{
    // �J�E���^�X�V
    ++m_Counter;

    if ( m_Counter == 1 )
    {
        // �X�e�B�b�N�̒l�ɂ��p�x�␳
        chr_func::AngleControll(
            m_pAmefoot,
            m_pAmefoot->m_Params.pos + Vector3(m_StickValue.x, 0, m_StickValue.y),
            m_Params.MaxTurnRadian
            );

        // �L�����N�^�[�̑O�����ɔ�΂�

        // �{�[���̈ʒu�␳

        // �Q�[�W�㏸�H

        // �{�[���̐ݒ�

        // �G�t�F�N�g

        // �T�E���h

        // �O�ɐi�ށH
    }

    // �X�e�[�g�I��
    if ( m_Counter > m_Params.AfterThrowFrame )
    {
        SetState(&AmefootThrowBall::End);
        return;
    }

    // ��{�I�ȍX�V
    chr_func::UpdateAll(m_pAmefoot, m_pHitEventBase);

}
//-----------------------------------------------------------------------------------------
// �I��
void AmefootThrowBall::End()
{
    if ( m_Counter == 0 )
    {
        //��񂵂����s���Ȃ��悤��
        ++m_Counter;

        // �X�e�[�g�ڍs
        // �v���C���[��p�����Ȃ��̂łƂ肠������������
        m_pAmefoot->SetState(new AmefootPlayerState_UsualMove());
    }

    // ��{�I�ȍX�V
    chr_func::UpdateAll(m_pAmefoot, m_pHitEventBase);
}
//-----------------------------------------------------------------------------------------
// �X�e�[�g�ݒ�
void AmefootThrowBall::SetState(StateFunc func)
{
    m_pStateFunc = func;
    m_Counter = 0;
}



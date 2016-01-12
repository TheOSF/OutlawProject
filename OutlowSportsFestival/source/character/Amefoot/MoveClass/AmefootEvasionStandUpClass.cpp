#include "AmefootEvasionStandUpClass.h"
#include "../AmefootPlayer.h"
#include "../../CharacterFunction.h"
#include "../AmefootPlayerState.h"


//-----------------------------------------------------------------------------------------
// AmefootEvasionStandUpClass
// [ �A���t�g ] ����㗧���オ��N���X
// �A���t�g��p�Ȃ̂ŃC�x���g���g�p���Ȃ�
//-----------------------------------------------------------------------------------------
AmefootEvasionStandUpClass::AmefootEvasionStandUpClass(
     AmefootPlayer* pAmefoot,
     Params params
     ) :
     m_pAmefoot(pAmefoot),
     m_timer(0)
{
    //���͌���t���O��on�Ȃ�
    if (params.isViewAround)
    {
        m_pStateFunc = &AmefootEvasionStandUpClass::State_ViewAround;
    }
    else
    {
        m_pStateFunc = &AmefootEvasionStandUpClass::State_StandUp;
    }

    
}
//-----------------------------------------------------------------------------------------
bool AmefootEvasionStandUpClass::Update()
{
    //�X�V�������t���O�擾
    bool ret = (this->*m_pStateFunc)();

    // ���f���̃��[���h�ϊ��s����X�V
    chr_func::CreateTransMatrix(
        m_pAmefoot,
        &m_pAmefoot->m_Renderer.m_TransMatrix
        );

    return ret;
}
bool AmefootEvasionStandUpClass::State_ViewAround()
{
    const int AroundViewFrame = 28;

    //�A�j���[�V�����X�V
    m_pAmefoot->m_Renderer.Update(2.0f);

    if (m_timer == 0)
    {
        m_pAmefoot->m_Renderer.SetMotion(AmefootPlayer::Motion_Evasion_Standup);
    }

    m_timer++;

    //���Ԍo�߂Ŏ��͂���낫���֐����Z�b�g
    if (m_timer >= AroundViewFrame)
    {
        m_timer = 0;
        m_pStateFunc = &AmefootEvasionStandUpClass::State_StandUp;
    }

    return true;
}

bool AmefootEvasionStandUpClass::State_StandUp()
{
    const int StandUpFrame = 50;

    //�A�j���[�V�����X�V
    m_pAmefoot->m_Renderer.Update(1);

    
    if (m_timer == 0)
    {
        m_pAmefoot->m_Renderer.SetMotion(AmefootPlayer::Motion_Evasion_Standup);
    }

    m_timer++;

    if (m_timer >= StandUpFrame)
    {
        m_pAmefoot->SetState(new AmefootPlayerState_UsualMove());
        return false;
    }

    return true;
}

//-----------------------------------------------------------------------------------------



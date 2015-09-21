#include "CharacterDefaultNearAtk.h"
#include "../CharacterFunction.h"
#include "../CharacterBase.h"


//****************************************************************
//		�L�����N�^���ʂ̍U���N���X
//****************************************************************

CharacterDefaultNearAttack::CharacterDefaultNearAttack(
    CharacterBase*	pParent,	//����L�����N�^�̃|�C���^
    const Params&	param,
    AttackEvent*	pAtttackEvent) :
    m_pCharacter(pParent),
    m_timer(0),
    m_pAttackEvent(pAtttackEvent)
{
    m_Params = param;

    m_Damage.pBall = NULL;
    m_Damage.pParent = pParent;

}

CharacterDefaultNearAttack::~CharacterDefaultNearAttack()
{
    delete m_pAttackEvent;
}

void CharacterDefaultNearAttack::Update()
{
    if (m_timer == 0)
    {
        // �U���J�n
        m_pAttackEvent->Start();

    }

    // ���W�X�V
    chr_func::PositionUpdate(m_pCharacter);

    // �X�V
    m_pAttackEvent->Update(m_timer);

    {
        if (m_timer == m_Params.DamageStartFrame)
        {
            // �_���[�W�����t���[��
            m_Damage.m_Enable = true;
            m_pAttackEvent->DamageStart();
        }

        if (m_timer > m_Params.DamageStartFrame&&
            m_timer < m_Params.DamageEndFrame)
        {
            // �_���[�W������
            m_pAttackEvent->DamageUpdate();
        }

        if (m_timer == m_Params.DamageEndFrame)
        {
            // �_���[�W���Ńt���[��
            m_Damage.m_Enable = false;
            m_pAttackEvent->DamageEnd();
        }
    }

    //�����蔻��ʒu�̍X�V
    UpdateDamagePos();

    if (m_timer == m_Params.EndFrame)
    {// �U���I��
        m_pAttackEvent->End();
    }

    // �^�C�}�[�X�V
    m_timer = min(m_timer + 1, INT_MAX);
}

void CharacterDefaultNearAttack::UpdateDamagePos()
{
    Matrix R;

    D3DXMatrixRotationY(&R, m_pCharacter->m_Params.angle);
    
    m_Damage.m_Param.pos = m_pCharacter->m_Params.pos + Vector3MulMatrix3x3(m_Params.DamagePosOffset, R);
}

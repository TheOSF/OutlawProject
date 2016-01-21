#include "CharacterBase.h"
#include "CharacterShotAttackClass.h"
#include "CharacterFunction.h"
#include "../Effect/ParticleRenderer.h"
#include "../Effect/ParticleMoveObject.h"
#include "../GameSystem/ResourceManager.h"

#include "../Ball/Ball.h"
#include "../Effect/HitEffectObject.h"

//***************************************************
//		���N���X�p �������U�� (�{�[���ł��o��) �N���X
//***************************************************

// �R���X�g���N�^
CharacterShotAttack::CharacterShotAttack(
	CharacterBase* pCharacter,
	Event* pEvent,
	const AttackParams& attack_params,
	HitEventBase*    pHitEventBase
	) :
	m_AttackParams(attack_params),
	m_StickValue(0, 0),
	m_pCharacter(pCharacter),
	m_Timer(0),
	m_pEvent(pEvent),
	m_pHitEventBase(pHitEventBase)
{

}


// �f�X�g���N�^
CharacterShotAttack::~CharacterShotAttack()
{
	delete m_pEvent;
	delete m_pHitEventBase;
}


// �X�V
bool CharacterShotAttack::Update()
{
	if (m_Timer == 0)
	{// �U���J�n
		m_pEvent->AttackStart();
	}

    if (m_Timer < m_AttackParams.ShotFrame)
    {
        CharacterBase* pTarget = nullptr;
        
        if (chr_func::CalcAtkTarget(m_pCharacter, D3DXToRadian(90), 60.0f, &pTarget))
        {
            // �����␳
            chr_func::AngleControll(
                m_pCharacter,
                pTarget->m_Params.pos,
                D3DXToRadian(10)
                );
        }

        
    }

	// ����
	chr_func::XZMoveDown(
		m_pCharacter,
		m_AttackParams.MoveDownSpeed);

	// �X�V
	m_pEvent->Update();

	if (m_Timer == m_AttackParams.ShotFrame)
	{// �_���[�W�����t���[��
		m_pEvent->Shot();
	}

	if (m_Timer >= m_AttackParams.AllFrame)
	{// �U���I��
		m_pEvent->AttackEnd();
		return false;
	}

	// �^�C�}�[�X�V
	m_Timer++;


    chr_func::UpdateAll(m_pCharacter, m_pHitEventBase);


	return true;
}


// �X�e�B�b�N�̒l�Z�b�g
void CharacterShotAttack::SetStickValue(CrVector2 stickValue)
{
	m_StickValue = stickValue;
}


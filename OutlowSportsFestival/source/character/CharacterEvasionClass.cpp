#include "CharacterBase.h"
#include "CharacterEvasionClass.h"
#include "CharacterFunction.h"


//***************************************************
//		���N���X�p����N���X
//***************************************************


// �R���X�g���N�^
CharacterEvasion::CharacterEvasion(
	CharacterBase* pCharacterBase,
	Event* pEvent,
	const EvasionParams& evasion_params
	) :
	m_EvasionParams(evasion_params),
	m_StickValue(0, 0),
	m_pCharacterBase(pCharacterBase),
	m_Timer(0),
	m_pEvent(pEvent)
{

}


// �f�X�g���N�^
CharacterEvasion::~CharacterEvasion()
{
	delete m_pEvent;
}


// �X�V
bool CharacterEvasion::Update()
{

	if (m_Timer == 0)
	{// ����J�n
        m_pEvent->EvasionStart();

        {
            Vector3 ViewPos;

            ViewPos = m_pCharacterBase->m_Params.pos;
            ViewPos += Vector3(m_StickValue.x, 0, m_StickValue.y);

            // �����␳
            chr_func::AngleControll(
                m_pCharacterBase,
                ViewPos,
                cosf(m_EvasionParams.MaxTurnRadian)
                );
        }

        // �ړ�
        chr_func::AddMoveFront(
            m_pCharacterBase,
            m_EvasionParams.MoveSpeed,
            m_EvasionParams.MoveSpeed);
	}

	// ���W�X�V
	chr_func::PositionUpdate(m_pCharacterBase);

	// ����
	chr_func::XZMoveDown(
		m_pCharacterBase,
		m_EvasionParams.MoveDownSpeed);

	// �X�V
	m_pEvent->Update();

	if (m_Timer >= m_EvasionParams.AllFrame)
	{
          if(!m_pEvent->IsEvasionContinue())
          {
               // ����I��
               m_pEvent->EvasionEnd();
               return false;
          }
	}

	// �^�C�}�[�X�V
	m_Timer++;
	
	return true;
}


// �X�e�B�b�N�̒l�Z�b�g
void CharacterEvasion::SetStickValue(CrVector2 stickValue)
{
	m_StickValue = stickValue;
}


// �_���[�W���󂯂邱�Ƃ��ł��邩�ǂ���
bool CharacterEvasion::CanGetDamage()const
{
	return (
		m_Timer < m_EvasionParams.NoDamageStartFrame || 
		m_Timer >= m_EvasionParams.NoDamageEndFrame
		);
}

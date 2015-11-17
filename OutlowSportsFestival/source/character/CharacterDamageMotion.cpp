#include "CharacterDamageMotion.h"
#include "CharacterFunction.h"
#include "CharacterBase.h"
#include "../Camera/Camera.h"
#include "../Sound/Sound.h"


CharacterDamageMotion::CharacterDamageMotion(
	CharacterBase* pCharacter,
	Event* pEvent,
	DamageManager::HitEventBase* pHitEvent,
	const Params& params
	):
	m_pCharacter(pCharacter),
	m_Timer(0),
	m_pEvent(pEvent),
	m_Start(false),
	m_End(false),
	m_pHitEvent(pHitEvent)
{
	m_Params = params;
    m_Pos = m_pCharacter->m_Params.pos;


    //�Ђ�ޕ������Ȃ��ꍇ�������Ă�������̂܂�
    if (m_Params.damage_vec.x == 0 && m_Params.damage_vec.z)
    {
        chr_func::GetFront(m_pCharacter, &m_Params.damage_vec);
        m_Params.damage_vec = -m_Params.damage_vec * 0.001f;
    }
}

CharacterDamageMotion::~CharacterDamageMotion()
{
    //���C�g��off�ɂ���
    m_pEvent->SetLight(0);

	delete m_pEvent;
	delete m_pHitEvent;
}


// �X�V
void CharacterDamageMotion::Update()
{
    const int AllFrame = 25;
    const int NoDamageFrame = 5;

	//�t���[���X�V
	++m_Timer;

	if (m_Start == false)
	{
		m_Start = true;

		//�L�����N�^���_���[�W�̕����Ɍ�������
		chr_func::AngleControll(
			m_pCharacter,
			m_pCharacter->m_Params.pos - m_Params.damage_vec
			);

        //�q�b�g�o�b�N����
        {
            const float val = Vector3XZLength(m_Params.damage_vec);
            chr_func::AddMoveFront(m_pCharacter, -val, val);
        }

        //�J�����̂��
        DefCamera.SetShock(Vector2(0.05f, 0.05f), 15);

		//�X�^�[�g�C�x���g�Ăяo��
		m_pEvent->Start();

        //�T�E���h
        Sound::Play(Sound::Damage3);
	}

    //�኱�L�����N�^���W��h�炷(�H����Ă���̂��������邽��)
    {
        m_Pos = m_pCharacter->m_Params.pos;

        if (m_Timer < 15)
        {
            m_pCharacter->m_Params.pos += Vector3Rand()*0.25f;
        }
    }

    //�L�����N�^���኱���点��p�����[�^�𑗂�
    {
        m_pEvent->SetLight(pow(1 - min(((float)m_Timer / 15.0f), 1), 2)*0.25f);
    }

    //�C�x���g�N���X�X�V
    m_pEvent->Update(1.0f);

    //�I������
    if (m_End == false &&
        AllFrame <= m_Timer)
    {
        m_End = true;
        m_pEvent->End();
    }



    //���G�t���[������
    if (m_Timer > NoDamageFrame)
    {
        //�_���[�W����
        chr_func::DamageCheck(m_pCharacter, m_pHitEvent);
    }

    

    //�O��̃t���[���ŗh��ē������������ɖ߂�
    m_pCharacter->m_Params.pos = m_Pos;


    //�L�����N�^�X�V
    {
        //�q�b�g�o�b�N����
        chr_func::XZMoveDown(m_pCharacter, 0.1f);

        //�d�͉��Z
        chr_func::UpdateMoveY(m_pCharacter);

        //�ʒu���X�V
        chr_func::PositionUpdate(m_pCharacter);

        //�ǂƂ̐ڐG����
        chr_func::CheckWall(m_pCharacter);

        //���Ƃ̐ڐG����
        chr_func::CheckGround(m_pCharacter);

    }
}
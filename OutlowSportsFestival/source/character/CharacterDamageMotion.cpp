#include "CharacterDamageMotion.h"
#include "CharacterFunction.h"
#include "CharacterBase.h"
#include "../Camera/Camera.h"
#include "../Sound/Sound.h"
#include "../Effect/ImpactLightObject.h"
#include "../Effect/EffectFactory.h"


CharacterDamageMotion::Params::Params()
{
    frame = 25;
    damage_vec = Vector3AxisX;
    counter_hit = false;
}

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
    const float AllFrame = 25;
    const float NoDamageFrame = 5;
    const float AddSpeed = (m_Params.counter_hit) ? (0.33333f) : (25.0f / (float)m_Params.frame);

	//�t���[���X�V
    m_Timer += AddSpeed;

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

        //�J�E���^�[���o
        if (m_Params.counter_hit)
        {
            const Vector3 EffectPos = m_pCharacter->m_Params.pos + Vector3(0, 2, 0);
            const Vector3 Glavity(0, -0.02f, 0);
            const COLORf  EffectColor(1.0f, 1, 0.8f, 0);

            //�r�d
            Sound::Play(Sound::Damage2);
            Sound::Play(Sound::AtkHit1);

            //�G�t�F�N�g
            new ImpactLightObject(
                EffectPos,
                45,
                Vector3(1, 0.4f, 0),
                0.1f
                );

            for (int i = 0; i < 10; ++i)
            {
                EffectFactory::LocusParticle(
                    EffectPos,
                    (Vector3Rand() - chr_func::GetFront(m_pCharacter))*1.5f,
                    Glavity,
                    0.1f,
                    8,
                    EffectColor,
                    EffectColor,
                    45,
                    0.1f,
                    0.2f
                    );
            }
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

        if (m_Timer < 15 && m_Params.counter_hit == false)
        {
            m_pCharacter->m_Params.pos.x += (frand() - 0.5f)*0.35f;
            m_pCharacter->m_Params.pos.z += (frand() - 0.5f)*0.35f;
        }
    }

    //�L�����N�^���኱���点��p�����[�^�𑗂�
    {
        m_pEvent->SetLight(pow(1 - min((m_Timer / 15.0f), 1), 2)*0.25f);
    }

    //�C�x���g�N���X�X�V
    m_pEvent->Update(AddSpeed);

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
        chr_func::XZMoveDown(m_pCharacter, 0.1f*AddSpeed);

        //�d�͉��Z
        chr_func::UpdateMoveY(m_pCharacter, AddSpeed);

        //�ʒu���X�V
        chr_func::PositionUpdate(m_pCharacter, AddSpeed);

        //�ǂƂ̐ڐG����
        chr_func::CheckWall(m_pCharacter);

        //���Ƃ̐ڐG����
        chr_func::CheckGround(m_pCharacter);

    }
}
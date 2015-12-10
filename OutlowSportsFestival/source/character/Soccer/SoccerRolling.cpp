#include "SoccerRolling.h"
#include "../CharacterFunction.h"
#include "SoccerPlayerState.h"
#include "SoccerHitEvent.h"
#include "../../Effect/EffectFactory.h"
#include "../../Sound/Sound.h"


SoccerState_Rolling::SoccerState_Rolling(CallBackClass* pCallBackClass,bool dash) :
m_pCallBackClass(pCallBackClass), Dash(dash)
{

}

SoccerState_Rolling::~SoccerState_Rolling()
{
	delete m_pCallBackClass;
}


// �X�e�[�g�J�n
void SoccerState_Rolling::Enter(SoccerPlayer* s)
{
	m_Timer = 0;

	//�f�t�H���g�̃��[�����O�����Ƃ��ăL�����̑O�x�N�g��������
	chr_func::GetFront(s, &m_Vec);

	//�L�����N�^�̈ړ��ʂ�������
	chr_func::ResetMove(s);
}

// �X�e�[�g���s
void SoccerState_Rolling::Execute(SoccerPlayer* s)
{
	const int EndFrame = 52;          //�I���܂ł̃t���[��
	const int CanControllFrame = 2;   //�ړ��������R���g���[���ł���t���[��
	const int NoDamageFrame = 10;     //���G����

	float DownValue = 0.5f;
	float MoveValue = 0.05f;
	
	if (Dash)
	{
		 MoveValue = 0.6f;    //�ړ���
		 DownValue = 0.1f;     //������
	}
	else
	{
		 MoveValue = 0.4f;    //�ړ���
		 DownValue = 0.09f;     //������
	}

	

	//�t���[���J�E���g�X�V
	++m_Timer;

	//���[�V�����Z�b�g
	if (m_Timer == 1)
	{
		s->m_Renderer.SetMotion(SoccerPlayer::_ms_Rolling);
	}

	//�ړ��������R���g���[��
	if (m_Timer < CanControllFrame)
	{
		Vector3 Vec = m_pCallBackClass->GetVec();

		//�l���������ꍇ�͍X�V
		if (Vec != Vector3Zero)
		{
			m_Vec = Vec;
			chr_func::AngleControll(s, s->m_Params.pos + m_Vec, 0.3f);
		}
	}

	//�R���g���[���ł���t���[�����I������ꍇ�����ƈړ���ݒ�
	if (m_Timer == CanControllFrame)
	{
		/*m_Vec.y = 0;
		m_Vec.Normalize();*/

		chr_func::AngleControll(s, s->m_Params.pos + m_Vec);

		chr_func::AddMoveFront(s, MoveValue, MoveValue);
	}

	//�I��
	if (m_Timer == EndFrame)
	{
		//�ʏ�X�e�[�g�Ɉڍs
		s->SetState(SoccerState_PlayerControll_Move::GetPlayerControllMove(s));
	}

	//���G�t�F�N�g
	{
		//�X�^�[�g��
		if (m_Timer == 2)
		{
			for (int i = 0; i < 2; ++i)
			{
				EffectFactory::Smoke(
					s->m_Params.pos + Vector3(frand() - 0.5f, frand(), frand() - 0.5f)*2.0f,
					Vector3Zero,
					1.5f,
					1.0f,
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
					s->m_Params.pos + Vector3(0, 2, 0) + Vector3Rand() * 0.2f,
					Vector3Zero,
					1.0f + frand()*0.5f,
					0.6f
					);
			}
		}

		//���n��
		if (m_Timer == 43)
		{
			for (int i = 0; i < 2; ++i)
			{
				EffectFactory::Smoke(
					s->m_Params.pos + Vector3(frand() - 0.5f, frand(), frand() - 0.5f)*2.0f,
					Vector3Zero,
					2.5f,
					1.0f,
					true
					);
			}
		}

	}
	if (m_Timer == EndFrame - 25)
	{
		//�Y�U�[��
		Sound::Play(Sound::Sand1);
	}
	//��{�I�ȍX�V
	{
		DamageManager::HitEventBase NoDmgHitEvent;   //�m�[�_���[�W
		SoccerHitEvent              UsualHitEvent(s);//�ʏ�


		//�ړ��ʂ̌���
		if (m_Timer > EndFrame - 13)
		{
			chr_func::XZMoveDown(s, DownValue);
		}

		//���G�t���[�����ɂ���ăq�b�g�C�x���g�N���X�̕���
		if (m_Timer < NoDamageFrame)
		{
			chr_func::UpdateAll(s, &NoDmgHitEvent);
		}
		else
		{
			chr_func::UpdateAll(s, &UsualHitEvent);
		}

		//���f���X�V
		s->m_Renderer.Update(1);

		//�s��X�V
		chr_func::CreateTransMatrix(s, s->m_ModelSize, &s->m_Renderer.m_TransMatrix);
	}
}

// �X�e�[�g�I��
void SoccerState_Rolling::Exit(SoccerPlayer* t)
{

}
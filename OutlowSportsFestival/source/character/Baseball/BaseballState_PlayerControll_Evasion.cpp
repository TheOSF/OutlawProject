#include "BaseballState_PlayerControll_Evasion.h"
#include "../CharacterFunction.h"
#include "BaseballPlayerState.h"
#include "Baseball_HitEvent.h"
#include "../../Effect/EffectFactory.h"
#include "../../Sound/Sound.h"



BaseballState_Rolling::BaseballState_Rolling(CallBackClass* pCallBackClass) :
m_pCallBackClass(pCallBackClass)
{

}

BaseballState_Rolling::~BaseballState_Rolling()
{
	delete m_pCallBackClass;
}


// �X�e�[�g�J�n
void BaseballState_Rolling::Enter(BaseballPlayer* b)
{
	m_Timer = 0;

	//�f�t�H���g�̃��[�����O�����Ƃ��ăL�����̑O�x�N�g��������
	chr_func::GetFront(b, &m_Vec);

	//�L�����N�^�̈ړ��ʂ�������
	chr_func::ResetMove(b);
}

// �X�e�[�g���s
void BaseballState_Rolling::Execute(BaseballPlayer* b)
{
	const int EndFrame = 27;          //�I���܂ł̃t���[��
	const int CanControllFrame = 4;   //�ړ��������R���g���[���ł���t���[��
	const int NoDamageFrame = 10;     //���G����
	//const int CanCancel = 15;         //�L�����Z���\�t���[��

	float DownValue = 0.5f;     //������
	float MoveValue = 0.05f;

	//�@�o�b�^�[or�s�b�`���[�̈ړ���
	if (b->getBatterFlg())
	{
		MoveValue = 0.5f;    //�o�b�^�[��
		DownValue = 0.05f;     //������
	}
	else
	{
		MoveValue = 0.6f;    //�s�b�`���[��
		DownValue = 0.08f;     //������
	}

	//�t���[���J�E���g�X�V
	++m_Timer;

	//���[�V�����Z�b�g
	if (m_Timer == 1)
	{
		if (b->getBatterFlg())
		{
			b->m_Renderer.SetMotion(baseball_player::_mb_Evasion_B);
		}
		else
		{
			b->m_Renderer.SetMotion(baseball_player::_mb_Evasion_P);
		}
	}

	//�ړ��������R���g���[��
	if (m_Timer < CanControllFrame)
	{
		Vector3 Vec = m_pCallBackClass->GetVec();

		//�l���������ꍇ�͍X�V
		if (Vec != Vector3Zero)
		{
			m_Vec = Vec;
			chr_func::AngleControll(b, b->m_Params.pos + m_Vec, 0.3f);
		}
	}

	//�R���g���[���ł���t���[�����I������ꍇ�����ƈړ���ݒ�
	if (m_Timer == CanControllFrame)
	{
		/*m_Vec.y = 0;
		m_Vec.Normalize();*/

		chr_func::AngleControll(b, b->m_Params.pos + m_Vec);

		chr_func::AddMoveFront(b, MoveValue, MoveValue);
	}



	//�I��
	if (m_Timer == EndFrame)
	{
		//�ʏ�X�e�[�g�Ɉڍs
		b->SetState(BaseballState_PlayerControll_Move::GetPlayerControllMove(b));
	}

	//���G�t�F�N�g
	{
		//�X�^�[�g��
		if (m_Timer == 2)
		{
			for (int i = 0; i < 2; ++i)
			{
				EffectFactory::Smoke(
					b->m_Params.pos + Vector3(frand() - 0.5f, frand(), frand() - 0.5f)*2.0f,
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
					b->m_Params.pos + Vector3(0, 2, 0) + Vector3Rand() * 0.2f,
					Vector3Zero,
					1.0f + frand()*0.5f,
					0x20FFA080
					);
			}
		}

		//���n��
		if (m_Timer == 43)
		{
			for (int i = 0; i < 2; ++i)
			{
				EffectFactory::Smoke(
					b->m_Params.pos + Vector3(frand() - 0.5f, frand(), frand() - 0.5f)*2.0f,
					Vector3Zero,
					2.5f,
					1.0f,
					true
					);
			}
		}

	}

	//�T�E���h
	if (m_Timer == EndFrame - 25)
	{
		//�Y�U�[��
		Sound::Play(Sound::Sand1);
	}

	//��{�I�ȍX�V
	{
		DamageManager::HitEventBase NoDmgHitEvent;   //�m�[�_���[�W
		BaseballHitEvent              UsualHitEvent(b);//�ʏ�


		//�ړ��ʂ̌���
		if (m_Timer > EndFrame - 13)
		{
			chr_func::XZMoveDown(b, DownValue);
		}

		//���G�t���[�����ɂ���ăq�b�g�C�x���g�N���X�̕���
		if (m_Timer < NoDamageFrame)
		{
			chr_func::UpdateAll(b, &NoDmgHitEvent);
		}
		else
		{
			chr_func::UpdateAll(b, &UsualHitEvent);
		}

		//���f���X�V
		b->m_Renderer.Update(2);

		//�s��X�V
		chr_func::CreateTransMatrix(b, b->m_ModelSize, &b->m_Renderer.m_TransMatrix);
	}
}

// �X�e�[�g�I��
void BaseballState_Rolling::Exit(BaseballPlayer* b)
{

}
#include "SoccerCommonState.h"
#include "SoccerPlayerState.h"
#include "SoccerHitEvent.h"

#include "../CharacterFunction.h"

SoccerState_SmallDamage::SoccerState_SmallDamage(
	SoccerPlayer* pSoccer,
	const Vector3& Damage_vec  //�_���[�W���󂯂�����
	) :
	m_pSoccer(pSoccer),
	m_Damage_vec(Damage_vec)
{

}
void SoccerState_SmallDamage::Enter(SoccerPlayer* s)
{
	//�L�����N�^���ʂЂ�݃N���X�̃T�b�J�[�ŗL�C�x���g�N���X
	class SoccerEvent :public CharacterDamageMotion::Event
	{
	public:
		SoccerEvent(SoccerPlayer* pSoccer) :m_pSoccer(pSoccer){}

		void Update(float speed)
		{
			//���f���̍X�V�̂�
			m_pSoccer->m_Renderer.Update(speed);
			chr_func::CreateTransMatrix(
				m_pSoccer, 
				m_pSoccer->m_ModelSize,
				&m_pSoccer->m_Renderer.m_TransMatrix);
		}
		void Start()
		{
			//�Ђ�݃��[�V�������Z�b�g
			m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_Damage);
		}
		void End()
		{
			//�ʏ�X�e�[�g���Z�b�g
			m_pSoccer->SetState(
				SoccerState_PlayerControll_Move::GetPlayerControllMove(m_pSoccer)
				);
		}
	private:
		SoccerPlayer*  m_pSoccer;
	};

	//�_���[�W���[�V�����p�����[�^�[���쐬����
	CharacterDamageMotion::Params Param;

	Param.AllFrame = 40;
	Param.damage_vec = m_Damage_vec;
	Param.hitBack = 0.5f;
	Param.hitStopFrame = 0;
	Param.NoDamageFrame = 10;

	//�Ђ�݃N���X���쐬
	m_pDamageMotionClass = new CharacterDamageMotion(
		m_pSoccer,
		new SoccerEvent(m_pSoccer),
		new SoccerHitEvent(m_pSoccer),
		Param
		);

}
void SoccerState_SmallDamage::Execute(SoccerPlayer* t)
{
	m_pDamageMotionClass->Update();
}

void SoccerState_SmallDamage::Exit(SoccerPlayer* t)
{

}
SoccerState_DamageVanish::SoccerState_DamageVanish(
	SoccerPlayer* pSoccer,
	const Vector3& Damage_vec  //�_���[�W���󂯂�����
	) :
	m_pSoccer(pSoccer),
	m_Damage_vec(Damage_vec)
{

}
void SoccerState_DamageVanish::Enter(SoccerPlayer* s)
{
	//�L�����N�^���ʂЂ�݃N���X�̃T�b�J�[�ŗL�C�x���g�N���X
	class SoccerEvent :public CharacterDamageVanish::Event
	{
	public:
		SoccerEvent(SoccerPlayer* pSoccer) :m_pSoccer(pSoccer), m_Timer(0){}

		void FlyStart()
		{
			//������у��[�V�������Z�b�g
			m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_Blowing);
		}
		void Flying(const Matrix& Rotate)
		{
			//���f���̃A�j���[�V�����X�V
			m_pSoccer->m_Renderer.Update(1);

			//�ʒu�ɂ��ƂÂ��A���[���h�ϊ��s����v�Z
			chr_func::CreateTransMatrix(m_pSoccer, m_pSoccer->m_ModelSize, &m_pSoccer->m_Renderer.m_TransMatrix);

			//������т̉�]������
			m_pSoccer->m_Renderer.m_TransMatrix = Rotate*m_pSoccer->m_Renderer.m_TransMatrix;
		}
		void DownStart()
		{
			//�_�E�����[�V�������Z�b�g
			m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_Scrub);
		}

		void Downing()
		{
			m_pSoccer->m_Renderer.Update(1);

			//���[���h�ϊ��s����v�Z
			chr_func::CreateTransMatrix(m_pSoccer, m_pSoccer->m_ModelSize, &m_pSoccer->m_Renderer.m_TransMatrix);
		}

		void StandUpStart()
		{
			//������у��[�V�������Z�b�g
			m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_GetUp);
		}

		void StandUping()
		{
			//���f���̃A�j���[�V�����X�V
			m_pSoccer->m_Renderer.Update(1);

			//���[���h�ϊ��s����v�Z
			chr_func::CreateTransMatrix(m_pSoccer, m_pSoccer->m_ModelSize, &m_pSoccer->m_Renderer.m_TransMatrix);
		}

		void End()
		{
			//�ʏ�X�e�[�g���Z�b�g
			m_pSoccer->SetState(
				SoccerState_PlayerControll_Move::GetPlayerControllMove(m_pSoccer)
				);
		}
	private:
		SoccerPlayer*  m_pSoccer;
		int            m_Timer;
	};

	//�_���[�W���[�V�����p�����[�^�[���쐬����
	CharacterDamageVanish::Param Param;

	Param.rotate_speed = Vector3(0.0f, 0.0f, 0.0);
	Param.move = m_Damage_vec * 0.95f;
	Param.move.y = 0.2f;
	Param.standup_frame = 50;

	//�Ђ�݃N���X���쐬
	m_pDamageVanishClass = new CharacterDamageVanish(
		m_pSoccer,
		Param,
		new SoccerEvent(s),
		new SoccerHitEvent(s)
		);

}
void SoccerState_DamageVanish::Execute(SoccerPlayer* s)
{
	m_pDamageVanishClass->Update();
}

void SoccerState_DamageVanish::Exit(SoccerPlayer* s)
{

}
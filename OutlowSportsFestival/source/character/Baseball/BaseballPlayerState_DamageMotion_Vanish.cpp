#include "BaseballPlayerState_DamageMotion_Vanish.h"
#include "BaseballPlayerState.h"
#include "Baseball_HitEvent.h"
#include "../CharacterFunction.h"

#include "../../Effect/HitEffectObject.h"
#include "../../Effect/BlurImpact.h"


BaseballState_DamageVanish::BaseballState_DamageVanish(
	BaseballPlayer* pBaseball,
	const Vector3& Damage_vec  //�_���[�W���󂯂�����
	) :
	m_pBaseball(pBaseball),
	m_Damage_vec(Damage_vec)
{

}

void BaseballState_DamageVanish::Enter(BaseballPlayer* b)
{
	
	class BaseballEvent :public CharacterDamageVanish::Event
	{
	public:
		BaseballEvent(BaseballPlayer* pBaseball) :m_pBaseball(pBaseball){}

		void FlyStart()
		{
			//������у��[�V�������Z�b�g
			m_pBaseball->m_Renderer.SetMotion(baseball_player::_mb_Damage_Vanish_Fly);
		}

		void Flying(const Matrix& Rotate)
		{
			//���f���̃A�j���[�V�����X�V
			m_pBaseball->m_Renderer.Update(1);

			//�ʒu�ɂ��Ƃ����A���[���h�ϊ��s����v�Z
			chr_func::CreateTransMatrix(m_pBaseball, m_pBaseball->m_ModelSize, &m_pBaseball->m_Renderer.m_TransMatrix);

			//������т̉�]������
			m_pBaseball->m_Renderer.m_TransMatrix = Rotate*m_pBaseball->m_Renderer.m_TransMatrix;
		}

		void DownStart()
		{
			//�_�E�����[�V�������Z�b�g
			m_pBaseball->m_Renderer.SetMotion(baseball_player::_mb_Damage_Vanish_Down);
		}

		void Downing()
		{
			m_pBaseball->m_Renderer.Update(1);

			//���[���h�ϊ��s����v�Z
			chr_func::CreateTransMatrix(m_pBaseball, m_pBaseball->m_ModelSize, &m_pBaseball->m_Renderer.m_TransMatrix);
		}

		void StandUpStart()
		{
			//�N���オ�胂�[�V�������Z�b�g
			m_pBaseball->m_Renderer.SetMotion(baseball_player::_mb_Damage_Vanish_StandUp);
		}

		void StandUping()
		{
			//���f���̃A�j���[�V�����X�V
			m_pBaseball->m_Renderer.Update(1);

			//���[���h�ϊ��s����v�Z
			chr_func::CreateTransMatrix(m_pBaseball, m_pBaseball->m_ModelSize, &m_pBaseball->m_Renderer.m_TransMatrix);
		}

		void End()
		{
			//�ʏ�X�e�[�g���Z�b�g
			m_pBaseball->SetState(
				BaseballState_PlayerControll_Move::GetPlayerControllMove(m_pBaseball)
				);
		}


	private:
		BaseballPlayer*  m_pBaseball;
	};

	//�_���[�W���[�V�����p�����[�^�[���쐬����
	CharacterDamageVanish::Param Param;

	Param.rotate_speed = Vector3(0.0f, 0.0f, 0.0);
	Param.move = Vector3Normalize(m_Damage_vec)*0.6f;
	Param.move.y = 0.2f;

	Param.down_frame = 15;
	Param.down_muteki_frame = 15;
	Param.standup_frame = 50;
	Param.standup_muteki_frame = 10;


	//�Ђ�݃N���X���쐬
	m_pDamageVanishClass = new CharacterDamageVanish(
		m_pBaseball,
		Param,
		new BaseballEvent(b),
		new BaseballHitEvent(b)
		);

	//�q�b�g�G�t�F�N�g�쐬
	new HitEffectObject(
		m_pBaseball->m_Params.pos + Vector3(0, 3, 0),
		m_Damage_vec,
		0.05f,
		0.15f,
		Vector3(1.0f, 1.0f, 1.0f)
		);

	//�u���[�G�t�F�N�g
	new BlurImpactSphere(
		m_pBaseball->m_Params.pos + Vector3(0, 3, 0),
		25,
		10,
		30
		);
}

void BaseballState_DamageVanish::Execute(BaseballPlayer* b)
{
	m_pDamageVanishClass->Update();
}

void BaseballState_DamageVanish::Exit(BaseballPlayer* b)
{
	delete m_pDamageVanishClass;
}
#include "BaseballPlayerState_DamageMotion_Die.h"
#include "Baseball_HitEvent.h"
#include "../CharacterFunction.h"
#include "../../Effect/BlurImpact.h"

#include "../../Effect/HitEffectObject.h"
#include "../../Ball/Ball.h"

#include "../../Camera/Camera.h"

BaseballState_DamageMotion_Die::BaseballState_DamageMotion_Die(
	BaseballPlayer* pBaseball,
	const Vector3& Damage_vec  //�_���[�W���󂯂�����
	) :
	m_pBaseball(pBaseball),
	m_Damage_vec(Damage_vec)
{

}

void BaseballState_DamageMotion_Die::Enter(BaseballPlayer* b)
{
	//�L�����N�^���ʂЂ�݃N���X�̃e�j�X�ŗL�C�x���g�N���X
	class BaseballEvent :public CharacterDamageVanish::Event
	{
	public:
		BaseballEvent(BaseballPlayer* pBaseball) :m_pBaseball(pBaseball){}

		void FlyStart()
		{
			//������у��[�V�������Z�b�g
			m_pBaseball->m_Renderer.SetMotion(baseball_player::_mb_Damage_Vanish_Fly);
		}

        void Flying(const Matrix& Rotate, RATIO t)
		{
			//���f���̃A�j���[�V�����X�V
			m_pBaseball->m_Renderer.Update(t);

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
			//�������Ȃ�
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
			//�J�����ʂ��t���O��false��
			m_pBaseball->m_Params.camera_draw = false;
		}

	private:
		BaseballPlayer*  m_pBaseball;
	};

	//�_���[�W���[�V�����p�����[�^�[���쐬����
	CharacterDamageVanish::Param Param;

	Param.rotate_speed = Vector3(0.0f, 0.0f, 0.0);
	Param.move = Vector3Normalize(m_Damage_vec) * 0.7f;
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
		new DamageManager::HitEventBase()
		);

	COLORf EffectColor(CharacterBase::GetPlayerColor(b->m_PlayerInfo.number));


	//�q�b�g�G�t�F�N�g�쐬
	new HitEffectObject(
		m_pBaseball->m_Params.pos + Vector3(0, 3, 0) + Vector3Normalize(m_Damage_vec)*3.5f,
		m_Damage_vec,
		0.05f,
		0.15f,
		Vector3(EffectColor.r, EffectColor.g, EffectColor.b),
		5,
		50
		);

	//�u���[�G�t�F�N�g
	new BlurImpactSphere(
		m_pBaseball->m_Params.pos + Vector3(0, BallBase::UsualBallShotY, 0),
		20,
		50,
		15
		);

	//�J�����V���b�N
	DefCamera.SetShock(
		Vector2(1, 1)*0.22f,
		20
		);
}

void BaseballState_DamageMotion_Die::Execute(BaseballPlayer* b)
{
	m_pDamageVanishClass->Update();
}

void BaseballState_DamageMotion_Die::Exit(BaseballPlayer* b)
{
	delete m_pDamageVanishClass;
}
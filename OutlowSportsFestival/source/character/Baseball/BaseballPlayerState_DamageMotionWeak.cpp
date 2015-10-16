#include "BaseballPlayerState_DamageMotionWeak.h"
#include "Baseball_HitEvent.h"
#include "BaseballPlayerState.h"
#include "../CharacterFunction.h"
#include "../../Effect/HitEffectObject.h"
#include "../../Effect/BlurImpact.h"
BaseballState_DamageMotion_Weak::BaseballState_DamageMotion_Weak(
	BaseballPlayer* pBaseball,
	const Vector3& Damage_vec  //�_���[�W���󂯂�����
	) :
	m_pBaseball(pBaseball),
	m_Damage_vec(Damage_vec)
{

}

void BaseballState_DamageMotion_Weak::Enter(BaseballPlayer* t)
{
	//�L�����N�^���ʂЂ�݃N���X�̖싅�ŗL�C�x���g�N���X
	class BaseballEvent :public CharacterDamageMotion::Event
	{
	public:
		BaseballEvent(BaseballPlayer* pBaseball) :m_pBaseball(pBaseball){}

		void Update(float speed)
		{
			//���f���̍X�V�̂�
			m_pBaseball->m_Renderer.Update(speed);
			chr_func::CreateTransMatrix(m_pBaseball, m_pBaseball->m_ModelSize, &m_pBaseball->m_Renderer.m_TransMatrix);
		}
		void Start()
		{
			//�Ђ�݃��[�V�������Z�b�g
			m_pBaseball->m_Renderer.SetMotion(baseball_player::_mb_Damage_Weak);
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
	CharacterDamageMotion::Params Param;

	Param.AllFrame = 35;
	Param.damage_vec = m_Damage_vec;
	Param.hitBack = 0.5f;
	Param.hitStopFrame = 0;
	Param.NoDamageFrame = 10;

	//�Ђ�݃N���X���쐬
	m_pDamageMotionClass = new CharacterDamageMotion(
		m_pBaseball,
		new BaseballEvent(m_pBaseball),
		new BaseballHitEvent(m_pBaseball),
		Param
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
		10,
		15,
		30
		);
}

void BaseballState_DamageMotion_Weak::Execute(BaseballPlayer* t)
{
	m_pDamageMotionClass->Update();
}

void BaseballState_DamageMotion_Weak::Exit(BaseballPlayer* t)
{
	delete m_pDamageMotionClass;
}
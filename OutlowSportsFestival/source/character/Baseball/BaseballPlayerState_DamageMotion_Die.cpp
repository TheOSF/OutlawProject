#include "BaseballPlayerState_DamageMotion_Die.h"
#include "Baseball_HitEvent.h"
#include "../CharacterFunction.h"
#include "../../Effect/BlurImpact.h"
#include "../../Effect/HitEffectObject.h"
#include "../../Ball/Ball.h"
#include "../../Camera/Camera.h"
#include "../../Effect/EffectFactory.h"
#include "../../GameSystem/GameController.h"


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
            m_pBaseball->getNowModeModel()->SetMotion(baseball_player::_mb_Damage_Vanish_Fly);
		}

        void Flying(const Matrix& Rotate, RATIO t)
		{
			//���f���̃A�j���[�V�����X�V
            m_pBaseball->getNowModeModel()->Update(t);

			//�ʒu�ɂ��Ƃ����A���[���h�ϊ��s����v�Z
            chr_func::CreateTransMatrix(m_pBaseball, &m_pBaseball->getNowModeModel()->m_TransMatrix);

			//������т̉�]������
            m_pBaseball->getNowModeModel()->m_TransMatrix = Rotate*m_pBaseball->getNowModeModel()->m_TransMatrix;
		}

		void DownStart()
		{
			//�_�E�����[�V�������Z�b�g
            m_pBaseball->getNowModeModel()->SetMotion(baseball_player::_mb_Damage_Vanish_Down);
		}

		void Downing()
		{
            m_pBaseball->getNowModeModel()->Update(1);

			//���[���h�ϊ��s����v�Z
            chr_func::CreateTransMatrix(m_pBaseball, &m_pBaseball->getNowModeModel()->m_TransMatrix);
		}

		void StandUpStart()
		{
			//�������Ȃ�
		}

		void StandUping()
		{
			//���f���̃A�j���[�V�����X�V
            m_pBaseball->getNowModeModel()->Update(1);

			//���[���h�ϊ��s����v�Z
            chr_func::CreateTransMatrix(m_pBaseball, &m_pBaseball->getNowModeModel()->m_TransMatrix);
		}

		void End()
		{
			//�J�����ʂ��t���O��false��
            m_pBaseball->m_DrawObject.m_isDraw = false;
		}

		void HitWall()
		{
			//�ǂɓ����������[�V�������Z�b�g
            m_pBaseball->getNowModeModel()->SetMotion(baseball_player::_mb_Damage_Vanish_Wall);
		}

		void HitFloor()
		{
			//���ɓ����������[�V�������Z�b�g
            m_pBaseball->getNowModeModel()->SetMotion(baseball_player::_mb_Damage_Vanish_Bound);
		}

		void HitFloorAndStandUp()
		{
			//�����オ�胂�[�V�������Z�b�g
            m_pBaseball->getNowModeModel()->SetMotion(baseball_player::_mb_Damage_Vanish_StandUp);
		}

		void HitWallUpdate()
		{
			//���f���̃A�j���[�V�����X�V
            m_pBaseball->getNowModeModel()->Update(1);

			//���[���h�ϊ��s����v�Z
            chr_func::CreateTransMatrix(m_pBaseball, &m_pBaseball->getNowModeModel()->m_TransMatrix);
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
	Param.standup_frame = 50;


	//�Ђ�݃N���X���쐬
	m_pDamageVanishClass = new CharacterDamageVanish(
		m_pBaseball,
		Param,
		new BaseballEvent(b),
		new DamageManager::HitEventBase()
		);

	//���S�G�t�F�N�g
	EffectFactory::DieEffect(
		b,
		m_Damage_vec
		);

	//�R���g���[����U��
    chr_func::SetControllerShock(
        m_pBaseball,
        0.8f,
        0.5f
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
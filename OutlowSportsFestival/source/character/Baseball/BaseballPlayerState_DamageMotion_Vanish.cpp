#include "BaseballPlayerState_DamageMotion_Vanish.h"
#include "BaseballPlayerState.h"
#include "Baseball_HitEvent.h"
#include "../CharacterFunction.h"
#include "../../Effect/EffectFactory.h"
#include "../../GameSystem/GameController.h"
#include "BaseballState_PlayerControll_Evasion.h"

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
            m_pBaseball->getNowModeModel()->SetMotion(baseball_player::_mb_Damage_Vanish_Fly);
		}

        void Flying(const Matrix& Rotate, RATIO t)
		{
			//���f���̃A�j���[�V�����X�V
            m_pBaseball->getNowModeModel()->Update(1);

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
			//�N���オ�胂�[�V�������Z�b�g
            m_pBaseball->getNowModeModel()->SetMotion(baseball_player::_mb_Damage_Vanish_StandUp);
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
			//�ʏ�X�e�[�g���Z�b�g
			m_pBaseball->SetState(
				BaseballState_PlayerControll_Move::GetPlayerControllMove(m_pBaseball)
				);
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

		void CanActionUpdate()
		{
			//�s�����򂪉\�ȂƂ��ɌĂ΂��
            if (m_pBaseball->m_PlayerInfo.player_type == PlayerType::_Player)
            {
                if (controller::GetPush(controller::button::batu, m_pBaseball->m_PlayerInfo.number))
                {
                    m_pBaseball->SetState(new BaseballState_Rolling());
                }
            }
            else
            {
                if (frand() < 0.02f)
                {
                    m_pBaseball->SetState(new BaseballState_Rolling(Vector3Rand() - Vector3Normalize(m_pBaseball->m_Params.pos)));
                }
            }
		}

	private:
		BaseballPlayer*  m_pBaseball;
	};

	//�_���[�W���[�V�����p�����[�^�[���쐬����
	CharacterDamageVanish::Param Param;

	Param.rotate_speed = Vector3(0.0f, 0.0f, 0.0);
	Param.move = m_Damage_vec;

	Param.down_frame = 15;
	Param.standup_frame = 50;


	//�Ђ�݃N���X���쐬
	m_pDamageVanishClass = new CharacterDamageVanish(
		m_pBaseball,
		Param,
		new BaseballEvent(b),
		new BaseballHitEvent(b)
		);

    //�G�t�F�N�g
    EffectFactory::VanishEffect(
        m_pBaseball,
        m_Damage_vec
        );

	//�R���g���[����U��
    chr_func::SetControllerShock(
        m_pBaseball,
        0.5f,
        0.15f
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

#include "BaseballPlayerState.h"
#include "Baseball_HitEvent.h"
#include "BaseballState_PlayerControll_ShotAttack_P.h"

#include "Computer/BaseballPlayerState_ComMove.h"

#include "../../GameSystem/GameController.h"
#include "../CharacterFunction.h"
#include "../CharacterManager.h"
#include "../CharacterEvasionClass.h"

#include "../../Ball/UsualBall.h"
#include "../../Ball/MilderHoming.h"
#include "../../Damage/Damage.h"

#include "Sound/Sound.h"
#include "../../GameSystem/GameController.h"

//***************************************
//�@������(����)
//***************************************
class ShotAttackEvent_P;
//�@�R���X�g���N�^
BaseballState_PlayerControll_ShotAttack_P::BaseballState_PlayerControll_ShotAttack_P() :
pTargetEnemy(nullptr), m_pShotAttackClass_P(nullptr)
{

}


//�@�X�e�[�g�J�n
void BaseballState_PlayerControll_ShotAttack_P::Enter(BaseballPlayer* b)
{
	// ������(�s�b�`���[)�N���X�쐬
	m_pShotAttackClass_P = this->CreateShotAttackClass_P(b);

}


//�@�X�e�[�g���s
void BaseballState_PlayerControll_ShotAttack_P::Execute(BaseballPlayer* b){

	//�@Player�Ȃ炱����
	if (b->m_PlayerInfo.player_type == PlayerType::_Player)
	{
		// �X�e�B�b�N�̒l�Z�b�g
		m_pShotAttackClass_P->SetStickValue(
			controller::GetStickValue(controller::stick::left, b->m_PlayerInfo.number));
		// �X�V
		if (m_pShotAttackClass_P->Update() == false)
		{
			return;
		}
	}
	else
	{

		// �X�V
		if (m_pShotAttackClass_P->Update() == false)
		{
			return;
		}


	}


}

//�@�X�e�[�g�I��
void BaseballState_PlayerControll_ShotAttack_P::Exit(BaseballPlayer* b){
	delete m_pShotAttackClass_P;
}


//�@�������N���X
CharacterShotAttack* BaseballState_PlayerControll_ShotAttack_P::CreateShotAttackClass_P(BaseballPlayer* b){
	class ShotAttackEvent_P :public CharacterShotAttack::Event{
		BaseballPlayer* m_pBaseball;//�@�싅
		MilderHoming* mild;
	public:
		//�@�{�[��
		BallBase::Params param;
		//�@�^�[�Q�b�g
		Vector3 target;
	public:
		//�@�R���X�g���N�^
		ShotAttackEvent_P(BaseballPlayer* pBaseball) :target(0, 0, 0),
			m_pBaseball(pBaseball){}
		//�@�X�V
		void Update()override{

			//�@���f���X�V
			m_pBaseball->ModelUpdate(1);

			// �]���s��X�V
			chr_func::CreateTransMatrix(
				m_pBaseball,
                &m_pBaseball->getNowModeModel()->m_TransMatrix);
		}
	public:
		// �_���[�W����J�n & �{�[������
		void Shot()
		{
			//�@�������U��(param�v�Z)
			BallBase::Params param;

			chr_func::GetFront(m_pBaseball, &param.move);

            param.pos = m_pBaseball->getNowModeModel()->GetWorldBonePos(40);
			param.pos.y = UsualBall::UsualBallShotY;

			param.pParent = m_pBaseball;
			param.scale = 1.0f;
			param.type = BallBase::Type::_Milder;

			//����
            new UsualBall(
                param,
                DamageBase::Type::_WeekDamage,
                5.0f,
                MilderHoming::GetMilderHomingMove()
                );

            //�R���g���[����U��
            chr_func::SetControllerShock(
                m_pBaseball,
                0.5f,
                0.15f
                );

            //�X�L���Q�[�W���Z
            chr_func::AddSkillGauge(m_pBaseball, UsualBall::AddSkillValueRatio);
		}

		//�@�������U���J�n
		void AttackStart()override{
			//�@�����[�V����
			m_pBaseball->SetMotion(baseball_player::_mb_Atk2_P);
			//�@���ʉ�
			Sound::Play(Sound::Swing3);
		}

		void AttackEnd()

		{	//�U���I�����ɒʏ�ړ����[�h�ɖ߂�
			if (m_pBaseball->m_PlayerInfo.player_type == PlayerType::_Player)
			{
				//�@�v���C���[
				m_pBaseball->SetState(new BaseballState_PlayerControll_Move());
			}
			else
			{
				//�@�R���s���[�^�[
				m_pBaseball->SetState(new BaseballPlayerState_ComMove());
			}
		}
	};

	CharacterShotAttack::AttackParams atk;

	atk.AllFrame = 40;
	atk.MaxTurnRadian = PI / 4;
	atk.MoveDownSpeed = 0.8f;
	atk.ShotFrame = 15;

	return m_pShotAttackClass_P = new CharacterShotAttack(
		b,
		new ShotAttackEvent_P(b),
		atk,
		new  BaseballHitEvent(b)
		);
}

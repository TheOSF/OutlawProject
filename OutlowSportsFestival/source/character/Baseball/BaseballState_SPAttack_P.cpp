#include "BaseballState_SPAttack_P.h"
#include "BaseballPlayerState.h"
#include "Baseball_HitEvent.h"
#include "../../Ball/Baseball_SpAtk_Ball.h"

#include "../../Camera/Camera.h"
#include "../../Collision/Collision.h"
#include "../../GameSystem/GameController.h"
#include "../CharacterFunction.h"
#include "../CharacterManager.h"
#include "../CharacterEvasionClass.h"

//�@�R���X�g���N�^
BaseballState_SPAttack_P::BaseballState_SPAttack_P() :m_pSpAttack_P(nullptr)
{

}


//�@�X�e�[�g�J�n
void BaseballState_SPAttack_P::Enter(BaseballPlayer* b)
{
	// ������(�o�b�^�[)�N���X�쐬
	m_pSpAttack_P = this->CreateSpAttack_P(b);
}


//�@�X�e�[�g���s
void BaseballState_SPAttack_P::Execute(BaseballPlayer* b){
	// �X�e�B�b�N�̒l�Z�b�g
	m_pSpAttack_P->SetStickValue(
		controller::GetStickValue(controller::stick::left, b->m_PlayerInfo.number));

	// �X�V
	if (m_pSpAttack_P->Update() == false)
	{
		return;
	}
}

//�@�X�e�[�g�I��
void BaseballState_SPAttack_P::Exit(BaseballPlayer* b){
	delete m_pSpAttack_P;
}


//�@�������N���X
CharacterShotAttack* BaseballState_SPAttack_P::CreateSpAttack_P(BaseballPlayer* b){
	class SpAttackEvent :public CharacterShotAttack::Event{
		BaseballPlayer* m_pBaseball;//�@�싅
	public:
		//�@�R���X�g���N�^
		SpAttackEvent(BaseballPlayer* pBaseball) :
			m_pBaseball(pBaseball){}
		//�@�X�V
		void Update()override{
			//�@���f���X�V
			m_pBaseball->m_Renderer.Update(1.0f);

			// �]���s��X�V
			chr_func::CreateTransMatrix(
				m_pBaseball,
				0.05f,
				&m_pBaseball->m_Renderer.m_TransMatrix);
		}

		// �_���[�W����J�n & �{�[������
		void Shot()
		{
			BallBase::Params param;

			chr_func::GetFront(m_pBaseball, &param.move);
			param.move *= 10.0f;
			param.pos = m_pBaseball->m_Params.pos;
			param.pos.y = BallBase::UsualBallShotY;
			param.pParent = m_pBaseball;
			param.type = BallBase::Type::_BaseballSpecialAtk;

			new Baseball_SpAtk_Ball(param, DamageBase::Type::_VanishDamage, 1);
		}

		//�@�������U���J�n
		void AttackStart()override{
			//�@�����[�V����
			m_pBaseball->m_Renderer.SetMotion(baseball_player::_mb_Shot);
		}

		void AttackEnd()
		{
			//�U���I�����ɒʏ�ړ����[�h�ɖ߂�
			m_pBaseball->SetState(new BaseballState_PlayerControll_Move());
		}
	};

	CharacterShotAttack::AttackParams atk;

	atk.AllFrame = 40;
	atk.AttackPower = 20;
	atk.MaxTurnRadian = PI / 4;
	atk.MoveDownSpeed = 1.2f;
	atk.ShotFrame = 15;

	return m_pSpAttack_P = new CharacterShotAttack(
		b,
		new SpAttackEvent(b),
		atk,
		new  BaseballHitEvent(b)
		);
}

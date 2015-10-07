#include "BaseballPlayerState.h"
#include "Baseball_HitEvent.h"
#include "BaseballState_PlayerControll_ShotAttack_B.h"
#include "../../GameSystem/GameController.h"
#include "../CharacterFunction.h"
#include "../CharacterManager.h"
#include "../CharacterEvasionClass.h"

#include "../../Ball/UsualBall.h"
#include "../../Ball/MilderHoming.h"
#include "../../Damage/Damage.h"
#include "../CharacterCounterClass.h"

//***************************************
//�@������(�o�b�^�[)
//***************************************

//�@�R���X�g���N�^
BaseballState_PlayerControll_ShotAttack_B::BaseballState_PlayerControll_ShotAttack_B() :m_pShotAttackClass_B(nullptr){

}


//�@�X�e�[�g�J�n
void BaseballState_PlayerControll_ShotAttack_B::Enter(BaseballPlayer* b){
	// ������(�o�b�^�[)�N���X�쐬
	m_pShotAttackClass_B = this->CreateShotAttackClass_B(b);
}


//�@�X�e�[�g���s
void BaseballState_PlayerControll_ShotAttack_B::Execute(BaseballPlayer* b){
	// �X�e�B�b�N�̒l�Z�b�g
	m_pShotAttackClass_B->SetStickValue(
		controller::GetStickValue(controller::stick::left, b->m_PlayerInfo.number));

	// �X�V
	if (m_pShotAttackClass_B->Update() == false)
	{
		return;
	}
}

//�@�X�e�[�g�I��
void BaseballState_PlayerControll_ShotAttack_B::Exit(BaseballPlayer* b){
	delete m_pShotAttackClass_B;
}




//�@�������N���X
CharacterShotAttack* BaseballState_PlayerControll_ShotAttack_B::CreateShotAttackClass_B(BaseballPlayer* b){
	class ShotAttackEvent_B :public CharacterShotAttack::Event{
		BaseballPlayer* m_pBaseball;//�@�싅
		MilderHoming* mild;
	public:
		//�@�R���X�g���N�^
		ShotAttackEvent_B(BaseballPlayer* pBaseball) :
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
			param.move *= 0.7f;
			param.pos = m_pBaseball->m_Params.pos;
			param.pos.y = BallBase::UsualBallShotY;
			param.pParent = m_pBaseball ;
			param.type = BallBase::Type::_Usual;

			new UsualBall(param, DamageBase::Type::_WeekDamage, 1);
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
	atk.AttackPower = 8;
	atk.MaxTurnRadian = PI / 4;
	atk.MoveDownSpeed = 0.8f;
	atk.ShotFrame = 15;

	return m_pShotAttackClass_B = new CharacterShotAttack(
		b,
		new ShotAttackEvent_B(b),
		atk,
		new  BaseballHitEvent(b)
		);
}

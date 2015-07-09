
#include "BaseballPlayerState.h"
#include "Baseball_HitEvent.h"
#include "../../GameSystem/GameController.h"
#include "../CharacterFunction.h"
#include "../CharacterManager.h"
#include "../CharacterEvasionClass.h"

#include "../../Ball/UsualBall.h"
#include "../../Ball/MilderHoming.h"
#include "../../Damage/Damage.h"
#include "../CharacterCounterClass.h"


//***************************************
//�@������(����)
//***************************************
class ShotAttackEvent_P;
//�@�R���X�g���N�^
BaseballState_PlayerControll_ShotAttack_P::BaseballState_PlayerControll_ShotAttack_P() :m_pShotAttackClass_P(nullptr){

}


//�@�X�e�[�g�J�n
void BaseballState_PlayerControll_ShotAttack_P::Enter(BaseballPlayer* b){
	// ����N���X�쐬
	m_pShotAttackClass_P = this->CreateShotAttackClass_P(b);
}


//�@�X�e�[�g���s
void BaseballState_PlayerControll_ShotAttack_P::Execute(BaseballPlayer* b){
	// �X�e�B�b�N�̒l�Z�b�g
	m_pShotAttackClass_P->SetStickValue(
		controller::GetStickValue(controller::stick::left, b->m_PlayerInfo.number));

	// �X�V
	if (m_pShotAttackClass_P->Update() == false)
	{
		return;
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
	public:
		//�@�{�[��
		BallBase::Params param;
		//�@�^�[�Q�b�g
		Vector3 target;
	public:
		//�@�R���X�g���N�^
		ShotAttackEvent_P(BaseballPlayer* pBaseball) :
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
	public:
		// �_���[�W����J�n & �{�[������
		void Shot()
		{
			//�@�������U��(param�v�Z)
			param = m_pBaseball->BaseballShot(m_pBaseball, param);
			//�@�^�[�Q�b�g����
			param = m_pBaseball->TargetDecision(param, target);

			//����
			new MilderHoming(m_pBaseball, param, target, DamageBase::Type::_WeekDamage, 1);
		}

		//�@�������U���J�n
		void AttackStart()override{
			//�@�����[�V����
			m_pBaseball->m_Renderer.SetMotion(baseball_player::_mt_Shot);
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

	return m_pShotAttackClass_P = new CharacterShotAttack(
		b,
		new ShotAttackEvent_P(b),
		atk,
		new  BaseballHitEvent(b)
		);
}

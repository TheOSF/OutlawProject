
#include "BaseballPlayerState.h"
#include "BaseballState_PlayerControll_Evasion.h"
#include "Baseball_HitEvent.h"
#include "../../GameSystem/GameController.h"
#include "../CharacterFunction.h"
#include "../CharacterManager.h"
#include "../CharacterEvasionClass.h"

#include "../../Damage/Damage.h"
#include "../CharacterCounterClass.h"


//***************************************
//�@���
//***************************************

//�@�R���X�g���N�^
BaseballState_PlayerControll_Evasion::BaseballState_PlayerControll_Evasion(float speed) :
m_pEvasionClass(nullptr)
{
	roolspeed = speed;
}

//�@�X�e�[�g�J�n
void BaseballState_PlayerControll_Evasion::Enter(BaseballPlayer* b){
	// ����N���X�쐬
	m_pEvasionClass = this->CreateEvasionClass(b);
}


//�@�X�e�[�g���s
void BaseballState_PlayerControll_Evasion::Execute(BaseballPlayer* b){
	// �X�e�B�b�N�̒l�Z�b�g
	m_pEvasionClass->SetStickValue(
		controller::GetStickValue(controller::stick::left, b->m_PlayerInfo.number));

	// �X�V
	if (m_pEvasionClass->Update() == false)
	{
		return;
	}
}

//�@�X�e�[�g�I��
void BaseballState_PlayerControll_Evasion::Exit(BaseballPlayer* b){
	delete m_pEvasionClass;
}


//�@����N���X
CharacterEvasion* BaseballState_PlayerControll_Evasion::CreateEvasionClass(BaseballPlayer* b){
	class EvasionEvent : public CharacterEvasion::Event{
		BaseballPlayer* m_pBaseball;//�@�싅
	public:
		//�@�R���X�g���N�^
		EvasionEvent(BaseballPlayer* pBaseball) :
			m_pBaseball(pBaseball){}

		// �X�V
		void Update()override
		{
			// ���f���X�V
			m_pBaseball->m_Renderer.Update(1.0f);

			// �]���s��X�V
			chr_func::CreateTransMatrix(
				m_pBaseball,
				0.05f,
				&m_pBaseball->m_Renderer.m_TransMatrix);
		}


		// ����s���J�n
		void EvasionStart()override
		{
			m_pBaseball->m_Renderer.SetMotion(baseball_player::_mt_Evasion);
		}


		// ����s���I��
		void EvasionEnd()override
		{
			// �ʏ�ړ���
			m_pBaseball->m_Renderer.SetMotion(baseball_player::_mt_Stand);
			m_pBaseball->SetState(new BaseballState_PlayerControll_Move());
		}
	};

	// ����p�����[�^�ݒ�
	CharacterEvasion::EvasionParams params;
	params.AllFrame = 35;         // �S35�t���[��
	params.MaxTurnRadian = PI / 4;    // 45��
	params.MoveDownSpeed = 0.2f;      // ��������
	params.MoveSpeed = roolspeed;    // �ړ��X�s�[�h
	params.NoDamageStartFrame = 3;          // �J�n3�t���[���Ŗ��G�J�n
	params.NoDamageEndFrame = 20;       // �J�n20�t���[���Ŗ��G�I��

	// �쐬
	return new CharacterEvasion(
		b,
		new EvasionEvent(b),
		params
		);
}
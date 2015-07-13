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
//�@�ړ�
//***************************************

//�@�X�e�[�g�J�n
void BaseballState_PlayerControll_Move::Enter(BaseballPlayer* b)
{
	//�@�ړ��C�x���g�N���X(���[�J���N���X)
	class BaseballMoveEvent :public CharacterUsualMove::MoveEvent
	{
		BaseballPlayer* m_pBaseball;
	public:
		BaseballMoveEvent(BaseballPlayer* pBaseball) :
			m_pBaseball(pBaseball){}

		//�@�A�j���[�V�����X�V
		void Update(bool isRun, RATIO speed_ratio){
			m_pBaseball->m_Renderer.Update(1);
		}
		//�@����n�߂Ƀ��[�V�����Z�b�g
		void RunStart(){
			m_pBaseball->m_Renderer.SetMotion(baseball_player::_mt_Run);
		}
		//�@�����n�߂Ƀ��[�V�����Z�b�g
		void StandStart(){
			m_pBaseball->m_Renderer.SetMotion(baseball_player::_mt_Stand);
		}
	};

	//�@�_���[�W�C�x���g�N���X�̍쐬
	class BaseballDamageHitEvent :public DamageManager::HitEventBase
	{
		BaseballPlayer* m_pTennis;
	public:
		BaseballDamageHitEvent(BaseballPlayer* pTennis) :
			m_pTennis(pTennis){}

		//�����������ɂ��̃_���[�W�̎�ނ���A���ꂼ��̃X�e�[�g�ɔh��������
		bool Hit(DamageBase* pDmg)
		{
			bool ret = true;

			switch (pDmg->type)
			{
			case DamageBase::Type::_WeekDamage:
				//m_pTennis->SetState();	�X�e�[�g���ł��Ă��Ȃ�����
				break;
			case DamageBase::Type::_VanishDamage:
				//m_pTennis->SetState();
				break;
			case DamageBase::Type::_UpDamage:
				//m_pTennis->SetState();
				break;
			default:
				ret = false;
				break;
			}

			return ret;
		}
	};

	//�@�ړ��p�����[�^����
	CharacterUsualMove::Params p;

	p.Acceleration = 0.2f;
	p.MaxSpeed = 0.2f;
	p.TurnSpeed = 0.3f;
	p.DownSpeed = 0.2f;

	//�@�ړ��N���X�̍쐬
	m_pMoveClass = new CharacterUsualMove(
		b,
		p,
		new BaseballMoveEvent(b),
		new BaseballDamageHitEvent(b)
		);
}


//�@�X�e�[�g���s
void BaseballState_PlayerControll_Move::Execute(BaseballPlayer* b){

	//�@�X�e�B�b�N�̒l���擾
	Vector2 st = controller::GetStickValue(controller::stick::left, b->m_PlayerInfo.number);

	//�@�X�e�B�b�N�̒l�Z�b�g
	m_pMoveClass->SetStickValue(st);

	//�@�X�V
	m_pMoveClass->Update();

	//�@���f���̃��[���h�ϊ��s����X�V
	chr_func::CreateTransMatrix(b, 0.05f, &b->m_Renderer.m_TransMatrix);

	//�@�؂�ւ�

	SetBatterFlg(b);
	//�@���s�p�^�[��
	if (batterflg){
		//�@�o�b�^�[��
		Batter(b);
	}
	else{
		//�@���莞
		Pitcher(b);
	}

}

//�@�X�e�[�g�̏I��
void BaseballState_PlayerControll_Move::Exit(BaseballPlayer* b){
	delete m_pMoveClass;
}

//�@�o�b�^�[��
void BaseballState_PlayerControll_Move::Batter(BaseballPlayer* b){
	//�@�������U��[��]
	if (controller::GetTRG(controller::button::sankaku, b->m_PlayerInfo.number)){
		b->SetState(new BaseballState_PlayerControll_ShotAttack_B());
		return;
	}
	//�@����s��[�~]
	if (controller::GetTRG(controller::button::batu, b->m_PlayerInfo.number)){
		b->SetState(new BaseballState_PlayerControll_Evasion(0.25f));
		return;
	}
}

//�@���莞
void  BaseballState_PlayerControll_Move::Pitcher(BaseballPlayer* b){
	//�@�������U��[��]
	if (controller::GetTRG(controller::button::sankaku, b->m_PlayerInfo.number)){
		b->SetState(new BaseballState_PlayerControll_ShotAttack_P());
		return;
	}
	//�@����s��[�~]
	if (controller::GetTRG(controller::button::batu, b->m_PlayerInfo.number)){
		b->SetState(new BaseballState_PlayerControll_Evasion(0.45f));
		return;
	}
}

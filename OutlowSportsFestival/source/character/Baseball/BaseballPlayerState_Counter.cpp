#include "BaseballPlayerState_Counter.h"
#include "BaseballPlayerState.h"
#include "../../GameSystem/GameController.h"
#include "../CharacterFunction.h"
#include "../../Ball/UsualBall.h"
#include "../../Damage/Damage.h"
#include "Baseball_HitEvent.h"

#include "../CharacterCounterClass.h"
#include "../CharacterMoveClass.h"
#include "../CharacterDefaultCounterClass.h"


//***************************************************
//		�v���C���[����� �J�E���^�[�N���X
//***************************************************

BaseballState_PlayerControll_Counter::BaseballState_PlayerControll_Counter(int size)
{
	areasize = size;
}

// �X�e�[�g�J�n
void BaseballState_PlayerControll_Counter::Enter(BaseballPlayer* b)
{
	//�J�E���^�[�C�x���g�N���X
	class CounterEvent :public CharacterDefaultCounter::Event
	{
		BaseballPlayer* const m_pBaseball;
	public:
		CounterEvent(BaseballPlayer* pBaseball) :
			m_pBaseball(pBaseball)
		{

		}

		//�\���J�n
		void Pose()
		{
			m_pBaseball->m_Renderer.SetMotion(baseball_player::_mb_CounterPose);
		}

		//�{�[���ֈړ��J�n
		void Move(BallBase* pCounterBall)
		{
			//�{�[���̈ʒu�ɂ���ă��[�V��������
			if (chr_func::isRight(m_pBaseball, pCounterBall->m_Params.pos))
			{
				m_pBaseball->m_Renderer.SetMotion(baseball_player::_mb_CounterRight);
			}
			else
			{
				m_pBaseball->m_Renderer.SetMotion(baseball_player::_mb_CounterLeft);
			}
		}

		//�ł�
		void Shot(BallBase* pCounterBall)
		{
			chr_func::ResetMove(m_pBaseball);
			chr_func::AddMoveFront(m_pBaseball, -0.15f, 1000);
		}

		//�ł����s
		void ShotFaild()
		{
			m_pBaseball->m_Renderer.SetMotion(baseball_player::_mb_LosePose);
		}

		//�I��
		void End()
		{
			//�ʏ�ړ��N���X��
			m_pBaseball->SetState(BaseballState_PlayerControll_Move::GetPlayerControllMove(m_pBaseball));
		}
	};

	//�J�E���^�[�p�����[�^�ݒ�
	CharacterDefaultCounter::Param param;

	param.AfterShotFrame = 15;
	param.CanCounterFrame = 30;
	param.CatchAriaSize = areasize;
	param.ControllRadian = D3DXToRadian(33);
	param.FailedFrame = 20;
	param.PoseFrame = 10;
	param.ShotFrame = 6;
	param.BallSpeed = 0.85f;

	// �J�E���^�[�N���X�쐬
	m_pCounter = new CharacterDefaultCounter(
		b,
		param,
		new CounterEvent(b),
		new BaseballHitEvent(b)
		);
}


// �X�e�[�g���s
void BaseballState_PlayerControll_Counter::Execute(BaseballPlayer* b)
{
	m_pCounter->SetStickValue(controller::GetStickValue(controller::stick::left, b->m_PlayerInfo.number));
	//�X�V
	m_pCounter->Update();

	//���f���̃��[���h�ϊ��s����X�V
	chr_func::CreateTransMatrix(b, 0.05f, &b->m_Renderer.m_TransMatrix);

	//���f���X�V
	b->m_Renderer.Update(1);
}


// �X�e�[�g�I��
void BaseballState_PlayerControll_Counter::Exit(BaseballPlayer* b)
{
	delete m_pCounter;
}

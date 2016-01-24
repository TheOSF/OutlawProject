#include "BaseballPlayerState_Counter.h"
#include "BaseballPlayerState.h"
#include "../../GameSystem/GameController.h"
#include "../CharacterFunction.h"
#include "../../Ball/UsualBall.h"
#include "../../Damage/Damage.h"
#include "Baseball_HitEvent.h"

#include "../CharacterMoveClass.h"
#include "../CharacterDefaultCounterClass.h"


//***************************************************
//		�v���C���[����� �J�E���^�[�N���X
//***************************************************

BaseballState_PlayerControll_Counter::BaseballState_PlayerControll_Counter()
{

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
			if (m_pBaseball->getBatterFlg())
			{
				m_pBaseball->m_Renderer.SetMotion(baseball_player::_mb_CounterPose_B);
			}
			else
			{
				m_pBaseball->m_Renderer.SetMotion(baseball_player::_mb_CounterPose_P);
			}
		}

		//�{�[���ֈړ��J�n
		void Move(BallBase* pCounterBall)
		{
			if (m_pBaseball->getBatterFlg())
			{
                m_pBaseball->m_Renderer.SetMotion(baseball_player::_mb_CounterRight_B);
			}
			else
			{
				//�{�[���̈ʒu�ɂ���ă��[�V��������
				if (chr_func::isRight(m_pBaseball, pCounterBall->m_Params.pos))
				{
					m_pBaseball->m_Renderer.SetMotion(baseball_player::_mb_CounterRight_P);
				}
				else
				{
					m_pBaseball->m_Renderer.SetMotion(baseball_player::_mb_CounterLeft_P);
				}
			}
		}

		//�ł�
		void Shot(BallBase* pCounterBall)
		{

		}

		//�ł����s
		void ShotFaild()
		{
			if (m_pBaseball->getBatterFlg())
			{
				m_pBaseball->m_Renderer.SetMotion(baseball_player::_mb_Stand_B);
			}
			else
			{
				m_pBaseball->m_Renderer.SetMotion(baseball_player::_mb_Stand_P);
			}
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

    if (b->getBatterFlg())
    {
        param.AfterShotFrame = 15;
        param.CanCounterFrame = 30;
        param.CatchAriaSize = areasize;
        param.ControllRadian = D3DXToRadian(33);
        param.FailedFrame = 20;
        param.PoseFrame = 24;
        param.ShotFrame = 6;
        param.BallSpeed = 0.85f;
    }
    else
    {
        param.AfterShotFrame = 15;
        param.CanCounterFrame = 30;
        param.CatchAriaSize = areasize;
        param.ControllRadian = D3DXToRadian(33);
        param.FailedFrame = 20;
        param.PoseFrame = 24;
        param.ShotFrame = 6;
        param.BallSpeed = 0.85f;
    }

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
	//�X�V
	m_pCounter->Update();

	//���f���̃��[���h�ϊ��s����X�V
	chr_func::CreateTransMatrix(b, &b->m_Renderer.m_TransMatrix);

	//���f���X�V
	b->m_Renderer.Update(1);
}


// �X�e�[�g�I��
void BaseballState_PlayerControll_Counter::Exit(BaseballPlayer* b)
{
	delete m_pCounter;
}

#include "SoccerComputerCounter.h"
#include "../../CharacterFunction.h"
#include "SoccerComputerMove.h"
#include "../SoccerPlayerState.h"
#include "../SoccerHitEvent.h"

//-------------�J�E���^�[�X�e�[�g�N���X-------------
void SoccerState_ComputerControll_Counter::Enter(SoccerPlayer* s)
{
	//�J�E���^�[�C�x���g�N���X
	class CounterEvent :public CharacterDefaultCounter::Event
	{
		SoccerPlayer* const m_pSoccer;
	public:
		CounterEvent(SoccerPlayer* pTennis) :
			m_pSoccer(pTennis)
		{

		}

		//�\���J�n
		void Pose()
		{
			m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_Counter);
		}

		//�{�[���ֈړ��J�n
		void Move(BallBase* pCounterBall)
		{
			//�{�[���̈ʒu�ɂ���ă��[�V��������
			if (chr_func::isRight(m_pSoccer, pCounterBall->m_Params.pos))
			{
				m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_Shot);
			}
			else
			{
				m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_Shot);
			}
		}

		//�ł�
		void Shot(BallBase* pCounterBall)
		{
			chr_func::ResetMove(m_pSoccer);
			chr_func::AddMoveFront(m_pSoccer, -0.15f, 1000);

		}

		//�ł����s
		void ShotFaild()
		{
			m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_Crap);
		}

		//�I��
		void End()
		{
			//�ʏ�ړ��N���X��
			m_pSoccer->SetState(SoccerState_PlayerControll_Move::GetPlayerControllMove(m_pSoccer));
		}
	};

	//�J�E���^�[�p�����[�^�ݒ�
	CharacterDefaultCounter::Param param;

	param.AfterShotFrame = 15;
	param.CanCounterFrame = 30;
	param.CatchAriaSize = 8;
	param.ControllRadian = D3DXToRadian(33);
	param.FailedFrame = 20;
	param.PoseFrame = 60;
	param.ShotFrame = 6;
	param.BallSpeed = 0.85f;

	// �J�E���^�[�N���X�쐬
	m_pCounter = new CharacterDefaultCounter(
		s,
		param,
		new CounterEvent(s),
		new SoccerHitEvent(s)
		);
}
void SoccerState_ComputerControll_Counter::Execute(SoccerPlayer* s)
{
	m_pCounter->SetStickValue(controller::GetStickValue(controller::stick::left, s->m_PlayerInfo.number));
	//�X�V
	m_pCounter->Update();

	//���f���̃��[���h�ϊ��s����X�V
	chr_func::CreateTransMatrix(s, 0.05f, &s->m_Renderer.m_TransMatrix);

	//���f���X�V
	s->m_Renderer.Update(1);
}
void SoccerState_ComputerControll_Counter::Exit(SoccerPlayer* s)
{
	delete m_pCounter;
}
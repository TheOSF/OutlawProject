#include "../Computer/SoccerComputerMove.h"
#include "../../CharacterFunction.h"
#include "../../CharacterCounterClass.h"
#include "../../CharacterMoveClass.h"
#include "../../CharacterShotAttackClass.h"
#include "../../CharacterManager.h"
#include "../SoccerHitEvent.h"

//�X�e�[�g�J�n
void SoccerState_ComputerControll_Move::Enter(SoccerPlayer* s)
{
	//�ړ��C�x���g�N���X
	class SoccerMoveEvent :public CharacterUsualMove::MoveEvent
	{
		SoccerPlayer* m_cSoccer;
	public:
		SoccerMoveEvent(SoccerPlayer* cSoccer) :
			m_cSoccer(cSoccer){}

		//�A�j���[�V�����̍X�V
		void Update(bool isRun, RATIO speed_ratio)
		{
			m_cSoccer->m_Renderer.Update(1);
		}
		//����n�߂Ƀ��[�V�������Z�b�g
		void RunStart()
		{
			m_cSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_Run);
		}
		//�����͂��߂Ƀ��[�V�������Z�b�g
		void StandStart()
		{
			m_cSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_Stand);
		}
	};

	//�ړ��p�����[�^����
	CharacterUsualMove::Params p;

	p.Acceleration = 0.2f;
	p.MaxSpeed = 0.2f;
	p.TurnSpeed = 0.3f;
	p.DownSpeed = 0.2f;

	//�ړ��N���X�̍쐬
	m_pMoveClass = new CharacterUsualMove(
		s,
		p,
		new SoccerMoveEvent(s),
		new SoccerHitEvent(s)
		);


	//�ړ��R���g���[���N���X�̍쐬
	
	m_pMoveControllClass = new CharacterComputerMove(s);

}


void SoccerState_ComputerControll_Move::Execute(SoccerPlayer* s)
{
	//�X�e�B�b�N�l���Z�b�g
	m_pMoveClass->SetStickValue(m_pMoveControllClass->SwitchAction(s));

	//�X�V
	m_pMoveClass->Update();

	//���f���̃��[���h�ϊ��s����X�V
	chr_func::CreateTransMatrix(s, s->m_ModelSize, &s->m_Renderer.m_TransMatrix);

}

void SoccerState_ComputerControll_Move::Exit(SoccerPlayer* t)
{
	delete m_pMoveClass;
	delete m_pMoveControllClass;
}
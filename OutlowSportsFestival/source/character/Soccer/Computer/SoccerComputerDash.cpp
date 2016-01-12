#include "../Computer/SoccerComputerMove.h"
#include "../../Computer/CharacterComputerReactionHitEvent.h"
#include "../../CharacterFunction.h"
#include "../../CharacterMoveClass.h"
#include "../SoccerPlayerState.h"
#include "../SoccerRolling.h"
#include "../SoccerSliding.h"
#include "SoccerComputerDash.h"
#include "../SoccerPlayerState_PoseMotion.h"
#include "../../CharacterManager.h"
#include "../Computer/SoccerComputerUtilityClass.h"
#include "../../../Camera/Camera.h"
#include "../SoccerHitEvent.h"
#include "../SoccerCommonState.h"
#include "../../../Effect/EffectFactory.h"
#include "../../../Sound/Sound.h"
#include "../../../Collision/Collision.h"

bool SoccerState_ComputerControll_Dash::SwitchGameState(SoccerPlayer* ps)
{
	Vector3 v;

	switch (ps->GetStateType())
	{
	case CharacterBase::State::Usual:

		return false;

	case CharacterBase::State::Freeze:

		return true;

	case CharacterBase::State::LosePose:
		ps->SetState(new SoccerState_PoseMotion(SoccerPlayer::_ms_Lose, 0.2f, 1000));
		return true;

	case CharacterBase::State::WinPose:
		ps->SetState(new SoccerState_PoseMotion(SoccerPlayer::_ms_Win, 0.2f, 1000));

		return true;
	default:
		break;
	}

	return false;


}
SoccerState_ComputerControll_Dash::SoccerState_ComputerControll_Dash(SoccerPlayer* s) :m_cSoccer(s)
{
}
//�X�e�[�g�J�n
void SoccerState_ComputerControll_Dash::Enter(SoccerPlayer* s)
{
	class SoccerDashEvent : public CharacterUsualMove::MoveEvent
	{
		SoccerPlayer* m_pSoccer;
	public:
		SoccerDashEvent(SoccerPlayer* pSoccer) :
			m_pSoccer(pSoccer) {}

		void Update(bool isRun, RATIO speed_ratio)
		{
			m_pSoccer->m_Renderer.Update(1);
		}

		void RunStart()
		{
			m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_Run);
		}

		void StandStart()
		{
			m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_Stand);
		}
	};
	m_Count = 0;
	CharacterComputerMove::GetParams(m_cParam, s->m_PlayerInfo.strong_type);
	Sound::Play(Sound::Soccer_Speed_Up1);
	m_pDashClass = new SoccerDash(s);

	//�U���C�x���g�N���X
	class SoccerDoEvent :public CharacterComputerDoAction::ActionEvent
	{
		SoccerPlayer* m_cSoccer;
		CharacterComputerMove::Param cParam;
		CharacterComputerMove*  m_pMoveControllClass;
		int AttackPoint;
	public:
		SoccerDoEvent(SoccerPlayer* cSoccer) :
			m_cSoccer(cSoccer) 
		{
			AttackPoint = rand() % 500;
			m_pMoveControllClass->GetParams(cParam, m_cSoccer->m_PlayerInfo.strong_type);
		}

		//�A�j���[�V�����̍X�V
		void Attack(float len)override
		{
			//if (m_cSoccer->m_Params.SP >= 0.6f)
			if (chr_func::isCanSpecialAttack(m_cSoccer))
			{
				m_cSoccer->SetState(new SoccerState_PlayerControll_Finisher());
			}
			if (len < 6.0f)
			{
				if ((cParam.ActionFrequence * 500) > AttackPoint)
				{
                    m_cSoccer->SetState(new SoccerState_Sliding(m_cSoccer));
				}
			}
			else if (len < 22.0f)
			{
				if ((cParam.ActionFrequence * 150) > AttackPoint)
				{
					m_cSoccer->SetState(new SoccerState_PlayerControll_Shot);
				}
			}
		}

	};
	CharacterComputerMove::Param cParam;
	//�U���N���X�̍쐬
	m_pDoActionClass = new CharacterComputerDoAction(
		s,
		cParam,
		new SoccerDoEvent(s),
		new SoccerHitEvent(s)
		);

	//�����C�x���g�N���X
	class SoccerReactionEvent :public CharacterComputerReaction::ActionEvent
	{
		Vector3 Vec;
		SoccerPlayer* m_cSoccer;
		CharacterComputerMove::Param cParam;
		CharacterComputerMove*  m_pMoveControllClass;
		int ReactionPoint;
	public:
		SoccerReactionEvent(SoccerPlayer* cSoccer) :
			m_cSoccer(cSoccer)
		{
			m_pMoveControllClass->GetParams(cParam, m_cSoccer->m_PlayerInfo.strong_type);
			ReactionPoint = rand() % 100;
		}

		//�A�j���[�V�����̍X�V
		void Reaction(CharacterComputerReactionHitEvent::HitType hittype, Vector3 vec)override
		{
			if (hittype == CharacterComputerReactionHitEvent::HitType::CanCounter)
			{
				if ((cParam.BallCounter * 100) > ReactionPoint)
				{
					m_cSoccer->SetState(new SoccerState_PlayerControll_Counter);
				}
			}
			else
			{
				if ((cParam.BallCounter * 100) > ReactionPoint)
				{
					m_cSoccer->SetState(
						new SoccerState_Rolling
						(new SocceComputerrUtillityClass::ComputerRollingControll(m_cSoccer, vec), true));
				}
			}
		}

	};
	//�����N���X�̍쐬
	m_pReactionClass = new CharacterComputerReaction(
		s,
		m_cParam,
		new SoccerReactionEvent(s)
		);
	
	movemode = Wait;
}


void SoccerState_ComputerControll_Dash::Execute(SoccerPlayer* s)
{
	++m_Count;


	Vector2 st;
	

	//�@�ǂɌ��˂����猃�˃X�e�[�g
	// ��莞�ԑ��� / �ړ����Ȃ��Ȃ�����߂�
	if (movemode == clash)
	{
		s->SetState(new SoccerState_clash(s));
	}
	else if (m_Count>100  || movemode== Stop)
	{
		s->SetState(new SoccerState_brake(s));
	}
	else if(SwitchGameState(s) == false)
	{
		
		st = SwitchAction(s);
		m_pDashClass->SetStickValue(st);
		m_pDashClass->Update();
		m_pDoActionClass->Update();
		m_pReactionClass->Update();
	}
	else
	{
		m_pDashClass->SetStickValue(Vector2(0, 0));
	}

	if (m_Count % 19 == 5)
	{
		Sound::Play(Sound::Sand1);
		EffectFactory::Smoke(
			s->m_Params.pos + Vector3(frand() - 0.5f, frand(), frand() - 0.5f)*2.0f,
			Vector3Zero,
			1.8f,
			0.5f,
			true
			);
	}
	
	chr_func::CreateTransMatrix(s, &s->m_Renderer.m_TransMatrix);
}

void SoccerState_ComputerControll_Dash::Exit(SoccerPlayer* s)
{
	delete m_pDashClass;
	delete m_pReactionClass;
	delete m_pDoActionClass;
}
Vector2 SoccerState_ComputerControll_Dash::StateMoveFront(SoccerPlayer* s)
{
	++m_Count;
	const float GoalOKlen = 1.0f; //�S�[���Ƃ݂Ȃ�����(�덷�̑Ώ�)
	m_MoveTargetPos = CharacterComputerMove::GetMoveTargetPos(s);

	//�ڕW�ɓ��B���Ă�����Ƃ܂�
	if (Vector3Distance(m_MoveTargetPos, m_cSoccer->m_Params.pos) < GoalOKlen)
	{
		m_Count = (int)(m_cParam.RunStop * 100.0f);
		movemode = Stop;
		m_Count = 0;
	}


	//�ڕW�ɓ��B�ł��Ȃ� or �V�ڕW������΂����ɕύX����
	if (m_Count > 200)
	{
		movemode = Stop;
		m_Count = 0;
	}
	//�ǂɌ������Đi�ނ̖h�~
	if (chr_func::IsHitWall(s))
	{
		m_Count = (int)(m_cParam.RunStop * 100.0f);
		movemode = clash;
		m_Count = 0;
	}


	//�ڕW�Ɍ������Ĉړ�


	Vector3 v = m_MoveTargetPos - s->m_Params.pos;
	/*if (v.Length() < 3.0f)
	{
		v = Vector3Zero;
		movemode = Stop;
		m_Count = 0;
	}*/
	return Vector2Normalize(Vector2(v.x, v.z));
}
Vector2 SoccerState_ComputerControll_Dash::StateMoveDistance(SoccerPlayer* s)
{
	++m_Count;
	const float Bestlen = 12.0f + rand() % 10; //���̃L�����̃x�X�g����(���͌Œ�)


	//�ڕW�ɓ��B���Ă�����Ƃ܂�
	if (Vector3Distance(m_MoveTargetPos, m_cSoccer->m_Params.pos) > 20.0f)
	{
		m_Count = (int)(m_cParam.RunStop * 100.0f);
		movemode = Stop;
		m_Count = 0;
	}


	//�ڕW�ɓ��B�ł��Ȃ� or �V�ڕW������΂����ɕύX����
	if (m_Count > 300)
	{
		movemode = Stop;
		m_Count = 0;
	}
	//�ǂɓ�����ƌ���
	if (chr_func::IsHitWall(m_cSoccer))
	{
		m_Count = (int)(m_cParam.RunStop * 100.0f);
		movemode = clash;
		m_Count = 0;
	}


	//�ڕW�Ɍ������Ĉړ�
	m_MoveTargetPos = CharacterComputerMove::GetMoveTargetPos(s);

	Vector3 v = s->m_Params.pos - m_MoveTargetPos;

	if (v.Length() < 1.0f)
	{
		v = Vector3Zero;
		movemode = Stop;
		m_Count = 0;
	}
	return Vector2Normalize(Vector2(v.x, v.z));
}
Vector2 SoccerState_ComputerControll_Dash::StateMoveCenter(SoccerPlayer* s)
{
	++m_Count;

	//�ڕW�ɓ��B���Ă�����Ƃ܂�
	if (Vector3Distance(Vector3(0, 0, 0), m_cSoccer->m_Params.pos) < 1.0f)
	{
		m_Count = (int)(m_cParam.RunStop * 100.0f);
		movemode = Forward;
		m_Count = 0;
	}


	//�����ړ��������߂�
	if (m_Count > 150)
	{
		movemode = Stop;
		m_Count = 0;
	}


	//�ڕW�Ɍ������Ĉړ�
	//m_MoveTargetPos = Vector3(0, 0, 0);

	Vector3 v = Vector3(0, 0, 0) - s->m_Params.pos;

	/*if (v.Length() < 1.0f)
	{
	v = Vector3Zero;
	movemode = Stop;
	m_Count = 0;
	}*/
	return Vector2Normalize(Vector2(v.x, v.z));


}
Vector2 SoccerState_ComputerControll_Dash::StateWait(SoccerPlayer* s)
{

	m_MoveTargetPos = CharacterComputerMove::GetMoveTargetPos(s);

	if (Vector3Distance(Vector3(0, 0, 0), m_cSoccer->m_Params.pos) > 30.0f)
	{
		movemode = MoveCenter;
	}
	if (Vector3Distance(m_MoveTargetPos, s->m_Params.pos) > 20.0f)
	{
		movemode = Forward;
	}
	else
	{
		movemode = Distance;
	}
	

	return Vector2Normalize(Vector2(0, 0));
}

Vector2 SoccerState_ComputerControll_Dash::SwitchAction(SoccerPlayer* s)
{

	Vector2 xz;
	switch (movemode)
	{
	case Wait:
		xz = StateWait(s);
		break;
	case Forward:
		xz = StateMoveFront(s);
		break;
	case Distance:
		xz = StateMoveDistance(s);
		break;
	case MoveCenter:
		xz = StateMoveCenter(s);
		break;
	}
	return xz;
}

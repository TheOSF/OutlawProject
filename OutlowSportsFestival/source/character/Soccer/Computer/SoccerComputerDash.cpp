#include "../Computer/SoccerComputerMove.h"
#include "../../Computer/CharacterComputerReactionHitEvent.h"
#include "../../CharacterFunction.h"
#include "../../CharacterCounterClass.h"
#include "../../CharacterMoveClass.h"
#include "SoccerComputerAttack.h"
#include "../SoccerPlayerState.h"
#include "../SoccerRolling.h"
#include "SoccerComputerShot.h"
#include "../SoccerSliding.h"
#include "SoccerComputerDash.h"
#include "SoccerComputerCounter.h"
#include "SoccerComputerRolling.h"
#include "SoccerComputerSliding.h"
#include "../SoccerPlayerState_PoseMotion.h"
#include "../../CharacterManager.h"
#include "../../../Camera/Camera.h"
#include "../SoccerHitEvent.h"
#include "../SoccerCommonState.h"
#include "../../../Effect/EffectFactory.h"
#include "../../../Sound/Sound.h"
class SocceComputerrUtillityClass
{
public:
	//���[�����O�̕�������N���X
	class ComputerRollingControll :public SoccerState_ComputerControll_Rolling::CallBackClass
	{
	public:
		SoccerPlayer*const cs;
		ComputerRollingControll(SoccerPlayer* ps, Vector3 vec) :cs(cs), stick(vec) {}
		Vector3 stick;

		Vector3 GetVec()override
		{

			Vector3 vec(stick.x, 0, stick.z);

			return vec;
		}
	};

};


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
	class SoccerDashEvent
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
	public:
		SoccerDoEvent(SoccerPlayer* cSoccer) :
			m_cSoccer(cSoccer) {}

		//�A�j���[�V�����̍X�V
		void Attack(float len)override
		{

			if (len < 6.0f)
			{
				m_cSoccer->SetState(new SoccerState_ComputerControll_Sliding(m_cSoccer));
			}
			else if (len < 20.0f)
			{
				m_cSoccer->SetState(new SoccerState_ComputerControll_Shot);
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
	public:
		SoccerReactionEvent(SoccerPlayer* cSoccer) :
			m_cSoccer(cSoccer) {}

		//�A�j���[�V�����̍X�V
		void Reaction(CharacterComputerReactionHitEvent::HitType hittype, Vector3 vec)override
		{
			if (hittype == CharacterComputerReactionHitEvent::HitType::CanCounter)
			{
				m_cSoccer->SetState(new SoccerState_PlayerControll_Counter);
			}
			else
			{
				m_cSoccer->SetState(
					new SoccerState_ComputerControll_Rolling
					(new SocceComputerrUtillityClass::ComputerRollingControll(m_cSoccer, vec),true));
			}
		}

	};
	//�����N���X�̍쐬
	m_pReactionClass = new CharacterComputerReaction(
		s,
		m_cParam,
		new SoccerReactionEvent(s)
		);
	
	movemode = Stop;
}


void SoccerState_ComputerControll_Dash::Execute(SoccerPlayer* s)
{
	++m_Count;


	Vector2 st;
	


	// ��莞�ԑ��� / �ړ����Ȃ��Ȃ�����߂�
	if (m_Count>100 )
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

	if (m_Count % 19 == 5)
	{
		Sound::Play(Sound::Sand1);
		EffectFactory::Smoke(
			s->m_Params.pos + Vector3(frand() - 0.5f, frand(), frand() - 0.5f)*2.0f,
			Vector3Zero,
			1.8f,
			0xFFFFA080,
			true
			);
	}
	
	chr_func::CreateTransMatrix(s, 0.05f, &s->m_Renderer.m_TransMatrix);
}

void SoccerState_ComputerControll_Dash::Exit(SoccerPlayer* s)
{
	delete m_pDashClass;
}
Vector2 SoccerState_ComputerControll_Dash::StateMoveFront(SoccerPlayer* s)
{
	++m_Count;
	const float GoalOKlen = 2.0f; //�S�[���Ƃ݂Ȃ�����(�덷�̑Ώ�)

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


	//�ڕW�Ɍ������Ĉړ�
	m_MoveTargetPos = CharacterComputerMove:: GetMoveTargetPos(s);

	Vector3 v = m_MoveTargetPos - s->m_Params.pos;
	if (v.Length() < 3.0f)
	{
		v = Vector3Zero;
		movemode = Stop;
		m_Count = 0;
	}
	return Vector2Normalize(Vector2(v.x, v.z));
}
Vector2 SoccerState_ComputerControll_Dash::StateMoveDistance(SoccerPlayer* s)
{
	++m_Count;
	const float Bestlen = 12.0f + rand() % 10; //���̃L�����̃x�X�g����(���͌Œ�)


											   //�ڕW�ɓ��B���Ă�����Ƃ܂�
	if (Vector3Distance(m_MoveTargetPos, m_cSoccer->m_Params.pos) > Bestlen)
	{
		m_Count = (int)(m_cParam.RunStop * 100.0f);
		movemode = Forward;
		m_Count = 0;
	}


	//�ڕW�ɓ��B�ł��Ȃ� or �V�ڕW������΂����ɕύX����
	if (m_Count > 200)
	{
		movemode = Stop;
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
Vector2 SoccerState_ComputerControll_Dash::StateStop(SoccerPlayer* s)
{

	m_MoveTargetPos = CharacterComputerMove::GetMoveTargetPos(s);

	
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
	case Stop:
		xz = StateStop(s);
		break;
	case Forward:
		xz = StateMoveFront(s);
		break;
	case Distance:
		xz = StateMoveDistance(s);
		break;
	}
	return xz;
}


//Vector3 SoccerState_ComputerControll_Dash::GetMoveTargetPos()
//{
//	Vector3 ret;
//	Vector3 nearTarget = Vector3Zero;
//	float MostTaugh = 0;
//
//	CharacterBase* pTarget = nullptr;
//
//
//
//	struct TargetInfo
//	{
//		bool      ok;
//		Vector3   pos;
//	};
//
//	TargetInfo targets[8];
//
//	const CharacterManager::CharacterMap& ChrMap = DefCharacterMgr.GetCharacterMap();
//
//	for (auto it = ChrMap.begin(); it != ChrMap.end(); ++it)
//	{
//
//		//�@����ł�or�����Ȃ�continue
//		if (chr_func::isDie(it->first) ||
//			it->first->m_PlayerInfo.number == m_cCharacter->m_PlayerInfo.number)
//		{
//			continue;
//		}
//		//�ł��̗͂̍����G���^�[�Q�b�g��
//		if (it->first->m_Params.HP > MostTaugh)
//		{
//			pTarget = it->first;
//			MostTaugh = it->first->m_Params.HP;
//		}
//	}
//	//�B�G�̍ő�HP��0(MostTaugh�̕ω��Ȃ�)�Ȃ�0�x�N�Ԃ�
//	if (MostTaugh == 0)
//	{
//		return Vector3Zero;
//	}
//
//
//	return pTarget->m_Params.pos;
//}
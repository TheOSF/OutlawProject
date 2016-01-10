#include "../Computer/SoccerComputerMove.h"
#include "../../Computer/CharacterComputerReactionHitEvent.h"
#include "../../CharacterFunction.h"
#include "../../CharacterMoveClass.h"
#include "SoccerComputerAttack.h"
#include "SoccerComputerDash.h"
#include "../SoccerPlayerState.h"
#include "../SoccerRolling.h"
#include "SoccerComputerShot.h"
#include "SoccerComputerFinisher.h"
#include "SoccerComputerCounter.h"
#include "../SoccerPlayerState_PoseMotion.h"
#include "../../CharacterManager.h"
#include "../../../Camera/Camera.h"
#include "../SoccerHitEvent.h"
#include "../../CharacterManager.h"

#include "../Computer/SoccerComputerUtilityClass.h"



bool SoccerState_ComputerControll_Move::SwitchGameState(SoccerPlayer* ps)
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

//�X�e�[�g�J�n
void SoccerState_ComputerControll_Move::Enter(SoccerPlayer* s)
{
    CharacterComputerMove::GetParams(cParam, s->m_PlayerInfo.strong_type);
	Dashpro = 250 - (int)(cParam.ActionFrequence * 100);
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
        //����I���
        void RunEnd()
        {
            m_cSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_RunStop);
        }
	};

	//�ړ��p�����[�^����
	CharacterUsualMove::Params Movep;

	Movep.Acceleration = 0.25f;
	Movep.MaxSpeed = 0.35f;
	Movep.TurnSpeed = 0.3f;
	Movep.DownSpeed = 0.08f;
	Movep.RunEndFrame = 35;


	//�ړ��R���g���[���N���X�̍쐬
	m_pMoveControllClass = new CharacterComputerMove(s);

	//�ړ��N���X�̍쐬
	m_pMoveClass = new CharacterUsualMove(
		s,
		Movep,
		new SoccerMoveEvent(s),
		new SoccerHitEvent(s)
		);


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
			if (chr_func::isCanSpecialAttack(m_cSoccer))
			{
				if (SoccerState_ComputerControll_Move::calcTarget(m_cSoccer) == true)
				{
					m_cSoccer->SetState(new SoccerState_ComputerControll_Finisher());
				}
			}
			if (len < 5.0f)
			{
				if (AttackPoint>450)
				{
					const Vector3 avoidvec(0, 0, 0);
					m_cSoccer->SetState(
						new SoccerState_Rolling
						(new SocceComputerrUtillityClass::ComputerRollingControll(m_cSoccer, avoidvec),
							false));
				}
				else if ((cParam.ActionFrequence * 450) > AttackPoint)
				{
					m_cSoccer->SetState(new SoccerState_ComputerControll_Attack(m_cSoccer));
				}
			}
			else if (len < 20.0f)
			{
				if ((cParam.ActionFrequence * 350) > AttackPoint)
				{
					m_cSoccer->SetState(new SoccerState_ComputerControll_Shot);
				}
			}
		}

	};

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
						(new SocceComputerrUtillityClass::ComputerRollingControll(m_cSoccer, vec),
							false));
				}
			}
		}

	};
	//�����N���X�̍쐬
	m_pReactionClass = new CharacterComputerReaction(
		s,
		cParam,
		new SoccerReactionEvent(s)
		);
}


void SoccerState_ComputerControll_Move::Execute(SoccerPlayer* s)
{
	if (SwitchGameState(s) == false)
	{
		m_pMoveClass->SetStickValue(m_pMoveControllClass->SwitchAction(s));
		m_pDoActionClass->Update();
		m_pReactionClass->Update();

		if (rand() %  Dashpro == 0)
		{
			s->SetState(new SoccerState_ComputerControll_Dash(s));
		}
	}
	else
	{
		m_pMoveClass->SetStickValue(Vector2(0,0));
	}
	m_pMoveClass->Update();
	
	
	//���f���̃��[���h�ϊ��s����X�V

	chr_func::CreateTransMatrix(s, s->m_ModelSize, &s->m_Renderer.m_TransMatrix);

}

void SoccerState_ComputerControll_Move::Exit(SoccerPlayer* s)
{
	delete m_pMoveClass;
	delete m_pMoveControllClass; 
	delete m_pDoActionClass;
	delete m_pReactionClass;
}
bool SoccerState_ComputerControll_Move::calcTarget(SoccerPlayer* s)
{
	//�^�[�Q�b�g�I�聕�����␳

	CharacterManager::CharacterMap ChrMap = DefCharacterMgr.GetCharacterMap();

	const float  AutoDistance = 400.0f;               //�������ł���ő勗��
	const RADIAN AutoMaxAngle = D3DXToRadian(90);   //�������ł���ő�p�x

	const CharacterBase* pTargetEnemy = nullptr;    //�^�[�Q�b�g�ێ��̃|�C���^
	RADIAN MostMinAngle = PI;                       //�����Ƃ������p�x
	RADIAN TempAngle;

	Vector3 MyFront;      //���g�̑O���x�N�g��
	chr_func::GetFront(s, &MyFront);

	auto it = ChrMap.begin();

	while (it != ChrMap.end())
	{
		//���g�����O
		if (s->m_PlayerInfo.number == it->first->m_PlayerInfo.number ||
			chr_func::isDie(it->first)
			)
		{
			++it;
			continue;
		}

		//���������ȏ�̃L�����N�^�����O����
		if (Vector3Distance(it->first->m_Params.pos, s->m_Params.pos) > AutoDistance)
		{
			it = ChrMap.erase(it);
			continue;
		}

		//�O�x�N�g���ƓG�ւ̃x�N�g���̊p�x���v�Z����
		TempAngle = Vector3Radian(MyFront, (it->first->m_Params.pos - s->m_Params.pos));

		//�p�x����ԋ���������X�V
		if (TempAngle < MostMinAngle)
		{
			pTargetEnemy = it->first;
			MostMinAngle = TempAngle;
		}

		++it;
	}
	if (MostMinAngle < .1f)
	{
		//return pTargetEnemy->m_Params.pos;
		return true;
	}
	else
	{
		//return Vector3(0, 0, 0);
		return false;
	}
}

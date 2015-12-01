#include "BaseballPlayerState_ComMove.h"

#include "../Baseball_HitEvent.h"
#include "../BaseballPlayerState_Attack_B.h"
#include "../BaseballPlayerState_Attack_P.h"
#include "../BaseballState_PlayerControll_ShotAttack_B.h"
#include "../BaseballState_PlayerControll_ShotAttack_P.h"
#include "../BaseballState_SPAttack_B.h"
#include "../BaseballState_SPAttack_P.h"
#include "../BaseballState_Change.h"
#include "../BaseballPlayerState_Counter.h"
#include "../BaseballPlayerState.h"
#include "../BaseballState_PlayerControll_Evasion.h"
#include "../BaseballState_PoseMotion.h"

#include "../../../GameSystem/GameController.h"
#include "../../CharacterFunction.h"
#include "../../../Ball/UsualBall.h"
#include "../../../Damage/Damage.h"
#include "../../CharacterCounterClass.h"
#include "../../CharacterMoveClass.h"
#include "../../CharacterShotAttackClass.h"
#include "../../Computer/CharacterComputerReactionHitEvent.h"
#include "../../CharacterManager.h"


class BaseballPlayerComputerrUtillityClass
{
public:
	//���[�����O�̕�������N���X
	class ComputerRollingControll :public  BaseballState_Rolling::CallBackClass
	{
	public:
		BaseballPlayer*const cb;
		ComputerRollingControll(BaseballPlayer* pb, Vector3 vec) :cb(cb), stick(vec) {}
		Vector3 stick;


		Vector3 GetVec()override
		{
			Vector3 vec(stick.x, 0, stick.z);

			return vec;

		}
	};

};


bool BaseballPlayerState_ComMove::SwitchGameState(BaseballPlayer* pb)
{
	Vector3 v;

	switch (pb->GetStateType())
	{
	case CharacterBase::State::Usual:

		return false;

	case CharacterBase::State::Freeze:

		return true;

	case CharacterBase::State::LosePose:
		pb->SetState(new BaseballState_PoseMotion(baseball_player::_mb_LosePose, 0.2f, 1000));
		return true;

	case CharacterBase::State::WinPose:
		pb->SetState(new BaseballState_PoseMotion(baseball_player::_mb_WinPose, 0.2f, 1000));

		return true;
	default:
		break;
	}

	return false;


}

//�X�e�[�g�J�n
void BaseballPlayerState_ComMove::Enter(BaseballPlayer* b)
{
	//�@�ړ�
	doMove(b);
	//�@�U��
	doAction(b);
	//�@����
	doReaction(b);
}


void BaseballPlayerState_ComMove::Execute(BaseballPlayer* b)
{

	if (SwitchGameState(b) == false)
	{
		BaseballPlayer* b2 = b;

		//�X�e�B�b�N�l���Z�b�g
		m_pMoveClass->SetStickValue(m_pMoveControllClass->SwitcAction_Baseball(b, b->getBatterFlg()));
		//�@�؂�ւ�
		doChange(b);
	}
	if (!chr_func::isDie(b))
	{
		//********
		//�@�X�V
		//********
		//�@�U��
		m_pDoActionClass->Update();
		//�@����
		m_pMoveClass->Update();
		//�@����
		m_pReactionClass->Update();

	}
	//���f���̃��[���h�ϊ��s����X�V
	chr_func::CreateTransMatrix(b, b->m_ModelSize, &b->m_Renderer.m_TransMatrix);


}

void BaseballPlayerState_ComMove::Exit(BaseballPlayer* b)
{
	delete m_pMoveClass;
	delete m_pMoveControllClass;
	delete m_pDoActionClass;
	delete m_pReactionClass;
}

//�@�ړ�
void BaseballPlayerState_ComMove::doMove(BaseballPlayer* b)
{
	//�ړ��C�x���g�N���X
	class BaseballMoveEvent :public CharacterUsualMove::MoveEvent
	{
		BaseballPlayer* m_pBaseball;
	public:
		BaseballMoveEvent(BaseballPlayer* pBaseball) :
			m_pBaseball(pBaseball){}

		//�A�j���[�V�����̍X�V
		void Update(bool isRun, RATIO speed_ratio)
		{
			m_pBaseball->m_Renderer.Update(1);
		}
		//����n�߂Ƀ��[�V�������Z�b�g
		void RunStart()
		{
			m_pBaseball->m_Renderer.SetMotion(baseball_player::_mb_Run);
		}
		//�����͂��߂Ƀ��[�V�������Z�b�g
		void StandStart()
		{
			m_pBaseball->m_Renderer.SetMotion(baseball_player::_mb_Stand);
		}
	};

	//�ړ��p�����[�^����
	CharacterUsualMove::Params p;

	p.Acceleration = 0.2f;
	p.MaxSpeed = 0.2f;
	p.TurnSpeed = 0.3f;
	p.DownSpeed = 0.2f;

	//�ړ��R���g���[���N���X�̍쐬
	m_pMoveControllClass = new CharacterComputerMove(b);

	//�ړ��N���X�̍쐬
	m_pMoveClass = new CharacterUsualMove(
		b,
		p,
		new BaseballMoveEvent(b),
		new BaseballHitEvent(b)
		);
}

//�@�U��
void BaseballPlayerState_ComMove::doAction(BaseballPlayer* b)
{
	//�U���C�x���g�N���X
	class BaseballDoEvent :public CharacterComputerDoAction::ActionEvent
	{

		BaseballPlayer* m_cBaseball;
		CharacterComputerMove::Param cParam;
		CharacterComputerMove*  m_pMoveControllClass;
		int AttackPoint;
	public:
		BaseballDoEvent(BaseballPlayer* cBaseball) :
			m_cBaseball(cBaseball) 
		{
			AttackPoint = rand() % 100;
			m_pMoveControllClass->GetParams(cParam, m_cBaseball->m_PlayerInfo.strong_type);
		}

		//�A�j���[�V�����̍X�V
		void Attack(float len)override
		{
			if (len == 0){
				return;
			}
			//�@���s�p�^�[��
			if (m_cBaseball->getBatterFlg())
			{
				//�@�o�b�^�[��
				if (len < 7.0f)
				{
					if ((cParam.ActionFrequence * 100) > AttackPoint)
					{
						//�@�K�E
						if (chr_func::isCanSpecialAttack(m_cBaseball))
						{
							m_cBaseball->SetState(new BaseballState_SPAttack_B(m_cBaseball));
						}
						else
						{
							m_cBaseball->SetState(new Baseball_PlayerControll_Attack_B(m_cBaseball));
						}
					}

				}
				else if (len >= 6.0f && len < 25.0f)
				{
					if ((cParam.ActionFrequence * 100) > AttackPoint)
					{
						m_cBaseball->SetState(new BaseballState_PlayerControll_ShotAttack_B(new PlayerShotControllClass_B(m_cBaseball)));
					}
				}
			}
			else{
				if (chr_func::isCanSpecialAttack(m_cBaseball))
				{
					m_cBaseball->SetState(new BaseballState_SPAttack_P());
				}
				//�@���莞
				if (len < 6.0f)
				{
					if ((cParam.ActionFrequence * 100) > AttackPoint)
					{
						m_cBaseball->SetState(new Baseball_PlayerControll_Attack_P(m_cBaseball));
					}
				}
				else if (len >= 6.0f && len < 35.0f)
				{
					if ((cParam.ActionFrequence * 100) > AttackPoint)
					{
						m_cBaseball->SetState(new BaseballState_PlayerControll_ShotAttack_P());
					}
				}
			}

		}
	};

	CharacterComputerMove::Param cParam;
	m_pMoveControllClass->GetParams(cParam, b->m_PlayerInfo.strong_type);
	//�U���N���X�̍쐬
	m_pDoActionClass = new CharacterComputerDoAction(
		b,
		cParam,
		new BaseballDoEvent(b),
		new BaseballHitEvent(b)
		);

}


//�@�؂�ւ�
void BaseballPlayerState_ComMove::doChange(BaseballPlayer* b)
{
	if (!chr_func::isDie(b))
	{
		nearpos = GetNearTargetPos(b) - b->m_Params.pos;

		//�@�^�[�Q�b�g�ƈ�苗���ȉ��E�ȏ�Ȃ�؂�ւ�
		if (nearpos.Length() < 15.0f && !b->getBatterFlg() ||
			nearpos.Length() > 34.0f && b->getBatterFlg())
		{
			b->SetState(new BaseballState_Change());
		}
	}

}

//�@����
void  BaseballPlayerState_ComMove::doReaction(BaseballPlayer* b)
{
	//�����C�x���g�N���X
	class  BaseballPlayerReactionEvent :public CharacterComputerReaction::ActionEvent
	{
		Vector3 Vec;
		BaseballPlayer* m_cBaseball;
		CharacterComputerMove::Param cParam;
		CharacterComputerMove*  m_pMoveControllClass;
		int ReactionPoint;
	public:
		BaseballPlayerReactionEvent(BaseballPlayer* cBaseball) :
			m_cBaseball(cBaseball)
		{
			m_pMoveControllClass->GetParams(cParam, m_cBaseball->m_PlayerInfo.strong_type);
			ReactionPoint = rand() % 100;

		}

		//�A�j���[�V�����̍X�V
		void Reaction(CharacterComputerReactionHitEvent::HitType hittype, Vector3 vec)override
		{
			int rnd = rand() % 10;
			//�@�������U���Ȃ�
			if (hittype == CharacterComputerReactionHitEvent::HitType::CanCounter)
			{
				if ((cParam.BallCounter * 100) > ReactionPoint)
				{
				//�@���s�p�^�[��
					if (m_cBaseball->getBatterFlg()){
						//�@7�F3�̊����ɂȂ�悤��
						if (rnd < 3)
						{
							//�@�o�b�^�[��
							m_cBaseball->SetState(new BaseballState_PlayerControll_Counter(5));
						}
						else
						{
							m_cBaseball->SetState(new BaseballState_Rolling(new BaseballPlayerComputerrUtillityClass::ComputerRollingControll(m_cBaseball, vec)));
						}
					}
				}
				else
				{
					if ((cParam.BallCounter * 100) > ReactionPoint)
					{
						//�@7�F3�̊����ɂȂ�悤��
						if (rnd >= 3)
						{
							//�@���莞
							m_cBaseball->SetState(new BaseballState_PlayerControll_Counter(9));
						}
						else
						{
							m_cBaseball->SetState(new BaseballState_Rolling(new BaseballPlayerComputerrUtillityClass::ComputerRollingControll(m_cBaseball, vec)));
						}
					}
				}
			}
			//�@����ȊO
			else
			{
				m_cBaseball->SetState(
					new BaseballState_Rolling(new BaseballPlayerComputerrUtillityClass::ComputerRollingControll(m_cBaseball, vec)));
			}
		}

	};

	//�����N���X�̍쐬
	CharacterComputerMove::Param cParam;
	m_pMoveControllClass->GetParams(cParam, b->m_PlayerInfo.strong_type);
	m_pReactionClass = new CharacterComputerReaction(
		b,
		cParam,
		new BaseballPlayerReactionEvent(b)
		);
}

//�@��ԋ߂��L������pos���Ƃ�
Vector3 BaseballPlayerState_ComMove::GetNearTargetPos(BaseballPlayer* b)
{
	Vector3 v;

	float MostNear = 1000;
	float TempLen;
	CharacterBase* pTarget = nullptr;

	//�@map���
	const CharacterManager::CharacterMap& chr_map =
		DefCharacterMgr.GetCharacterMap();

	for (auto it = chr_map.begin(); it != chr_map.end(); ++it)
	{

		//�@����ł�or�����Ȃ�continue
		if (chr_func::isDie(it->first) ||
			it->first->m_PlayerInfo.number == b->m_PlayerInfo.number)
		{
			continue;
		}

		v = it->first->m_Params.pos - b->m_Params.pos;
		v.y = 0;

		TempLen = v.Length();

		if (MostNear > TempLen)
		{
			MostNear = TempLen;
			pTarget = it->first;
		}
	}

	if (pTarget == nullptr)
	{
		return Vector3Zero;
	}

	return pTarget->m_Params.pos;
}
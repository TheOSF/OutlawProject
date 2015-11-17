#include "BaseballPlayerState_ComMove.h"
#include "../Baseball_HitEvent.h"
#include "../BaseballPlayerState_Attack_B.h"
#include "../BaseballPlayerState_Attack_P.h"
#include "../BaseballState_PlayerControll_ShotAttack_B.h"
#include "../BaseballState_PlayerControll_ShotAttack_P.h"
#include "../BaseballState_Change.h"
#include "../BaseballPlayerState_Counter.h"
#include "../BaseballPlayerState.h"
#include "../BaseballState_PlayerControll_Evasion.h"

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

		ComputerRollingControll(BaseballPlayer* pb) :cb(cb) {}


		Vector3 GetVec()override
		{
			Vector2 stick = Vector2(1, 1);
			//Vector2 stick = chr_func::GetRight(cs,);
			Vector3 vec(stick.x, 0, stick.y);

			if (vec.Length() < 0.25f)
			{
				return Vector3Zero;
			}

			vec = Vector3MulMatrix3x3(vec, matView);
			vec.Normalize();

			return vec;
		}
	};

};




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
	//�X�e�B�b�N�l���Z�b�g
	m_pMoveClass->SetStickValue(m_pMoveControllClass->SwitcAction_Baseball(b, b->getBatterFlg()));

	//********
	//�@�X�V
	//********
	//�@����
	m_pMoveClass->Update();
	//�@�U��
	m_pDoActionClass->Update();
	//�@����
	m_pReactionClass->Update();
	//�@�؂�ւ�
	doChange(b);
	
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
	public:
		BaseballDoEvent(BaseballPlayer* cBaseball) :
			m_cBaseball(cBaseball) {}

		//�A�j���[�V�����̍X�V
		void Attack(float len)override
		{
			//�@���s�p�^�[��
			if (m_cBaseball->getBatterFlg()){
				//�@�o�b�^�[��
				if (len < 6.0f)
				{
					m_cBaseball->SetState(new Baseball_PlayerControll_Attack_B(m_cBaseball));
				}
				else if ( len < 20.0f)
				{
					m_cBaseball->SetState(new BaseballState_PlayerControll_ShotAttack_B(new PlayerShotControllClass_B(m_cBaseball)));
				}
			}
			else{
				//�@���莞
				if (len < 5.0f)
				{
					m_cBaseball->SetState(new Baseball_PlayerControll_Attack_P(m_cBaseball));
				}
				else if (len < 30.0f)
				{
					m_cBaseball->SetState(new BaseballState_PlayerControll_ShotAttack_P());
				}
			}

		}
	};

	CharacterComputerMove::Param cParam;
	m_pMoveControllClass->GetParams(cParam, b->m_PlayerInfo.strong_type);
	//�U���N���X�̍쐬
	m_pDoActionClass = new CharacterComputerDoAction(b, cParam, new BaseballDoEvent(b), new BaseballHitEvent(b));

}


//�@�؂�ւ�
void BaseballPlayerState_ComMove::doChange(BaseballPlayer* b)
{

	nearpos = m_pMoveControllClass->GetMoveTargetPos(b) - b->m_Params.pos;
	
	//�@�^�[�Q�b�g�ƈ�苗���ȉ��E�ȏ�Ȃ�؂�ւ�
	if (nearpos.Length() < 15.0f && !b->getBatterFlg() ||
		nearpos.Length() > 36.0f && b->getBatterFlg())
	{
		b->SetState(new BaseballState_Change());
	}

}

//�@����
void  BaseballPlayerState_ComMove::doReaction(BaseballPlayer* b)
{
	//�����C�x���g�N���X
	class  BaseballPlayerReactionEvent :public CharacterComputerReaction::ActionEvent
	{

		BaseballPlayer* m_cBaseball;
	public:
		BaseballPlayerReactionEvent(BaseballPlayer* cBaseball) :
			m_cBaseball(cBaseball) {}

		//�A�j���[�V�����̍X�V
		void Reaction(CharacterComputerReactionHitEvent::HitType hittype, Vector3 vec)override
		{
			//�@�������U���Ȃ�
			if (hittype == CharacterComputerReactionHitEvent::HitType::CanCounter)
			{
				//�@���s�p�^�[��
				if (m_cBaseball->getBatterFlg()){
					//�@�o�b�^�[��
					m_cBaseball->SetState(new BaseballState_PlayerControll_Counter(5));
				}
				else
				{
					//�@���莞
					m_cBaseball->SetState(new BaseballState_PlayerControll_Counter(9));
				}
			}
			//�@����ȊO
			else
			{
				m_cBaseball->SetState(new BaseballState_Rolling(new BaseballPlayerComputerrUtillityClass::ComputerRollingControll(m_cBaseball)));
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

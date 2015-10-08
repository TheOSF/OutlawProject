#include "SoccerPlayerState.h"
#include "SoccerCommonState.h"
#include "SoccerHitEvent.h"
#include "../../GameSystem/GameController.h"
#include "../CharacterFunction.h"
#include "../CharacterBase.h"

SoccerState* SoccerState_PlayerControll_Move::GetPlayerControllMove(
	SoccerPlayer* ps)
{
	switch (ps->m_PlayerInfo.player_type)
	{
	case PlayerType::_Player:
		return new SoccerState_PlayerControll_Move();

	case PlayerType::_Computer:
		switch (ps->m_PlayerInfo.strong_type)
		{
		case StrongType::_Weak:
			//return new �ア�ʏ�ړ�
		case StrongType::_Usual:
			//return new �ア�ʏ�ړ�
		case StrongType::_Strong:
			//return new �ア�ʏ�ړ�
		default:break;
		}
	default:break;
	}

	assert("�ʏ�X�e�[�g���쐬�ł��Ȃ��L�����N�^�^�C�v�ł� TennisState_PlayerControll_Move::GetPlayerControllMove" && 0);
	return nullptr;
}

void SoccerState_PlayerControll_Move::Enter(SoccerPlayer* s)
{
	class SoccerMoveEvent :public CharacterUsualMove::MoveEvent
	{
		SoccerPlayer* m_pSoccer;
	public:
		SoccerMoveEvent(SoccerPlayer* pSoccer) :
			m_pSoccer(pSoccer){}

		//�A�j���[�V�����̍X�V
		void Update(bool isRun, RATIO speed_ratio)
		{
			m_pSoccer->m_Renderer.Update(1);
		}
		//����n�߂Ƀ��[�V�������Z�b�g
		void RunStart()
		{
			m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_Run);
		}
		//�����͂��߂Ƀ��[�V�������Z�b�g
		void StandStart()
		{
			m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_Stand);
		}
	};

	CharacterUsualMove::Params p;

	p.Acceleration = 0.2f;
	p.MaxSpeed = 0.2f;
	p.TurnSpeed = 0.3f;
	p.DownSpeed = 0.2f;

	m_pMoveClass = new CharacterUsualMove(
		s, 
		p, 
		new SoccerMoveEvent(s),
		new SoccerHitEvent(s));
}
void SoccerState_PlayerControll_Move::Execute(SoccerPlayer* s)
{
	Vector2 st = controller::GetStickValue(controller::stick::left, s->m_PlayerInfo.number);

	m_pMoveClass->SetStickValue(st);
	m_pMoveClass->Update();


	// [�~] �� �X���C�f�B���O
	if (controller::GetTRG(controller::button::batu, s->m_PlayerInfo.number))
	{
		s->SetState(new SoccerState_PlayerControll_Sliding);
	}
	// [��] �� �i��
	if (controller::GetTRG(controller::button::shikaku, s->m_PlayerInfo.number))
	{
		s->SetState(new SoccerState_PlayerControll_Attack);
	}
	// [��] �� �V���b�g
	if (controller::GetTRG(controller::button::sankaku, s->m_PlayerInfo.number))
	{
		s->SetState(new SoccerState_PlayerControll_Shot);
	}
	// [R1] �� �J�E���^�[
	if (controller::GetTRG(controller::button::_R1, s->m_PlayerInfo.number))
	{
		//s->SetState(new SoccerState_PlayerControll_Counter());
	}
	// [L1] �� �ŗL�Z(�_�b�V��)
	if (controller::GetTRG(controller::button::_L1, s->m_PlayerInfo.number))
	{
		s->SetState(new SoccerState_PlayerControll_Dash());
	}

	

	chr_func::CreateTransMatrix(s, 0.05f, &s->m_Renderer.m_TransMatrix);
}
void SoccerState_PlayerControll_Move::Exit(SoccerPlayer* s)
{
	delete m_pMoveClass;
}
void SoccerState_PlayerControll_Sliding::Enter(SoccerPlayer* s)
{

	SoccerSliding::Params p;
	p.speed = 0.5f;
	p.damage = 1;

	m_pSlidingClass = new SoccerSliding(p,s);
	m_pSlidingClass->Start();
}
void SoccerState_PlayerControll_Sliding::Execute(SoccerPlayer* s)
{
	m_pSlidingClass->Update();
	if (m_pSlidingClass->is_End())
	{
		s->SetState(new SoccerState_PlayerControll_Move);
	}
	
	chr_func::CreateTransMatrix(s, 0.05f, &s->m_Renderer.m_TransMatrix);
}
void SoccerState_PlayerControll_Sliding::Exit(SoccerPlayer* s)
{

	delete m_pSlidingClass;
}
void SoccerState_PlayerControll_Attack::Enter(SoccerPlayer* s)
{
	
	class SoccerAttackEvent :public CharacterNearAttack::AttackEvent
	{
		SoccerPlayer* m_pSoccer;
		DamageShpere m_damage;
	public:
		SoccerAttackEvent(SoccerPlayer* pSoccer) :
			m_pSoccer(pSoccer){

			m_damage.pParent = m_pSoccer;
			m_damage.pBall = NULL;
			m_damage.type = DamageBase::_WeekDamage;
			m_damage.Value = 20.0f;
			m_damage.m_Enable = false;
		}

		void Update()
		{
			m_pSoccer->m_Renderer.Update(1);
		}

		void AttackStart()
		{
			m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_Atk1);
		}

		void AttackEnd()
		{
			m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_Stand);
		}
		void Assault()
		{
			m_damage.m_Enable = true;
		}
	};
	


	p.TurnSpeed = 0.1f;
	p.AttackFrame = 15;
	p.EndFrame = 35;
	p.damage = 20;
	p.speed = 0.2f;
	p.CanHitNum = 1;
	p.HitCenter = 10.5f;

	timer = 0;

	m_pMoveClass = new CharacterNearAttack(s, p, new SoccerAttackEvent(s), DamageBase::Type::_WeekDamage, 1);
}
void SoccerState_PlayerControll_Attack::Execute(SoccerPlayer* s)
{
	Vector2 st = controller::GetStickValue(controller::stick::left, s->m_PlayerInfo.number);
	timer++;

	m_pMoveClass->SetStickValue(st);
	
	if (!m_pMoveClass->Update())
	{
		s->SetState(new SoccerState_PlayerControll_Move);
	}
	if (timer>p.AttackFrame && timer < p.EndFrame && controller::GetTRG(controller::button::shikaku, s->m_PlayerInfo.number))
	{
		s->SetState(new SoccerState_PlayerControll_AttackCombo);
	}
	chr_func::CreateTransMatrix(s, 0.05f, &s->m_Renderer.m_TransMatrix);
	
}
void SoccerState_PlayerControll_Attack::Exit(SoccerPlayer* s)
{
	delete m_pMoveClass;
}
void SoccerState_PlayerControll_AttackCombo::Enter(SoccerPlayer* s)
{
	class SoccerAttackEvent :public CharacterNearAttack::AttackEvent
	{
		SoccerPlayer* m_pSoccer;
	public:
		SoccerAttackEvent(SoccerPlayer* pSoccer) :
			m_pSoccer(pSoccer){}

		void Update()
		{
			m_pSoccer->m_Renderer.Update(1);
		}

		void AttackStart()
		{
			m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_Atk2);
		}

		void AttackEnd()
		{
			m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_Stand);
		}
		void Assault()
		{

		}
	};

	p.TurnSpeed = 0.1f;
	p.AttackFrame = 15;
	p.EndFrame = 35;
	p.damage = 20;
	p.speed = 0.2f;
	p.CanHitNum = 1;
	p.HitCenter = 10.5f;

	timer = 0;

	m_pMoveClass = new CharacterNearAttack(s, p, new SoccerAttackEvent(s), DamageBase::Type::_WeekDamage, 1);
}
void SoccerState_PlayerControll_AttackCombo::Execute(SoccerPlayer* s)
{
	Vector2 st = controller::GetStickValue(controller::stick::left, s->m_PlayerInfo.number);
	timer++;

	m_pMoveClass->SetStickValue(st);
	if (!m_pMoveClass->Update())
	{
		s->SetState(new SoccerState_PlayerControll_Move);
	}
	if (timer>p.AttackFrame && timer < p.EndFrame && controller::GetTRG(controller::button::shikaku, s->m_PlayerInfo.number))
	{
		s->SetState(new SoccerState_PlayerControll_AttackFinish);
	}

	chr_func::CreateTransMatrix(s, 0.05f, &s->m_Renderer.m_TransMatrix);
	
}
void SoccerState_PlayerControll_AttackCombo::Exit(SoccerPlayer* s)
{
	delete m_pMoveClass;
}
void SoccerState_PlayerControll_AttackFinish::Enter(SoccerPlayer* s)
{
	class SoccerAttackEvent :public CharacterNearAttack::AttackEvent
	{
		SoccerPlayer* m_pSoccer;
	public:
		SoccerAttackEvent(SoccerPlayer* pSoccer) :
			m_pSoccer(pSoccer){}

		void Update()
		{
			m_pSoccer->m_Renderer.Update(1);
		}

		void AttackStart()
		{
			m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_Atk3);
		}

		void AttackEnd()
		{
			m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_Stand);
		}
		void Assault()
		{

		}
	};

	p.TurnSpeed = 0.1f;
	p.AttackFrame = 15;
	p.EndFrame = 35;
	p.damage = 20;
	p.speed = 0.2f;
	p.CanHitNum = 1;
	p.HitCenter = 10.5f;

	timer = 0;

	p.TurnSpeed = 0.1f;

	m_pMoveClass = new CharacterNearAttack(s, p, new SoccerAttackEvent(s), DamageBase::Type::_VanishDamage, 1);
}
void SoccerState_PlayerControll_AttackFinish::Execute(SoccerPlayer* s)
{
	
	Vector2 st = controller::GetStickValue(controller::stick::left, s->m_PlayerInfo.number);
	timer++;

	m_pMoveClass->SetStickValue(st);
	if (!m_pMoveClass->Update())
	{
		s->SetState(new SoccerState_PlayerControll_Move);
	}

	chr_func::CreateTransMatrix(s, 0.05f, &s->m_Renderer.m_TransMatrix);

}
void SoccerState_PlayerControll_AttackFinish::Exit(SoccerPlayer* s)
{
	delete m_pMoveClass;
}
void SoccerState_PlayerControll_Shot::Enter(SoccerPlayer* s)
{
	class SoccerShotEvent :public CharacterShotAttack::Event
	{
		SoccerPlayer* m_pSoccer;
	public:
		SoccerShotEvent(SoccerPlayer* pSoccer) :
			m_pSoccer(pSoccer){}

		void Update()
		{
			m_pSoccer->m_Renderer.Update(1);
		
		}

		void AttackStart()
		{
			m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_Shot);
		}

		void AttackEnd()
		{
			m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_Stand);
		}
		void Shot()
		{
			BallBase::Params param;

			chr_func::GetFront(m_pSoccer, &param.move);
			param.move *= 0.5f;
			param.pos = m_pSoccer->m_Params.pos;
			param.pos.y = BallBase::UsualBallShotY;
			param.pParent = m_pSoccer;
			param.type = BallBase::Type::_Usual;

			new UsualBall(param, DamageBase::Type::_WeekDamage, 1);
		}
	};
	class SoccerHitEvent :public DamageManager::HitEventBase
	{
	public:
		SoccerHitEvent(SoccerPlayer* ps) :m_pSoccer(ps){}

		bool Hit(DamageBase* pDmg)
		{
			//�����������ɌĂ΂��֐�(�߂�l�F�����������ǂ���)
			//�����̍���Ă���_���[�W�������ꍇ�͉������Ȃ�
			if (pDmg->pParent->m_PlayerInfo.number == m_pSoccer->m_PlayerInfo.number)
			{
				return false;
			}

			//�����������ɂ��̃_���[�W�̎�ނ���A���ꂼ��̃X�e�[�g�ɔh��������
			switch (pDmg->type)
			{
			case DamageBase::Type::_WeekDamage:
				//��U��
				//m_pSoccer->SetState();
				return true;

				/*
				//���쐬
				case DamageBase::Type::_VanishDamage:
				//������у_���[�W
				m_pSoccer->SetState();
				return true;
				case DamageBase::Type::_UpDamage:
				//��ɐ������
				m_pSoccer->SetState();
				return true;
				*/

			default:break;
			}

			return false;

		}
	private:
		SoccerPlayer* m_pSoccer;
	};
	CharacterShotAttack::AttackParams p;

	p.ShotFrame = 10;
	p.AllFrame = 35;
	p.MoveDownSpeed = 0.2f;

	m_pShotClass = new CharacterShotAttack(s, new SoccerShotEvent(s), p, new SoccerHitEvent(s));
}
void SoccerState_PlayerControll_Shot::Execute(SoccerPlayer* s)
{

	
	if (!m_pShotClass->Update())
	{
		s->SetState(new SoccerState_PlayerControll_Move);
	}
	chr_func::CreateTransMatrix(s, 0.05f, &s->m_Renderer.m_TransMatrix);
}
void SoccerState_PlayerControll_Shot::Exit(SoccerPlayer* s)
{
	delete m_pShotClass;
}
void SoccerState_PlayerControll_Counter::Enter(SoccerPlayer* s)
{
	// �J�E���^�[�N���X�쐬
	m_pCounterClass = CreateCounterClass(s);
}
void SoccerState_PlayerControll_Counter::Execute(SoccerPlayer* s)
{
	
}
void SoccerState_PlayerControll_Counter::Exit(SoccerPlayer* s)
{
	delete m_pCounterClass;
}
CharacterCounter* SoccerState_PlayerControll_Counter::CreateCounterClass(SoccerPlayer* s)
{
	class SoccerCounterEvent :public CharacterCounter::CounterEvent
	{
		SoccerPlayer* m_pSoccer;
		int m_CounterLevel;
	public:
		SoccerCounterEvent(SoccerPlayer* pSoccer) :
			m_pSoccer(pSoccer),
			m_CounterLevel(0){};

		// �X�V
		void Update()override
		{
			// ���f���X�V
			m_pSoccer->m_Renderer.Update(1);

			// �]���s��X�V
			chr_func::CreateTransMatrix(
				m_pSoccer,
				0.05f,
				&m_pSoccer->m_Renderer.m_TransMatrix);
		}

		// �\���J�n
		void PoseStart()override
		{
			m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_Counter);
		}

		// �\���I��
		void PoseEnd()override
		{// �J�E���^�[
			m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_Shot2);
		}

		// �J�E���^�[�ł���{�[�������ꂽ
		void BallEnter()override
		{

		}

		// ���x���A�b�v
		void LevelUp(int level)override
		{
			m_CounterLevel = level;
		}

		// �J�E���^�[�J�n
		void SwingStart()override
		{
			m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_Counter);
		}

		// �J�E���^�[�I��
		void SwingEnd()override
		{// �ʏ�ړ���
			m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_Stand);
			m_pSoccer->SetState(new SoccerState_PlayerControll_Move());
		}

		// �ł��Ԃ����Ƃ�
		void HitBall(bool is_just)override
		{
			//MyDebugString("Hit Ball.\n");
		}
	};
	// �p�����[�^�ݒ�
	CharacterCounter::CounterParams p;

	p.MaxPoseFrame = 60;
	p.LevelUpFrame = 45;
	p.MoveDownSpeed = 0.2f;

	return new CharacterCounter(
		s,
		p,
		new SoccerCounterEvent(s)
		);
}
void SoccerState_PlayerControll_Dash::Enter(SoccerPlayer* s)
{
	class SoccerDashEvent :public SoccerDash::MoveEvent
	{
		SoccerPlayer* m_pSoccer;
	public:
		SoccerDashEvent(SoccerPlayer* pSoccer) :
			m_pSoccer(pSoccer){}

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

	SoccerDash::Params p;

	p.Acceleration = 0.2f;
	p.MaxSpeed = 0.7f;
	p.TurnSpeed = 0.03f;
	p.DownSpeed = 0.2f;

	m_pMoveClass = new SoccerDash(s, p, new SoccerDashEvent(s), new DamageManager::HitEventBase());
}
void SoccerState_PlayerControll_Dash::Execute(SoccerPlayer* s)
{
	Vector2 st = controller::GetStickValue(controller::stick::left, s->m_PlayerInfo.number);
	
	// [L1]�����Ɩ߂�
	if (!controller::GetPush(controller::button::_L1, s->m_PlayerInfo.number))
	{
		s->SetState(new SoccerState_PlayerControll_Move());
	}
	else
	{
		m_pMoveClass->SetStickValue(st);
	}

	
	m_pMoveClass->Update();

	chr_func::CreateTransMatrix(s, 0.05f, &s->m_Renderer.m_TransMatrix);
}
void SoccerState_PlayerControll_Dash::Exit(SoccerPlayer* s)
{
	delete m_pMoveClass;
}

void SoccerState_SmallDamage(SoccerPlayer* s)
{

}
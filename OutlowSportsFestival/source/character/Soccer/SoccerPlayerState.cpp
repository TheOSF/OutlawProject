#include "SoccerPlayerState.h"
#include "SoccerCommonState.h"
#include "SoccerHitEvent.h"
#include "SoccerPlayerState_PoseMotion.h"
#include "SoccerRolling.h"
#include "Snakeshot.h"
#include "../../Effect/EffectFactory.h"
#include "../Soccer/Computer/SoccerComputerMove.h"


#include "../CharacterDefaultCounterClass.h"
#include "../../GameSystem/GameController.h"
#include "../CharacterManager.h"
#include "../CharacterFunction.h"
#include "../CharacterBase.h"
#include "../../Sound/Sound.h"
#include "../../Camera/Camera.h"

class SoccerUtillityClass
{
public:
	//���[�����O�̕�������N���X
	class PlayerRollingControll :public SoccerState_Rolling::CallBackClass
	{
	public:
		SoccerPlayer*const ps;

		PlayerRollingControll(SoccerPlayer* ps) :ps(ps){}


		Vector3 GetVec()override
		{
			Vector2 stick = controller::GetStickValue(controller::stick::left, ps->m_PlayerInfo.number);
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
SoccerState* SoccerState_PlayerControll_Move::GetPlayerControllMove(
	SoccerPlayer* ps)
{
	switch (ps->m_PlayerInfo.player_type)
	{
	case PlayerType::_Player:
		return new SoccerState_PlayerControll_Move();

	case PlayerType::_Computer:

		return new SoccerState_ComputerControll_Move();

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

	assert("�ʏ�X�e�[�g���쐬�ł��Ȃ��L�����N�^�^�C�v�ł� SoccerState_PlayerControll_Move::GetPlayerControllMove" && 0);
	return nullptr;
}
bool SoccerState_PlayerControll_Move::SwitchGameState(SoccerPlayer* ps)
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
void SoccerState_PlayerControll_Move::ActionStateSwitch(SoccerPlayer*s)
{
	float movelen = s->m_Params.move.Length();
	// [�~] �� ���[�����O
	if (controller::GetTRG(controller::button::batu, s->m_PlayerInfo.number))
	{
		s->SetState(new SoccerState_Rolling(new SoccerUtillityClass::PlayerRollingControll(s), false));
	}
	// [��] �� �i��
	if (controller::GetTRG(controller::button::shikaku, s->m_PlayerInfo.number))
	{
		s->SetState(new SoccerState_PlayerControll_Attack(s));
	}
	// [��] �� �V���b�g
	if (controller::GetTRG(controller::button::sankaku, s->m_PlayerInfo.number))
	{
		s->SetState(new SoccerState_PlayerControll_Shot);
	}
	// [�Z] �� �K�E�Z
	if (controller::GetTRG(controller::button::maru, s->m_PlayerInfo.number))
	{
		s->SetState(new SoccerState_PlayerControll_Finisher);
	}
	// [R1] �� �J�E���^�[
	if (controller::GetTRG(controller::button::_R1, s->m_PlayerInfo.number))
	{
		s->SetState(new SoccerState_PlayerControll_Counter());
	}
	// [L1] �� �ŗL�Z(�_�b�V��)
	if (controller::GetPush(controller::button::_L1, s->m_PlayerInfo.number) && movelen>0.18f)
	{
		s->SetState(new SoccerState_PlayerControll_Dash());
	}
}
//-------------�ړ��X�e�[�g�N���X-------------
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

	p.Acceleration = 0.17f;
	p.MaxSpeed = 0.29f;
	p.TurnSpeed = 0.3f;
	p.DownSpeed = 0.2f;

	m_pMoveClass = new CharacterUsualMove(
		s, 
		p, 
		new SoccerMoveEvent(s),
		new SoccerHitEvent(s)
		);
	//�����̂������[�V�����Z�b�g
	s->m_Renderer.SetMotion(SoccerPlayer::_ms_Stand);
}
void SoccerState_PlayerControll_Move::Execute(SoccerPlayer* s)
{
	if (SwitchGameState(s) == false)
	{
		//�e�A�N�V�����ֈڍs�\
		ActionStateSwitch(s);
		Vector2 st = controller::GetStickValue(controller::stick::left, s->m_PlayerInfo.number);
		Vector3 st_vec3;

		//�r���[��Ԃɕϊ�
		st_vec3 = DefCamera.GetRight()*st.x + DefCamera.GetForward()*st.y;
		st.x = st_vec3.x;
		st.y = st_vec3.z;

		//�X�e�B�b�N�̒l�Z�b�g
		m_pMoveClass->SetStickValue(st);

	}
	else
	{
		//�X�e�B�b�N�̒l�Z�b�g
		m_pMoveClass->SetStickValue(Vector2(0, 0));
	}
	
	//�X�V
	m_pMoveClass->Update();


	//���f���̃��[���h�ϊ��s����X�V
	chr_func::CreateTransMatrix(s, 0.05f, &s->m_Renderer.m_TransMatrix);
}
void SoccerState_PlayerControll_Move::Exit(SoccerPlayer* s)
{
	delete m_pMoveClass;
}

//-------------�X���C�f�B���O�X�e�[�g�N���X-------------
SoccerState_PlayerControll_Sliding::PlayerControllEvent::PlayerControllEvent(SoccerPlayer*const pSoccer) :
m_pSoccer(pSoccer)
{

}
bool SoccerState_PlayerControll_Sliding::PlayerControllEvent::isDoCombo()
{
	return controller::GetTRG(controller::button::shikaku, m_pSoccer->m_PlayerInfo.number);
}
void SoccerState_PlayerControll_Sliding::PlayerControllEvent::AngleControll(RADIAN angle)
{
	const CharacterBase* const pTargetCharacter = GetFrontTargetEnemy();

	if (pTargetCharacter != nullptr)
	{
		//������]
		chr_func::AngleControll(m_pSoccer, pTargetCharacter->m_Params.pos, angle);
	}
	else
	{
		const Vector2 Stick = controller::GetStickValue(controller::stick::left, m_pSoccer->m_PlayerInfo.number);

		//�X�e�B�b�N�����ȏ�|����Ă��邩�ǂ���
		if (Vector2Length(Stick) > 0.25f)
		{
			Vector3 Vec(Stick.x, 0, Stick.y);

			//�X�e�B�b�N�l���J������Ԃ�
			Vec = Vector3MulMatrix3x3(Vec, matView);

			//�L�����N�^��]
			chr_func::AngleControll(m_pSoccer, m_pSoccer->m_Params.pos + Vec, angle);
		}
	}
}

const CharacterBase* SoccerState_PlayerControll_Sliding::PlayerControllEvent::GetFrontTargetEnemy()
{
	CharacterManager::CharacterMap ChrMap = DefCharacterMgr.GetCharacterMap();

	const float  AutoDistance = 10.0f;               //�������ł���ő勗��
	const RADIAN AutoMaxAngle = D3DXToRadian(90);   //�������ł���ő�p�x

	const CharacterBase* pTargetEnemy = nullptr;    //�^�[�Q�b�g�ێ��̃|�C���^
	RADIAN MostMinAngle = PI;                       //�����Ƃ������p�x
	RADIAN TempAngle;

	Vector3 MyFront;      //���g�̑O���x�N�g��
	chr_func::GetFront(m_pSoccer, &MyFront);

	auto it = ChrMap.begin();

	while (it != ChrMap.end())
	{
		//���g�����O
		if (m_pSoccer->m_PlayerInfo.number == it->first->m_PlayerInfo.number ||
			chr_func::isDie(it->first)
			)
		{
			++it;
			continue;
		}

		//���������ȏ�̃L�����N�^�����O����
		if (Vector3Distance(it->first->m_Params.pos, m_pSoccer->m_Params.pos) > AutoDistance)
		{
			it = ChrMap.erase(it);
			continue;
		}

		//�O�x�N�g���ƓG�ւ̃x�N�g���̊p�x���v�Z����
		TempAngle = Vector3Radian(MyFront, (it->first->m_Params.pos - m_pSoccer->m_Params.pos));

		//�p�x����ԋ���������X�V
		if (TempAngle < MostMinAngle)
		{
			pTargetEnemy = it->first;
			MostMinAngle = TempAngle;
		}

		++it;
	}

	return pTargetEnemy;

}
//-------------�X���C�f�B���O�U���X�e�[�g�N���X-------------
SoccerState_PlayerControll_Sliding::SoccerState_PlayerControll_Sliding(SoccerPlayer* s) :
m_Attack(s, new PlayerControllEvent(s),10)
{

}


SoccerState_PlayerControll_Sliding::~SoccerState_PlayerControll_Sliding()
{

}
void SoccerState_PlayerControll_Sliding::Enter(SoccerPlayer* s)
{
	//�U���N���X�쐬
	SoccerAttackInfo_UsualAtk* pAtk;

	SoccerAttackInfo_UsualAtk::Param AtkParam[] =
	{
		{ 6, 1.0f, 1.5f, DamageBase::Type::_VanishDamage, 5, 22, 0.17f, 5, 10, SoccerPlayer::_ms_Rolling, 35, 20, 27, 35, 0, 2, D3DXToRadian(1), 18 },
	};
	Sound::Play(Sound::Sand2);

	for (int i = 0; i < (int)ARRAYSIZE(AtkParam); ++i)
	{
		pAtk = new SoccerAttackInfo_UsualAtk(s);

		pAtk->m_Param = AtkParam[i];

		m_Attack.m_AttackInfoArray.push_back(pAtk);
	}
}
void SoccerState_PlayerControll_Sliding::Execute(SoccerPlayer* s)
{
	m_Attack.Update();

	if (m_Attack.isEnd())
	{
		s->SetState(SoccerState_PlayerControll_Move::GetPlayerControllMove(s));
	}
}
void SoccerState_PlayerControll_Sliding::Exit(SoccerPlayer* s)
{

}

//------------�v���C���[����̍U������N���X--------------

SoccerState_PlayerControll_Attack::PlayerControllEvent::PlayerControllEvent(SoccerPlayer*const pSoccer) :
m_pSoccer(pSoccer)
{

}

bool SoccerState_PlayerControll_Attack::PlayerControllEvent::isDoCombo()
{
	return controller::GetTRG(controller::button::shikaku, m_pSoccer->m_PlayerInfo.number);
}

void SoccerState_PlayerControll_Attack::PlayerControllEvent::AngleControll(RADIAN angle)
{
	const CharacterBase* const pTargetCharacter = GetFrontTargetEnemy();

	if (pTargetCharacter != nullptr)
	{
		//������]
		chr_func::AngleControll(m_pSoccer, pTargetCharacter->m_Params.pos, angle);
	}
	else
	{
		const Vector2 Stick = controller::GetStickValue(controller::stick::left, m_pSoccer->m_PlayerInfo.number);

		//�X�e�B�b�N�����ȏ�|����Ă��邩�ǂ���
		if (Vector2Length(Stick) > 0.25f)
		{
			Vector3 Vec(Stick.x, 0, Stick.y);

			//�X�e�B�b�N�l���J������Ԃ�
			Vec = Vector3MulMatrix3x3(Vec, matView);

			//�L�����N�^��]
			chr_func::AngleControll(m_pSoccer, m_pSoccer->m_Params.pos + Vec, angle);
		}
	}
}

const CharacterBase* SoccerState_PlayerControll_Attack::PlayerControllEvent::GetFrontTargetEnemy()
{
	CharacterManager::CharacterMap ChrMap = DefCharacterMgr.GetCharacterMap();

	const float  AutoDistance = 10.0f;               //�������ł���ő勗��
	const RADIAN AutoMaxAngle = D3DXToRadian(90);   //�������ł���ő�p�x

	const CharacterBase* pTargetEnemy = nullptr;    //�^�[�Q�b�g�ێ��̃|�C���^
	RADIAN MostMinAngle = PI;                       //�����Ƃ������p�x
	RADIAN TempAngle;

	Vector3 MyFront;      //���g�̑O���x�N�g��
	chr_func::GetFront(m_pSoccer, &MyFront);

	auto it = ChrMap.begin();

	while (it != ChrMap.end())
	{
		//���g�����O
		if (m_pSoccer->m_PlayerInfo.number == it->first->m_PlayerInfo.number ||
			chr_func::isDie(it->first)
			)
		{
			++it;
			continue;
		}

		//���������ȏ�̃L�����N�^�����O����
		if (Vector3Distance(it->first->m_Params.pos, m_pSoccer->m_Params.pos) > AutoDistance)
		{
			it = ChrMap.erase(it);
			continue;
		}

		//�O�x�N�g���ƓG�ւ̃x�N�g���̊p�x���v�Z����
		TempAngle = Vector3Radian(MyFront, (it->first->m_Params.pos - m_pSoccer->m_Params.pos));

		//�p�x����ԋ���������X�V
		if (TempAngle < MostMinAngle)
		{
			pTargetEnemy = it->first;
			MostMinAngle = TempAngle;
		}

		++it;
	}

	return pTargetEnemy;

}

//-------------�ߋ����U���X�e�[�g�N���X-------------
SoccerState_PlayerControll_Attack::SoccerState_PlayerControll_Attack(SoccerPlayer* s) :
m_Attack(s, new PlayerControllEvent(s),0)
{

}


SoccerState_PlayerControll_Attack::~SoccerState_PlayerControll_Attack()
{

}
void SoccerState_PlayerControll_Attack::Enter(SoccerPlayer* s)
{
	//�U���N���X�쐬
	SoccerAttackInfo_UsualAtk* pAtk;

	SoccerAttackInfo_UsualAtk::Param AtkParam[] =
	{
		{ 6, 1.0f, 1.5f, DamageBase::Type::_WeekDamage, 3, 20, 0.07f, 5, 10, SoccerPlayer::_ms_Atk1, 50, 20, 27, 35, 0, 5, D3DXToRadian(5), 21 },
		{ 2, 1.0f, 1.5f, DamageBase::Type::_WeekDamage, 5, 18, 0.02f, 1, 5, SoccerPlayer::_ms_Atk2, 35, 5, 15, 20, 0, 5, D3DXToRadian(5), 3 },
		{ 8, 1.0f, 1.5f, DamageBase::Type::_VanishDamage, 8, 16, 0.05f, 1, 6, SoccerPlayer::_ms_Atk3, 40, -1, -1, -1, 0, 8, D3DXToRadian(5), 21 },
	};

	for (int i = 0; i < (int)ARRAYSIZE(AtkParam); ++i)
	{
		pAtk = new SoccerAttackInfo_UsualAtk(s);

		pAtk->m_Param = AtkParam[i];

		m_Attack.m_AttackInfoArray.push_back(pAtk);
	}
}
void SoccerState_PlayerControll_Attack::Execute(SoccerPlayer* s)
{
	m_Attack.Update();

	if (m_Attack.isEnd())
	{
		s->SetState(SoccerState_PlayerControll_Move::GetPlayerControllMove(s));
	}
}
void SoccerState_PlayerControll_Attack::Exit(SoccerPlayer* s)
{
	
}

//-------------�ˌ��U���X�e�[�g�N���X-------------
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
			Sound::Play(Sound::Impact2);
			new UsualBall(param, DamageBase::Type::_WeekDamage, 10);
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
	//��{�I�ȍX�V
	SoccerHitEvent HitEvent(s);
	chr_func::UpdateAll(s, &HitEvent);
	//���f���֘A�̍X�V
	s->m_Renderer.Update(1);
	chr_func::CreateTransMatrix(s, 0.05f, &s->m_Renderer.m_TransMatrix);
}
void SoccerState_PlayerControll_Shot::Exit(SoccerPlayer* s)
{
	delete m_pShotClass;
}
//-------------�J�E���^�[�X�e�[�g�N���X-------------
void SoccerState_PlayerControll_Counter::Enter(SoccerPlayer* s)
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
void SoccerState_PlayerControll_Counter::Execute(SoccerPlayer* s)
{
	m_pCounter->SetStickValue(controller::GetStickValue(controller::stick::left, s->m_PlayerInfo.number));
	//�X�V
	m_pCounter->Update();

	//���f���̃��[���h�ϊ��s����X�V
	chr_func::CreateTransMatrix(s, 0.05f, &s->m_Renderer.m_TransMatrix);

	//���f���X�V
	s->m_Renderer.Update(1);
}
void SoccerState_PlayerControll_Counter::Exit(SoccerPlayer* s)
{
	delete m_pCounter;
}
//-------------�_�b�V���X�e�[�g�N���X-------------

void SoccerState_PlayerControll_Dash::Enter(SoccerPlayer* s)
{
	class SoccerDashEvent 
	{
		SoccerPlayer* m_pSoccer;
	public:
		SoccerDashEvent(SoccerPlayer* pSoccer) :
			m_pSoccer(pSoccer){}

		void Update(bool isRun, RATIO speed_ratio)
		{
			m_pSoccer->m_Renderer.Update(1.5f);
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
	m_timer = 0;
	Sound::Play(Sound::Soccer_Speed_Up1);
	m_pMoveClass = new SoccerDash(
		s
		);
}
void SoccerState_PlayerControll_Dash::Execute(SoccerPlayer* s)
{

	
	++m_timer;


	Vector2 st = controller::GetStickValue(controller::stick::left, s->m_PlayerInfo.number);

	
	// [L1]���� / �J�[�\���|���Ȃ��Ɩ߂�
	if (!controller::GetPush(controller::button::_L1, s->m_PlayerInfo.number) || (st.x==0 &&st.y==0))
	{
		s->SetState(new SoccerState_brake(s));
	}
	else
	{
		m_pMoveClass->SetStickValue(st);
	}
	// [��] �� �V���b�g
	if (controller::GetTRG(controller::button::sankaku, s->m_PlayerInfo.number))
	{
		s->SetState(new SoccerState_PlayerControll_Shot);
	}
	// [�~] �� ���[�����O
	if (controller::GetTRG(controller::button::batu, s->m_PlayerInfo.number))
	{
		s->SetState(new SoccerState_Rolling(new SoccerUtillityClass::PlayerRollingControll(s),true));
	}
	// [��] �� �X���C�f�B���O
	if (controller::GetTRG(controller::button::shikaku, s->m_PlayerInfo.number))
	{
		s->SetState(new SoccerState_PlayerControll_Sliding(s));
	}
	// [�Z] �� �K�E�Z
	if (controller::GetTRG(controller::button::maru, s->m_PlayerInfo.number))
	{
		s->SetState(new SoccerState_PlayerControll_Finisher);
	}
	// [R1] �� �K�E�Z
	if (controller::GetTRG(controller::button::_R1, s->m_PlayerInfo.number))
	{
		s->SetState(new SoccerState_PlayerControll_Counter());
	}

	if (m_timer % 19==5)
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
SoccerState_PlayerControll_Finisher::SoccerState_PlayerControll_Finisher() :
m_pSnakeShotClass(nullptr)
{

}
void SoccerState_PlayerControll_Finisher::Enter(SoccerPlayer* s)
{
	m_pSnakeShotClass = this->SnakeShotClass(s);
}
void SoccerState_PlayerControll_Finisher::Execute(SoccerPlayer* s)
{
	m_pSnakeShotClass->SetStickValue(
		controller::GetStickValue(controller::stick::left, s->m_PlayerInfo.number));

	// �X�V
	if (m_pSnakeShotClass->Update() == false)
	{
		return;
	}

}
void SoccerState_PlayerControll_Finisher::Exit(SoccerPlayer* s)
{
	delete m_pSnakeShotClass;
}
//�@�������N���X
CharacterShotAttack* SoccerState_PlayerControll_Finisher::SnakeShotClass(SoccerPlayer* s){
	class ShotAttackEvent :public CharacterShotAttack::Event{
		SoccerPlayer* m_pSoccer;//�@�싅
		Snakeshot* snake;
	public:
		//�@�{�[��
		BallBase::Params param;
		//�@�^�[�Q�b�g
		Vector3 target;
	public:
		//�@�R���X�g���N�^
		ShotAttackEvent(SoccerPlayer* pSoccer) :target(0, 0, 0),
			m_pSoccer(pSoccer){}
		//�@�X�V
		void Update()override{
			//�@���f���X�V
			m_pSoccer->m_Renderer.Update(1.0f);

			// �]���s��X�V
			chr_func::CreateTransMatrix(
				m_pSoccer,
				0.05f,
				&m_pSoccer->m_Renderer.m_TransMatrix);
		}
	public:
		// �_���[�W����J�n & �{�[������
		void Shot()
		{
			//�@�������U��(param�v�Z)
			BallBase::Params param;

			chr_func::GetFront(m_pSoccer, &param.move);
			
			param.pos = m_pSoccer->m_Params.pos;
			param.pos.y = UsualBall::UsualBallShotY;

			param.pParent = m_pSoccer;
			param.scale = 1.0f;
			param.type = BallBase::Type::_Usual;

			//����
			Sound::Play(Sound::Beam2);
			new Snakeshot(param, 1);
		}

		//�@�������U���J�n
		void AttackStart()override{
			//�@�����[�V����
			m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_Shot);
		}

		void AttackEnd()
		{
			//�U���I�����ɒʏ�ړ����[�h�ɖ߂�
			m_pSoccer->SetState(new SoccerState_PlayerControll_Move());
		}
	};

	CharacterShotAttack::AttackParams atk;

	atk.AllFrame = 40;
	atk.AttackPower = 8;
	atk.MaxTurnRadian = PI / 4;
	atk.MoveDownSpeed = 0.8f;
	atk.ShotFrame = 15;

	return m_pSnakeShotClass = new CharacterShotAttack(
		s,
		new ShotAttackEvent(s),
		atk,
		new  SoccerHitEvent(s)
		);
}



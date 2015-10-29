#include "BaseballPlayerState_ComMove.h"
#include "../BaseballState_PlayerControll_ShotAttack_B.h"
#include "../BaseballState_PlayerControll_ShotAttack_P.h"
#include "../BaseballState_Change.h"
#include "../BaseballPlayerState_Counter.h"
#include "../../../GameSystem/GameController.h"
#include "../../CharacterFunction.h"
#include "../../../Ball/UsualBall.h"
#include "../../../Damage/Damage.h"
#include "../../CharacterCounterClass.h"
#include "../../CharacterMoveClass.h"
#include "../../CharacterShotAttackClass.h"
#include "../../CharacterManager.h"
#include "../Baseball_HitEvent.h"
#include "../BaseballPlayerState_Attack_B.h"
#include "../BaseballPlayerState_Attack_P.h"



//�X�e�[�g�J�n
void BaseballPlayerState_ComMove::Enter(BaseballPlayer* b)
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

	//�ړ��N���X�̍쐬
	m_pMoveClass = new CharacterUsualMove(
		b,
		p,
		new BaseballMoveEvent(b),
		new BaseballHitEvent(b)
		);


	//�ړ��R���g���[���N���X�̍쐬
	if (b->m_PlayerInfo.strong_type == StrongType::__ErrorType)
	{
		m_pMoveControllClass = new SandBagMove_Com_ControllClass();
	}
	else
	{
		m_pMoveControllClass = new ComBaseballMoveControllClass(b);
	}

}


void BaseballPlayerState_ComMove::Execute(BaseballPlayer* b)
{
	//�X�e�B�b�N�l���Z�b�g
	m_pMoveClass->SetStickValue(m_pMoveControllClass->Move(b));

	//�@�؂�ւ�
	SetBatterFlg(b);

	//�@���s�p�^�[��
	if (batterflg){
		//�@�o�b�^�[��
		Batter(b);
	}
	else{
		//�@���莞
		Pitcher(b);
	}

	//�@�X�V
	//�@����
	m_pMoveClass->Update();

	//���f���̃��[���h�ϊ��s����X�V
	chr_func::CreateTransMatrix(b, b->m_ModelSize, &b->m_Renderer.m_TransMatrix);

}

void BaseballPlayerState_ComMove::Exit(BaseballPlayer* b)
{
	delete m_pMoveClass;
	delete m_pMoveControllClass;
}


//�@�o�b�^�[��
void BaseballPlayerState_ComMove::Batter(BaseballPlayer* b)
{

	//b->SetState(new BaseballState_PlayerControll_ShotAttack_B((new PlayerShotControllClass_B(b)));
		
}

//�@���莞
void  BaseballPlayerState_ComMove::Pitcher(BaseballPlayer* b)
{
	static int time = 0;
	time++;
	if (time >= 120)
	{
		b->SetState(new BaseballState_PlayerControll_ShotAttack_P());
		time = 0;
	}
}



//---------------------------------------------------------------------
//                      SandBagMoveControllClass
//---------------------------------------------------------------------

Vector2 SandBagMove_Com_ControllClass::Move(BaseballPlayer* b)
{
	//�X�e�B�b�N�̒l�Ƃ��ăR���s���[�^����̈ړ����v�Z����
	const Vector3 Target = DefCharacterMgr.GetRoundStartPos(b->m_PlayerInfo.number);
	Vector3 v = Target - b->m_Params.pos;

	if (v.Length() < 1.0f)
	{
		v = Vector3Zero;
	}

	return Vector2Normalize(Vector2(v.x, v.z));
}

//---------------------------------------------------------------------
//                      ComBaseballMoveControllClass
//---------------------------------------------------------------------

//�@�R���X�g���N�^
ComBaseballMoveControllClass::ComBaseballMoveControllClass(BaseballPlayer* b) :
m_pBaseball(b)
{
	//�@�s���^�C�v
	m_Type = 1;
	m_Vec = Vector3Zero;

	//�@�e�p�����[�^�Z�b�g
	GetParams(m_Param, b->m_PlayerInfo.strong_type);
	//�@�����֐��Z�b�g
	m_pStateFunc = &ComBaseballMoveControllClass::StateChoise;
	//�@�����l�Z�b�g
	m_Count = rand() % 20;
	m_MoveTargetPos = Vector3Zero;
}

ComBaseballMoveControllClass::~ComBaseballMoveControllClass()
{

}

//�@�ړ��p�^�[���I��
void  ComBaseballMoveControllClass::StateChoise(Vector2&)
{
	switch (m_Type)
	{
		// �X�^�[�g�n�_��
	case 0:
		m_pStateFunc = &ComBaseballMoveControllClass::StateMove_StartPos;
		break;
		//�@�^�[�Q�b�g(�v���C���[)�̂Ƃ����
	case 1:
		m_pStateFunc = &ComBaseballMoveControllClass::StateMove_TargetPos;
		break;

	default:
		break;
	}
	
}

Vector2 ComBaseballMoveControllClass::Move(BaseballPlayer* b)
{
	//�@�o�b�^�[�ƃs�b�`���[�؂�ւ�
	Change(b);

	(this->*m_pStateFunc)(Vector2(m_Vec.x, m_Vec.z));

	return Vector2Normalize(Vector2(m_Vec.x, m_Vec.z));
}

//�@�؂�ւ�
void ComBaseballMoveControllClass::Change(BaseballPlayer* b)
{
	nearpos = GetTargetPos() - b->m_Params.pos;
	
	if (nearpos.Length() < 10.0f && !b->getBatterFlg() ||
		nearpos.Length() > 41.0f && b->getBatterFlg())
	{
		b->SetState(new BaseballState_Change());
	}

}

void ComBaseballMoveControllClass::GetParams(Param& out, StrongType::Value st)
{
	switch (st)
	{
		//�ア
	case StrongType::_Weak:
		out.BallCounter = 0.3f;
		out.BallCounterSpeed = 0.5f;
		out.BallCounterTec = 0.1f;
		out.RunStop = 0.8f;
		out.RunPlaceTec = 0.1f;
		out.DangerEscape = 0.2f;
		out.ActionFrequence = 0.3f;
		out.NamePlay = 1.0f;
		break;

		//����
	case StrongType::_Usual:
		out.BallCounter = 0.6f;
		out.BallCounterSpeed = 0.7f;
		out.BallCounterTec = 0.7f;
		out.RunStop = 0.4f;
		out.RunPlaceTec = 0.5f;
		out.DangerEscape = 0.5f;
		out.ActionFrequence = 0.6f;
		out.NamePlay = 0.5f;
		break;

		//����
	case StrongType::_Strong:
		out.BallCounter = 1.0f;
		out.BallCounterSpeed = 1.0f;
		out.BallCounterTec = 1.0f;
		out.RunStop = 0.2f;
		out.RunPlaceTec = 1.0f;
		out.DangerEscape = 1.0f;
		out.ActionFrequence = 1.0f;
		out.NamePlay = 0.0f;
		break;

	default:
		MyAssert(false, "�F���ł��Ȃ�strongType��ComMoveControllClass::GetParams�ɑ����܂���");
		break;
	}
}


//�@�X�^�[�g�ʒu�֖߂�
void ComBaseballMoveControllClass::StateMove_StartPos(Vector2& out)
{
	//�X�e�B�b�N�̒l�Ƃ��ăR���s���[�^����̈ړ����v�Z����
	const Vector3 Target = DefCharacterMgr.GetRoundStartPos(m_pBaseball->m_PlayerInfo.number);
	Vector3 v = Target - m_pBaseball->m_Params.pos;
	if (v.Length() < 1.0f)
	{
		v = Vector3Zero;
	}
	m_Vec.x = v.x;
	m_Vec.z = v.z;

}

//�@�^�[�Q�b�g(�L����)�̈ʒu��
void ComBaseballMoveControllClass::StateMove_TargetPos(Vector2& out)
{
	Vector3 v = GetTargetPos() - m_pBaseball->m_Params.pos;
	m_Vec.x = v.x;
	m_Vec.z = v.z;
}


void ComBaseballMoveControllClass::StateStop(Vector2& out)
{

}

//�@�^�[�Q�b�g(�L����)
Vector3 ComBaseballMoveControllClass::GetTargetPos()
{
	Vector3 v1, v2;

	const float HomingAngle = PI / 4;
	float MostHp = 0;
	float TempHp;
	Vector3 pTarget = Vector3Zero;

	//�@map���
	const CharacterManager::CharacterMap& chr_map =
		DefCharacterMgr.GetCharacterMap();


	for (auto it = chr_map.begin(); it != chr_map.end(); ++it)
	{
		//�@����ł�or�����Ȃ�continue
		if (chr_func::isDie(it->first) ||
			it->first->m_PlayerInfo.number == m_pBaseball->m_PlayerInfo.number)
		{
			continue;
		}

		TempHp = it->first->m_Params.HP;

		//�@��ԑ̗͂������L�����I��
		if (MostHp < TempHp)
		{
			MostHp = TempHp;
			pTarget = it->first->m_Params.pos;
		}
	}

	return pTarget;

}

Vector3 ComBaseballMoveControllClass::GetMoveTargetPos()
{
	Vector3 ret;

	//�싅�@���@������or�ߋ������L���Ȃ̂ŁA
	//�@�Ջ@���ςɐ؂�ւ���̂�Best

	struct TargetInfo
	{
		bool      ok;
		Vector3   pos;
	};

	TargetInfo targets[8];

	const CharacterManager::CharacterMap& ChrMap = DefCharacterMgr.GetCharacterMap();

	for (int i = 0; i < (int)ARRAYSIZE(targets); ++i)
	{
		//�����_���ɍ��W���쐬
		targets[i].pos = Vector3(frand() - 0.5f, 0, frand() - 0.5f) * 50.f;
		targets[i].pos += Vector3Normalize(targets[i].pos) * 5.0f;


		//���̓_���ǂ����ǂ���
		for (auto& it : ChrMap)
		{
			if (it.first == m_pBaseball ||
				chr_func::isDie(it.first))
			{
				continue;
			}

		}
		return targets[i].pos;
	}
	
	return Vector3Zero;
}
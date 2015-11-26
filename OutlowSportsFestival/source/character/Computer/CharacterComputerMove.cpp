#include "CharacterComputerMove.h"
#include "../CharacterManager.h"
#include "../CharacterFunction.h"
#include "../../Collision/Collision.h"

CharacterComputerMove::CharacterComputerMove(CharacterBase* cb) :
m_cCharacter(cb)
{
	GetParams(m_cParam, cb->m_PlayerInfo.strong_type);
	movemode = Stop;
	m_MoveTargetPos = GetMoveTargetPos(cb);
	m_Count = rand() % 20;

}
CharacterComputerMove::~CharacterComputerMove()
{

}
//Vector2 CharacterComputerMove::Move(CharacterBase* cb)
//{
//	SwitchAction(cb);
//	
//	return Vector2Normalize(Vector2(v.x, v.z));
//
//}
void CharacterComputerMove::GetParams(Param& out, StrongType::Value st)
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
Vector2 CharacterComputerMove::StateMoveFront(CharacterBase* cb)
{
	++m_Count;
	const float GoalOKlen = 2.0f; //�S�[���Ƃ݂Ȃ�����(�덷�̑Ώ�)

	//�ڕW�ɓ��B���Ă�����Ƃ܂�
	if (Vector3Distance(m_MoveTargetPos, m_cCharacter->m_Params.pos) < GoalOKlen)
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
	m_MoveTargetPos = GetMoveTargetPos(cb);

	Vector3 v = m_MoveTargetPos - cb->m_Params.pos;

	if (v.Length() < 3.0f)
	{
		v = Vector3Zero;
		movemode = Stop;
		m_Count = 0;
	}
	cb->CalcHitCheckPos();
	return Vector2Normalize(Vector2(v.x, v.z));
}
Vector2 CharacterComputerMove::StateMoveDistance(CharacterBase* cb)
{
	++m_Count;
	const float Bestlen = 12.0f + rand() % 10; //���̃L�����̃x�X�g����(���͌Œ�)


	//�ڕW�ɓ��B���Ă�����Ƃ܂�
	if (Vector3Distance(m_MoveTargetPos, m_cCharacter->m_Params.pos) > Bestlen)
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
	m_MoveTargetPos = GetMoveTargetPos(cb);

	Vector3 v = cb->m_Params.pos - m_MoveTargetPos;

	/*if (v.Length() < 1.0f)
	{
	v = Vector3Zero;
	movemode = Stop;
	m_Count = 0;
	}*/
	return Vector2Normalize(Vector2(v.x, v.z));
}
Vector2 CharacterComputerMove::StateMoveCenter(CharacterBase* cb)
{
	++m_Count;

	//�ڕW�ɓ��B���Ă�����Ƃ܂�
	if (Vector3Distance(Vector3(0, 0, 0), m_cCharacter->m_Params.pos) < 1.0f)
	{
		m_Count = (int)(m_cParam.RunStop * 100.0f);
		movemode = Stop;
		m_Count = 0;
	}


	//�����ړ��������߂�
	if (m_Count > 50)
	{
		movemode = Stop;
		m_Count = 0;
	}


	//�ڕW�Ɍ������Ĉړ�
	m_MoveTargetPos = Vector3(0,0,0);

	Vector3 v = m_MoveTargetPos - cb->m_Params.pos;

	/*if (v.Length() < 1.0f)
	{
		v = Vector3Zero;
		movemode = Stop;
		m_Count = 0;
	}*/
	return Vector2Normalize(Vector2(v.x, v.z));


}
Vector2 CharacterComputerMove::StateStop(CharacterBase* cb)
{
	const int NextMove = rand() % 5;
	++m_Count;
	m_MoveTargetPos = GetMoveTargetPos(cb);

	if (m_Count > NextMove)
	{
		if (Vector3Distance(Vector3(0,0,0), m_cCharacter->m_Params.pos) > 30.0f)
		{
			movemode = MoveCenter;
		}
		else if (Vector3Distance(m_MoveTargetPos, m_cCharacter->m_Params.pos) > 20.0f)
		{
			movemode = Forward;
		}
		else
		{
			movemode = Distance;
		}
	}

	return Vector2Normalize(Vector2(0, 0));
}
Vector2 CharacterComputerMove::SwitchAction(CharacterBase* cb)
{

	Vector2 xz;
	switch (movemode)
	{
	case Stop:
		xz = StateStop(cb);
		break;
	case Forward:
		xz = StateMoveFront(cb);
		break;
	case Distance:
		xz = StateMoveDistance(cb);
		break;
	case MoveCenter:
		xz = StateMoveCenter(cb);
		break;

	}
	return xz;
}


Vector3 CharacterComputerMove::GetMoveTargetPos(CharacterBase* cb)
{
	Vector3 ret;
	Vector3 nearTarget = Vector3Zero;
	float MostTaugh = 0;

	CharacterBase* pTarget = nullptr;



	struct TargetInfo
	{
		bool      ok;
		Vector3   pos;
	};

	TargetInfo targets[8];

	const CharacterManager::CharacterMap& ChrMap = DefCharacterMgr.GetCharacterMap();

	for (auto it = ChrMap.begin(); it != ChrMap.end(); ++it)
	{

		//�@����ł�or�����Ȃ�continue
		if (chr_func::isDie(it->first) ||
			it->first->m_PlayerInfo.number == cb->m_PlayerInfo.number)
		{
			continue;
		}
		//�ł��̗͂̍����G���^�[�Q�b�g��
		if (it->first->m_Params.HP > MostTaugh)
		{
			pTarget = it->first;
			MostTaugh = it->first->m_Params.HP;
		}
	}
	//�B�G�̍ő�HP��0(MostTaugh�̕ω��Ȃ�)�Ȃ�0�x�N�Ԃ�
	if (MostTaugh == 0)
	{
		return Vector3Zero;
	}


	return pTarget->m_Params.pos;
}

//******************************
//�@�싅
//******************************

Vector2 CharacterComputerMove::SwitcAction_Baseball(CharacterBase* cb, bool flg)
{

	Vector2 xz;
	switch (movemode)
	{
	case Stop:
		xz = StateStop_Baseball(cb, flg);
		break;
	case Forward:
		xz = StateMoveFront(cb);
		break;
	case Distance:
		xz = StateMoveDistance(cb);
		break;
	case Distance_B:
		xz = StateMoveDistance_Baseball(cb);
		break;
	case Keep:
		xz = StateKeepLen(cb);
		break;
	}
	return xz;
}
Vector2 CharacterComputerMove::StateStop_Baseball(CharacterBase* cb, bool flg)
{
	const int NextMove = rand() % 4;
	++m_Count;
	m_MoveTargetPos = GetMoveTargetPos(cb);

	if (m_Count > NextMove)
	{
		if (Vector3Distance(m_MoveTargetPos, m_cCharacter->m_Params.pos) > 20.0f)
		{
			movemode = Forward;
		}
		else
		{
			//�@������
			if (flg && cb->m_PlayerInfo.chr_type == CharacterType::_Baseball)
			{
				movemode = Distance_B;
				//movemode = Keep;
			}
			//�@������Ɖ���
			else
			{
				movemode = Distance_B;
				//movemode = Keep;
			}
		}
	}

	return Vector2Normalize(Vector2(0, 0));
}

Vector2 CharacterComputerMove::StateMoveDistance_Baseball(CharacterBase* cb)
{

	++m_Count;
	const float Bestlen =35.0f + rand() % 5; //���̃L�����̃x�X�g����(���͌Œ�)

	//�ڕW�ɓ��B���Ă�����Ƃ܂�
	if (Vector3Distance(m_MoveTargetPos, m_cCharacter->m_Params.pos) > Bestlen)
	{
		m_Count = (int)(m_cParam.RunStop * 100.0f);
		movemode = Forward;
		m_Count = 0;
	}

	//�@�ړ����ǂɓ���������
	if (chr_func::CheckWall(cb))
	{
		chr_func::AngleControll(cb,m_MoveTargetPos*0.5f,2.0f);
	}

	//�ڕW�ɓ��B�ł��Ȃ� or �V�ڕW������΂����ɕύX����
	if (m_Count > 160)
	{
		movemode = Stop;
		m_Count = 0;
	}


	//�ڕW�Ɍ������Ĉړ�
	m_MoveTargetPos = GetMoveTargetPos(cb);

	Vector3 v = cb->m_Params.pos - m_MoveTargetPos;


	return Vector2Normalize(Vector2(v.x, v.z));
}

//�@�^�[�Q�b�g�Ƃ̋�����ۂ�
Vector2 CharacterComputerMove::StateKeepLen(CharacterBase* cb)
{
	++m_Count;
	const float Bestlen = 35.0f + rand() % 5; //���̃L�����̃x�X�g����(���͌Œ�)
	//�ڕW�ݒ�
	m_MoveTargetPos = GetMoveTargetPos(cb);
	Vector3 v = m_MoveTargetPos - cb->m_Params.pos;
	float len = v.Length();

	if (len < Bestlen)
	{
		v *= -1;
	}
	else
	{
		m_Count = (int)(m_cParam.RunStop * 100.0f);
		movemode = Forward;
		m_Count = 0;
	}

	//�@�ړ����ǂɓ���������
	if (chr_func::CheckWall(cb))
	{
		cb->m_Params.pos = Vector3(0, 0, 0);
	}
	//��莞�Ԍ�
	if (m_Count > 180)
	{
		movemode = Stop;
		m_Count = 0;
	}

	return Vector2Normalize(Vector2(v.x, v.z));
}
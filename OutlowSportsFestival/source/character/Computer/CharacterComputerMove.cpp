#include "CharacterComputerMove.h"
#include "../CharacterManager.h"
#include "../CharacterFunction.h"


CharacterComputerMove::CharacterComputerMove(CharacterBase* cb) :
m_cCharacter(cb)
{
	GetParams(m_cParam, cb->m_PlayerInfo.strong_type);
	movemode = Stop;
	m_MoveTargetPos = GetMoveTargetPos();
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
		//弱い
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

		//普通
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

		//強い
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
		MyAssert(false, "認識できないstrongTypeがComMoveControllClass::GetParamsに送られました");
		break;
	}
}
Vector2 CharacterComputerMove::StateMoveFront(CharacterBase* cb)
{
	++m_Count;
	const float GoalOKlen = 2.0f; //ゴールとみなす距離(誤差の対処)

	//目標に到達していたらとまる
	if (Vector3Distance(m_MoveTargetPos, m_cCharacter->m_Params.pos) < GoalOKlen)
	{
		m_Count = (int)(m_cParam.RunStop * 100.0f);
		movemode = Stop;
		m_Count = 0;
	}


	//目標に到達できない or 新目標があればそこに変更する
	if (m_Count > 200)
	{
		movemode = Stop;
		m_Count = 0;
	}


	//目標に向かって移動
	m_MoveTargetPos = GetMoveTargetPos();

	Vector3 v = m_MoveTargetPos - cb->m_Params.pos;

	if (v.Length() < 3.0f)
	{
		v = Vector3Zero;
		movemode = Stop;
		m_Count = 0;
	}
	return Vector2Normalize(Vector2(v.x, v.z));
}
Vector2 CharacterComputerMove::StateMoveDistance(CharacterBase* cb)
{
	++m_Count;
	const float Bestlen = 12.0f + rand()%10; //そのキャラのベスト距離(今は固定)

	//目標に到達していたらとまる
	if (Vector3Distance(m_MoveTargetPos, m_cCharacter->m_Params.pos) > Bestlen)
	{
		m_Count = (int)(m_cParam.RunStop * 100.0f);
		movemode = Forward;
		m_Count = 0;
	}


	//目標に到達できない or 新目標があればそこに変更する
	if (m_Count > 200)
	{
		movemode = Stop;
		m_Count = 0;
	}


	//目標に向かって移動
	m_MoveTargetPos = GetMoveTargetPos();

	Vector3 v = cb->m_Params.pos- m_MoveTargetPos;

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
	m_MoveTargetPos = GetMoveTargetPos();

	if (m_Count > NextMove)
	{
		if (Vector3Distance(m_MoveTargetPos, m_cCharacter->m_Params.pos) > 20.0f)
		{
			movemode = Forward;
		}
		else
		{
			movemode = Distance;
		}
	}

	return Vector2Normalize(Vector2(0,0));
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
	}
	return xz;
}

Vector3 CharacterComputerMove::GetMoveTargetPos()
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

		//　死んでるor自分ならcontinue
		if (chr_func::isDie(it->first) ||
			it->first->m_PlayerInfo.number == m_cCharacter->m_PlayerInfo.number)
		{
			continue;
		}
		//最も体力の高い敵をターゲットに
		if (it->first->m_Params.HP > MostTaugh)
		{
			pTarget = it->first;
			MostTaugh = it->first->m_Params.HP;
		}
	}
	//達敵の最大HPが0(MostTaughの変化なし)なら0ベク返す
	if (MostTaugh == 0)
	{
		return Vector3Zero;
	}

	//　一番近いキャラが一定距離以下かつ元のターゲットより近いなら近いやつをターゲットに
	nearTarget = NearCheak();
	Vector3 v = nearTarget - m_cCharacter->m_Params.pos;
	Vector3 v2 = pTarget->m_Params.pos - m_cCharacter->m_Params.pos;

	if (v.Length() < 4.5f && v.Length() <= v2.Length())
	{
		return nearTarget;
	}

	return pTarget->m_Params.pos;
}

//　近いやつ
Vector3 CharacterComputerMove::NearCheak()
{
	Vector3 v1, v2;

	const float HomingAngle = PI / 4;
	float MostNear = 1000;
	float TempLen;
	Vector3 pTarget = Vector3Zero;

	//　map代入
	const CharacterManager::CharacterMap& chr_map =
		DefCharacterMgr.GetCharacterMap();


	for (auto it = chr_map.begin(); it != chr_map.end(); ++it)
	{

		//　死んでるor自分ならcontinue
		if (chr_func::isDie(it->first) ||
			it->first->m_PlayerInfo.number == m_cCharacter->m_PlayerInfo.number)
		{
			continue;
		}		

		v2 = it->first->m_Params.pos - m_cCharacter->m_Params.pos;
		v2.y = 0;

		TempLen = v2.Length();

		if (MostNear > TempLen)
		{
			MostNear = TempLen;
			pTarget = it->first->m_Params.pos;
		}
	}

	return pTarget;
}
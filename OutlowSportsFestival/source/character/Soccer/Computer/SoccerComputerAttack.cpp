#include "SoccerComputerAttack.h"

#include "../../CharacterFunction.h"
#include "../../CharacterManager.h"
#include "../SoccerPlayerState.h"

//------------コンピュータ操作の攻撃操作クラス--------------

SoccerState_ComputerControll_Attack::ComputerControllEvent::ComputerControllEvent(SoccerPlayer*const pSoccer, SoccerAttackClass* Attack) :
	m_pSoccer(pSoccer),m_Attack(Attack)
{

}
bool SoccerState_ComputerControll_Attack::ComputerControllEvent::isDoCombo()
{
	int FoolPoint = rand() % 100;
	switch (m_pSoccer->m_PlayerInfo.strong_type)
	{
	case StrongType::_Strong:
		if (m_Attack->getDoHit())
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
	case StrongType::_Usual:
		if (m_Attack->getDoHit())
		{
			if (FoolPoint > 75)
			{
				return false;
			}
			else
			{
				return true;
			}
			
		}
		else
		{
			if (FoolPoint > 75)
			{
				return true;
			}
			else
			{
				return false;
			}
			
		}
		break;
	case StrongType::_Weak:
		if (m_Attack->getDoHit())
		{

			if (FoolPoint > 50)
			{
				return false;
			}
			else
			{
				return true;
			}

		}
		else
		{
			if (FoolPoint > 50)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		break;
	default:
		return false;
		break;

	}
	return false;
}
void SoccerState_ComputerControll_Attack::ComputerControllEvent::AngleControll(RADIAN angle)
{
	const CharacterBase* const pTargetCharacter = GetFrontTargetEnemy();

	
	//自動回転
	if (pTargetCharacter != nullptr)
	{
		chr_func::AngleControll(m_pSoccer, pTargetCharacter->m_Params.pos, 0.7f);
	}
}
const CharacterBase* SoccerState_ComputerControll_Attack::ComputerControllEvent::GetFrontTargetEnemy()
{
	CharacterManager::CharacterMap ChrMap = DefCharacterMgr.GetCharacterMap();

	const float  AutoDistance = 10.0f;               //自動ができる最大距離
	const RADIAN AutoMaxAngle = D3DXToRadian(90);   //自動ができる最大角度

	const CharacterBase* pTargetEnemy = nullptr;    //ターゲット保持のポインタ

	RADIAN MostMinAngle = PI;                       //もっとも狭い角度
	RADIAN TempAngle;

	Vector3 MyFront;      //自身の前方ベクトル
	chr_func::GetFront(m_pSoccer, &MyFront);

	auto it = ChrMap.begin();

	while (it != ChrMap.end())
	{
		//自身を除外
		if (m_pSoccer->m_PlayerInfo.number == it->first->m_PlayerInfo.number ||
			chr_func::isDie(it->first)
			)
		{
			++it;
			continue;
		}

		//距離が一定以上のキャラクタを除外する
		if (Vector3Distance(it->first->m_Params.pos, m_pSoccer->m_Params.pos) > AutoDistance)
		{
			it = ChrMap.erase(it);
			continue;
		}

		//前ベクトルと敵へのベクトルの角度を計算する
		TempAngle = Vector3Radian(MyFront, (it->first->m_Params.pos - m_pSoccer->m_Params.pos));

		//角度が一番狭かったら更新
		if (TempAngle <= MostMinAngle)
		{
			pTargetEnemy = it->first;
			MostMinAngle = TempAngle;
		}

		++it;
	}

	
	return pTargetEnemy;

}
SoccerState_ComputerControll_Attack::SoccerState_ComputerControll_Attack(SoccerPlayer* s) :
	m_Attack(s, new ComputerControllEvent(s,&m_Attack))
{

}
SoccerState_ComputerControll_Attack::~SoccerState_ComputerControll_Attack()
{

}
void SoccerState_ComputerControll_Attack::Enter(SoccerPlayer* s)
{
	//攻撃クラス作成
	SoccerAttackInfo_UsualAtk* pAtk;

	SoccerAttackInfo_UsualAtk::Param AtkParam[] =
	{
		{ 6, 2.0f, 2.5f, DamageBase::Type::_WeekDamage, 3, 20, 0.06f, 3, 9, SoccerPlayer::_ms_Atk1, 50, 10, 20, 35, 0, 5, D3DXToRadian(5), 21,SoccerPlayer::skillparams.Attack1 },
		{ 6, 2.0f, 2.5f, DamageBase::Type::_WeekDamage, 5, 18, 0.04f, 1, 6, SoccerPlayer::_ms_Atk2, 35, 5, 15, 20, 0, 5, D3DXToRadian(5), 3,SoccerPlayer::skillparams.Attack2 },
		{ 8, 2.0f, 2.5f, DamageBase::Type::_VanishDamage, 8, 16, 0.06f, 1, 7, SoccerPlayer::_ms_Atk3, 40, -1, -1, -1, 0, 8, D3DXToRadian(5), 21 ,SoccerPlayer::skillparams.Attack3 },
	};

	for (int i = 0; i < (int)ARRAYSIZE(AtkParam); ++i)
	{
		pAtk = new SoccerAttackInfo_UsualAtk(s);

		pAtk->m_Param = AtkParam[i];

		m_Attack.m_AttackInfoArray.push_back(pAtk);
	}
}
void SoccerState_ComputerControll_Attack::Execute(SoccerPlayer* s)
{
	m_Attack.Update();

	if (m_Attack.isEnd())
	{
		s->SetState(SoccerState_PlayerControll_Move::GetPlayerControllMove(s));
	}
}
void SoccerState_ComputerControll_Attack::Exit(SoccerPlayer* s)
{

}
#include "SoccerComputerAttack.h"
#include "../../CharacterFunction.h"
#include "../../CharacterManager.h"
#include "../SoccerPlayerState.h"
#include "SoccerComputerSliding.h"
#include "../../../Sound/Sound.h"
SoccerState_ComputerControll_Sliding::PlayerControllEvent::PlayerControllEvent(SoccerPlayer*const pSoccer) :
	m_pSoccer(pSoccer)
{

}
bool SoccerState_ComputerControll_Sliding::PlayerControllEvent::isDoCombo()
{
	return controller::GetTRG(controller::button::shikaku, m_pSoccer->m_PlayerInfo.number);
}
void SoccerState_ComputerControll_Sliding::PlayerControllEvent::AngleControll(RADIAN angle)
{
	const CharacterBase* const pTargetCharacter = GetFrontTargetEnemy();

	if (pTargetCharacter != nullptr)
	{
		//自動回転
		chr_func::AngleControll(m_pSoccer, pTargetCharacter->m_Params.pos, angle);
	}
	else
	{
		const Vector2 Stick = controller::GetStickValue(controller::stick::left, m_pSoccer->m_PlayerInfo.number);

		//スティックが一定以上倒されているかどうか
		if (Vector2Length(Stick) > 0.25f)
		{
			Vector3 Vec(Stick.x, 0, Stick.y);

			//スティック値をカメラ空間に
			Vec = Vector3MulMatrix3x3(Vec, matView);

			//キャラクタ回転
			chr_func::AngleControll(m_pSoccer, m_pSoccer->m_Params.pos + Vec, angle);
		}
	}
}

const CharacterBase* SoccerState_ComputerControll_Sliding::PlayerControllEvent::GetFrontTargetEnemy()
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
		if (TempAngle < MostMinAngle)
		{
			pTargetEnemy = it->first;
			MostMinAngle = TempAngle;
		}

		++it;
	}

	return pTargetEnemy;

}


SoccerState_ComputerControll_Sliding::SoccerState_ComputerControll_Sliding(SoccerPlayer* s) :
	m_Attack(s, new PlayerControllEvent(s), 10)
{

}


SoccerState_ComputerControll_Sliding::~SoccerState_ComputerControll_Sliding()
{

}
void SoccerState_ComputerControll_Sliding::Enter(SoccerPlayer* s)
{
	//攻撃クラス作成
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
void SoccerState_ComputerControll_Sliding::Execute(SoccerPlayer* s)
{
	m_Attack.Update();

	if (m_Attack.isEnd())
	{
		s->SetState(SoccerState_PlayerControll_Move::GetPlayerControllMove(s));
	}
}
void SoccerState_ComputerControll_Sliding::Exit(SoccerPlayer* s)
{

}
#include "SoccerHitEvent.h"
#include "SoccerCommonState.h"
//#include "TennisPlayerState_Vanish.h"

SoccerHitEvent::SoccerHitEvent(SoccerPlayer* ps) :
m_pSoccer(ps)
{

}

bool SoccerHitEvent::Hit(DamageBase* pDmg)
{

	//自分の作っているダメージだった場合は何もしない
	if (pDmg->pParent->m_PlayerInfo.number == m_pSoccer->m_PlayerInfo.number)
	{
		return false;
	}

	//当たった時にそのダメージの種類から、それぞれのステートに派生させる
	switch (pDmg->type)
	{
	case DamageBase::Type::_WeekDamage:
		//弱攻撃
		m_pSoccer->SetState(new SoccerState_SmallDamage(m_pSoccer, pDmg->vec));
		return true;

		//未作成
	case DamageBase::Type::_VanishDamage:
		//吹き飛びダメージ
		m_pSoccer->SetState(new SoccerState_DamageVanish(m_pSoccer, pDmg->vec));
		return true;
		/*
		case DamageBase::Type::_UpDamage:
		//上に吹き飛び
		m_pTennis->SetState(new TennisState_(m_pTennis));
		return true;
		*/

	default:break;
	}

	return false;
}
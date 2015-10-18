#include "SoccerHitEvent.h"
#include "SoccerCommonState.h"
#include "../CharacterFunction.h"

SoccerHitEvent::SoccerHitEvent(SoccerPlayer* ps) :
m_pSoccer(ps)
{

}

bool SoccerHitEvent::Hit(DamageBase* pDmg)
{

	//自分の作っているダメージだった場合は何もしない
	if (pDmg->pParent != nullptr &&
		pDmg->pParent->m_PlayerInfo.number == m_pSoccer->m_PlayerInfo.number)
	{
		return false;
	}

    //ダメージ計算
    chr_func::CalcDamage(m_pSoccer, pDmg->Value);

	//もし体力がなかったら、どんな攻撃であろうと死亡ステートへ
	if (chr_func::isDie(m_pSoccer))
	{
		m_pSoccer->SetState(new SoccerState_DamageMotion_Die(m_pSoccer, pDmg->vec));
		return true;
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

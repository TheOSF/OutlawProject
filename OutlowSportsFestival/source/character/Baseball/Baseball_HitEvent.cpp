#include "Baseball_HitEvent.h"
#include "BaseballplayerState_DamageMotionWeak.h"
#include "BaseballPlayerState_DamageMotion_Vanish.h"
#include "BaseballPlayerState_DamageMotion_Die.h"
#include "../CharacterFunction.h"


BaseballHitEvent::BaseballHitEvent(BaseballPlayer* pt) :
m_pBaseball(pt)
{

}

bool BaseballHitEvent::Hit(DamageBase* pDmg)
{

	//自分の作っているダメージだった場合は何もしない
	if (pDmg->pParent != nullptr &&
		pDmg->pParent->m_PlayerInfo.number == m_pBaseball->m_PlayerInfo.number)
	{
		return false;
	}

    //ダメージ計算
    chr_func::CalcDamage(m_pBaseball, pDmg->Value);

	//もし体力がなかったら、どんな攻撃であろうと死亡ステートへ
	if (chr_func::isDie(m_pBaseball))
	{
		m_pBaseball->SetState(new BaseballState_DamageMotion_Die(m_pBaseball, pDmg->vec), true);
		return true;
	}

	//当たった時にそのダメージの種類から、それぞれのステートに派生させる
	switch (pDmg->type)
	{
	case DamageBase::Type::_WeekDamage:
		//弱攻撃
		m_pBaseball->SetState(new BaseballState_DamageMotion_Weak(m_pBaseball, pDmg->vec), true);
		return true;

		//吹っ飛び
		case DamageBase::Type::_VanishDamage:
			m_pBaseball->SetState(new BaseballState_DamageVanish(m_pBaseball, pDmg->vec), true);
		return true;
		case DamageBase::Type::_UpDamage:
		////上に吹き飛び
		//m_pTennis->SetState(new TennisState_(m_pTennis));
		//return true;
		

	default:break;
	}

	return false;
}


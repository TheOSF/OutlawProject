#include "Tennis_HitEvent.h"
#include "TennisPlayerState_DamageMotionWeak.h"
#include "TennisPlayerState_Vanish.h"
#include "TennisPlayerState_DamageMotionDie.h"
#include "../CharacterFunction.h"



TennisHitEvent::TennisHitEvent(TennisPlayer* pt) :
m_pTennis(pt)
{

}

bool TennisHitEvent::Hit(DamageBase* pDmg)
{

	//自分の作っているダメージだった場合は何もしない
    if (pDmg->pParent != nullptr &&
        pDmg->pParent->m_PlayerInfo.number == m_pTennis->m_PlayerInfo.number)
	{
		return false;
	}

    //ダメージ計算
    chr_func::CalcDamage(m_pTennis, pDmg->Value);


    //もし体力がなかったら、どんな攻撃であろうと死亡ステートへ
    if (chr_func::isDie(m_pTennis))
    {
        m_pTennis->SetState(new TennisState_DamageMotion_Die(m_pTennis, pDmg->vec), true);
        return true;
    }


	//当たった時にそのダメージの種類から、それぞれのステートに派生させる
	switch (pDmg->type)
	{
	case DamageBase::Type::_WeekDamage:
		//弱攻撃
        m_pTennis->SetState(new TennisState_DamageMotion_Weak(m_pTennis, pDmg->vec), true);
		return true;
	
	//未作成
	case DamageBase::Type::_VanishDamage:
		//吹き飛びダメージ
        m_pTennis->SetState(new TennisState_DamageVanish(m_pTennis, pDmg->vec), true);
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

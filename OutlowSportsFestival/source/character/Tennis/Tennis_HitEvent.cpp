#include "Tennis_HitEvent.h"
#include "TennisPlayerState_DamageMotionWeak.h"
#include "TennisPlayerState_Vanish.h"
#include "TennisPlayerState_DamageMotionDie.h"
#include "../CharacterFunction.h"



#include "../CharacterHitEventFunc.h"  //ひるみ分岐関数を使用するため


TennisHitEvent::TennisHitEvent(TennisPlayer* pt, bool CounterHit) :
m_pTennis(pt),
m_CounterHit(CounterHit)
{

}

bool TennisHitEvent::Hit(DamageBase* pDmg)
{
    Vector3 DamageVec = Vector3Zero;

    //ひるみ分岐
    switch (CharacterHitEventFunc::CheckDamage(pDmg, m_pTennis, &DamageVec))
    {
    case CharacterHitEventFunc::SetType::Die:
        //死亡ステートへ
        m_pTennis->SetState(new TennisState_DamageMotion_Die(m_pTennis, DamageVec), 2);
        break;


    case CharacterHitEventFunc::SetType::Weak_Hit:
        //弱ひるみステートへ
        m_pTennis->SetState(new TennisState_DamageMotion_Weak(m_pTennis, DamageVec, m_CounterHit, pDmg->HitMotionFrame), 1);
        break;


    case CharacterHitEventFunc::SetType::Vanish_Hit:
        //吹き飛びステートへ
        m_pTennis->SetState(new TennisState_DamageVanish(m_pTennis, DamageVec), 1);
        break;



    case CharacterHitEventFunc::SetType::_None:
        //何もしない(自身のダメージだった場合
        return false;

    default:
        MyAssert(false,"ひるみ分岐ができていません");
        return false;
    }

    
	return true;
}

#include "SoccerHitEvent.h"
#include "SoccerCommonState.h"
#include "../CharacterFunction.h"

#include "../CharacterHitEventFunc.h"  //ひるみ分岐関数を使用するため

#include "SoccerState_ControllVanish.h"


SoccerHitEvent::SoccerHitEvent(SoccerPlayer* ps, bool CounterHit) :
m_pSoccer(ps),
m_CounterHit(CounterHit)
{

}

bool SoccerHitEvent::Hit(DamageBase* pDmg)
{
    Vector3 DamageVec = Vector3Zero;

    //ひるみ分岐
    switch (CharacterHitEventFunc::CheckDamage(pDmg, m_pSoccer, &DamageVec))
    {
    case CharacterHitEventFunc::SetType::Die:
        //死亡ステートへ
        m_pSoccer->SetState(new SoccerState_DamageMotion_Die(m_pSoccer, DamageVec), 2);
        break;


    case CharacterHitEventFunc::SetType::Weak_Hit:
        //弱ひるみステートへ
        m_pSoccer->SetState(new SoccerState_SmallDamage(m_pSoccer, DamageVec, m_CounterHit, pDmg->HitMotionFrame), 1);
        break;


    case CharacterHitEventFunc::SetType::Vanish_Hit:
        //吹き飛びステートへ
        m_pSoccer->SetState(new SoccerState_DamageVanish(m_pSoccer, DamageVec), 1);
        break;

    case CharacterHitEventFunc::SetType::Controll_Hit:   
    {
        //コントロール吹き飛びステートへ
        SoccerState_ControllVanish* p=new SoccerState_ControllVanish(m_pSoccer);
        m_pSoccer->SetState(p, 2);
        ((DamageControllVanish*)pDmg)->GetDamageControll_Transform()->AddControllClass(p->GetControllClass());
    }
        break;

    case CharacterHitEventFunc::SetType::_None:
        //何もしない(自身のダメージだった場合
        return false;

    default:
        MyAssert(false, "ひるみ分岐ができていません");
        return false;
    }


    return true;
}

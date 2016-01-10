#include "Baseball_HitEvent.h"
#include "BaseballplayerState_DamageMotionWeak.h"
#include "BaseballPlayerState_DamageMotion_Vanish.h"
#include "BaseballPlayerState_DamageMotion_Die.h"
#include "BaseballPlayerState_Controll_Vanish.h"
#include "../CharacterFunction.h"



#include "../CharacterHitEventFunc.h"  //ひるみ分岐関数を使用するため



BaseballHitEvent::BaseballHitEvent(BaseballPlayer* pt,bool CounterHit) :
m_pBaseball(pt),
m_CounterHit(CounterHit)
{

}

bool BaseballHitEvent::Hit(DamageBase* pDmg)
{
    Vector3 DamageVec = Vector3Zero;

    //ひるみ分岐
    switch (CharacterHitEventFunc::CheckDamage(pDmg, m_pBaseball, &DamageVec))
    {
    case CharacterHitEventFunc::SetType::Die:
        //死亡ステートへ
        m_pBaseball->SetState(new BaseballState_DamageMotion_Die(m_pBaseball, DamageVec), 2);
        break;


    case CharacterHitEventFunc::SetType::Weak_Hit:
        //弱ひるみステートへ
        m_pBaseball->SetState(new BaseballState_DamageMotion_Weak(m_pBaseball, DamageVec, m_CounterHit,pDmg->HitMotionFrame), 1);
        break;


    case CharacterHitEventFunc::SetType::Vanish_Hit:
        //吹き飛びステートへ
        m_pBaseball->SetState(new BaseballState_DamageVanish(m_pBaseball, DamageVec), 1);
        break;

    case CharacterHitEventFunc::SetType::Controll_Hit:
    {
        //コントロール吹き飛びステートへ
        BaseballState_ControllVanish* p = new BaseballState_ControllVanish(m_pBaseball);
        m_pBaseball->SetState(p, 2);
        ((DamageControllVanish*)pDmg)->GetDamageControll_Transform()->AddControllClass(p->GetControllClass());
    }
        break;
        //

    case CharacterHitEventFunc::SetType::_None:
        //何もしない(自身のダメージだった場合
        return false;

    default:



        MyAssert(false, "ひるみ分岐ができていません");
        return false;
    }


    return true;
}


#include "AmefootUsualHitEvent.h"
#include "AmefootPlayer.h"
#include "../CharacterHitEventFunc.h"
#include "AmefootPlayerState.h"

//-----------------------------------------------------------------------------------------
// AmefootUsualHitEvent
// [ アメフト ] ダメージを受けた時に反応させるクラス
//-----------------------------------------------------------------------------------------
AmefootUsualHitEvent::AmefootUsualHitEvent(AmefootPlayer* pAmefoot) :
m_pAmefoot(pAmefoot)
{}
//-----------------------------------------------------------------------------------------
bool AmefootUsualHitEvent::Hit(DamageBase* pDamage)
{

    Vector3 damageVec = Vector3Zero;
    CharacterHitEventFunc::SetType type = CharacterHitEventFunc::CheckDamage(pDamage, m_pAmefoot, &damageVec);
    
    switch ( type )
    {
    case CharacterHitEventFunc::SetType::Weak_Hit:
        m_pAmefoot->SetState(new AmefootPlayerState_ReceiveWeakDamage(damageVec));
        break;

    case CharacterHitEventFunc::SetType::Vanish_Hit:
        m_pAmefoot->SetState(new AmefootPlayerState_ReceiveVanishDamage(damageVec));
        break;

    case CharacterHitEventFunc::SetType::Die:
        m_pAmefoot->SetState(new AmefootPlayerState_Die(m_pAmefoot, damageVec));
        break;

    case CharacterHitEventFunc::SetType::Controll_Hit:   
    {
        //コントロール吹き飛びステートへ
        AmefootState_ControllVanish* p=new AmefootState_ControllVanish(m_pAmefoot);
        if ( m_pAmefoot->SetState(p, AmefootStateMachine::DONT_UPDATE_STATE_LEVEL) ) {
            ((DamageControllVanish*)pDamage)->GetDamageControll_Transform()->AddControllClass(p->GetControllClass());
        }
    }
        break;

    case CharacterHitEventFunc::SetType::_None:
        return false;
    default:
        MyAssert(false, "ひるみ分岐ができていません\n");
        return false;
    }

    return true;
}
//-----------------------------------------------------------------------------------------


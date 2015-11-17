#include "AmefootUsualHitEvent.h"
#include "AmefootPlayer.h"
#include "../CharacterHitEventFunc.h"
#include "StateClass/AmefootPlayerState_ReceiveWeakDamage.h"
#include "AmefootPlayerState.h"

//-----------------------------------------------------------------------------------------
// AmefootUsualHitEvent
// [ アメフト ] ダメージを受けた時に反応させるクラス
//-----------------------------------------------------------------------------------------
AmefootUsualHitEvent::AmefootUsualHitEvent(AmefootPlayer* pAmefoot):
     m_pAmefoot(pAmefoot)
{}
//-----------------------------------------------------------------------------------------
bool AmefootUsualHitEvent::Hit(DamageBase* pDamage)
{
     Vector3 damageVec = Vector3Zero;

     switch ( CharacterHitEventFunc::CheckDamage(pDamage , m_pAmefoot , &damageVec) )
     {
     case CharacterHitEventFunc::SetType::Weak_Hit:
          m_pAmefoot->SetState(new AmefootPlayerState_ReceiveWeakDamage(damageVec));
          break;

     case CharacterHitEventFunc::SetType::Vanish_Hit:
          m_pAmefoot->SetState(new AmefootPlayerState_ReceiveVanishDamage(damageVec));
          break;

     case CharacterHitEventFunc::SetType::Die:
          m_pAmefoot->m_Renderer.SetMotion(7);
          break;

     case CharacterHitEventFunc::SetType::_None:
          return false;
     default:
          MyAssert(false , "ひるみ分岐ができていません\n");
          return false;
     }

     return true;
}
//-----------------------------------------------------------------------------------------


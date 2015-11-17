#pragma once


#include "../../Damage/Damage.h"

class AmefootPlayer;

//-----------------------------------------------------------------------------------------
// AmefootUsualHitEvent
// [ アメフト ] ダメージを受けた時に反応させるクラス
//-----------------------------------------------------------------------------------------
class AmefootUsualHitEvent : public DamageManager::HitEventBase
{
public:
     AmefootUsualHitEvent(AmefootPlayer* pAmefoot);

     bool Hit(DamageBase* pDamage)override;

protected:
     AmefootPlayer* m_pAmefoot;
};
//-----------------------------------------------------------------------------------------

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
     AmefootUsualHitEvent(AmefootPlayer*const pAmefoot);

     bool Hit(DamageBase* pDamage)override;

protected:
     AmefootPlayer* const m_pAmefoot;
};
//-----------------------------------------------------------------------------------------

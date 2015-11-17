#pragma once


#include "../../Damage/Damage.h"

class AmefootPlayer;

//-----------------------------------------------------------------------------------------
// AmefootUsualHitEvent
// [ �A���t�g ] �_���[�W���󂯂����ɔ���������N���X
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

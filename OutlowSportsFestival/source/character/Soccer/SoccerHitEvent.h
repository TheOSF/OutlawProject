#ifndef __SOCCER_HITEVENT_H__
#define __SOCCER_HITEVENT_H__

#include "../../Damage/Damage.h"
#include "SoccerPlayer.h"

//************************************************************
//	サッカーのダメージを受けた時の反応クラス
//************************************************************

class SoccerHitEvent :public DamageManager::HitEventBase
{
public:
	SoccerHitEvent(SoccerPlayer* ps);

	bool Hit(DamageBase* pDmg);//当たった時に呼ばれる関数(戻り値：当たったかどうか)
private:
	SoccerPlayer* m_pSoccer;
};

#endif
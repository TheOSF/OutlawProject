#ifndef __TENNIS_HITEVENT_H__
#define __TENNIS_HITEVENT_H__

#include "../../Damage/Damage.h"
#include "TennisPlayer.h"

//************************************************************
//	テニスのダメージを受けた時の反応クラス
//************************************************************

class TennisHitEvent :public DamageManager::HitEventBase
{
public:
    TennisHitEvent(TennisPlayer* pt, bool CounterHit = false);
	
	bool Hit(DamageBase* pDmg);//当たった時に呼ばれる関数(戻り値：当たったかどうか)

private:
    bool          m_CheckFinished;
    bool          m_CounterHit;
	TennisPlayer* m_pTennis;
};

#endif
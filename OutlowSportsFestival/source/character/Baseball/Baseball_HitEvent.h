#ifndef __BASEBALL_HITEVENT_H__
#define __BASEBALL_HITEVENT_H__

#include "../../Damage/Damage.h"
#include "BaseballPlayer.h"

//************************************************************
//	野球のダメージを受けた時の反応クラス
//************************************************************

class BaseballHitEvent :public DamageManager::HitEventBase
{
public:
    BaseballHitEvent(BaseballPlayer* pt, bool CounterHit = false);

	bool Hit(DamageBase* pDmg);//当たった時に呼ばれる関数(戻り値：当たったかどうか)

private:
	BaseballPlayer* m_pBaseball;
    bool            m_CounterHit;
};

#endif
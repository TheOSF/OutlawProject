#ifndef __TENNIS_HITEVENT_H__
#define __TENNIS_HITEVENT_H__

#include "../../Damage/Damage.h"
#include "TennisPlayer.h"

//************************************************************
//	�e�j�X�̃_���[�W���󂯂����̔����N���X
//************************************************************

class TennisHitEvent :public DamageManager::HitEventBase
{
public:
    TennisHitEvent(TennisPlayer* pt, bool CounterHit = false);
	
	bool Hit(DamageBase* pDmg);//�����������ɌĂ΂��֐�(�߂�l�F�����������ǂ���)

private:
    bool          m_CheckFinished;
    bool          m_CounterHit;
	TennisPlayer* m_pTennis;
};

#endif
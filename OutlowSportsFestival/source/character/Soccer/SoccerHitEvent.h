#ifndef __SOCCER_HITEVENT_H__
#define __SOCCER_HITEVENT_H__

#include "../../Damage/Damage.h"
#include "SoccerPlayer.h"

//************************************************************
//	�T�b�J�[�̃_���[�W���󂯂����̔����N���X
//************************************************************

class SoccerHitEvent :public DamageManager::HitEventBase
{
public:
	SoccerHitEvent(SoccerPlayer* ps);

	bool Hit(DamageBase* pDmg);//�����������ɌĂ΂��֐�(�߂�l�F�����������ǂ���)
private:
	SoccerPlayer* m_pSoccer;
};

#endif
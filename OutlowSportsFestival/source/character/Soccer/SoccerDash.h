#ifndef __CHARACTER_DASH_CLASS_H__
#define __CHARACTER_DASH_CLASS_H__

#include "iextreme.h"
#include "SoccerPlayer.h"
#include "../../Damage/Damage.h"
#include "../../utillity/Locus.h"

#include "../../Effect/SpeedEffect.h"


//****************************************************************
//		サッカーのダッシュクラス
//****************************************************************

class SoccerDash
{
public:
	SoccerDash(
		SoccerPlayer* pParent	//操るキャラクタのポインタ
		);


	~SoccerDash();

	void Update();	//更新
	void UpdateLocusColor();
	void SetStickValue(CrVector2 StickValue);
	typedef DamageManager::HitEventBase HitEventBase;

	SoccerPlayer*	m_pParent;
	Vector2			m_StickValue;
	Locus           m_Locus;
	SpeedEffect     m_SpeedEffect;
	bool			m_isRun;
	bool            m_Init;
	int             m_count;
};
#endif
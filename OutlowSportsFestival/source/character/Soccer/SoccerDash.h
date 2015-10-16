#ifndef __CHARACTER_DASH_CLASS_H__
#define __CHARACTER_DASH_CLASS_H__

#include "iextreme.h"
#include "SoccerPlayer.h"
#include "../../Damage/Damage.h"


//****************************************************************
//		サッカーのダッシュクラス
//****************************************************************
//スライディングクラス
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
	void DoShot();
	void DoSliding();
	void DoCounter();
	void DoAvoid();
	void DoFinisher();
	void DoStop();
private:
	typedef DamageManager::HitEventBase HitEventBase;

	SoccerPlayer*	m_pParent;
	Vector2			m_StickValue;
	Locus           m_Locus;
	bool			m_isRun;
	bool            m_Init;
};
#endif
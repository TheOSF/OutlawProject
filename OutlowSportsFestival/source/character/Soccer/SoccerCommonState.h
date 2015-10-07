#ifndef __SOCCER_COMMON_STATE_H__
#define __SOCCER_COMMON_STATE_H__

#include "SoccerPlayer.h"
#include "../CharacterDamageMotion.h"
#include "../CharacterDamageVanish.h"

class SoccerState_SmallDamage :public SoccerState
{
public:
	SoccerState_SmallDamage(
		SoccerPlayer*  pSoccer,
		const Vector3& Damage_vec  //ダメージを受けた方向
		);
public:
	void Enter(SoccerPlayer* s);
	void Execute(SoccerPlayer* s);
	void Exit(SoccerPlayer* s);
private:
	CharacterDamageMotion*   m_pDamageMotionClass;	//ひるみ更新クラス
	SoccerPlayer*            m_pSoccer;
	Vector3                  m_Damage_vec;
};

class SoccerState_DamageVanish : public SoccerState
{
public:
	SoccerState_DamageVanish(
		SoccerPlayer*  m_pSoccer,
		const Vector3& Damage_vec  //ダメージを受けた方向
		);

	// ステート開始
	void Enter(SoccerPlayer* s)override;

	// ステート実行
	void Execute(SoccerPlayer* s)override;

	// ステート終了
	void Exit(SoccerPlayer* s)override;

private:
	CharacterDamageVanish*      m_pDamageVanishClass;	//ひるみ更新クラス
	SoccerPlayer*               m_pSoccer;
	Vector3                     m_Damage_vec;
};

#endif
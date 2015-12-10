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
		const Vector3& Damage_vec, //ダメージを受けた方向
        bool           Counter,
        int            Frame
		);
public:
	void Enter(SoccerPlayer* s);
	void Execute(SoccerPlayer* s);
	void Exit(SoccerPlayer* s);
private:
	CharacterDamageMotion*   m_pDamageMotionClass;	//ひるみ更新クラス
	SoccerPlayer*            m_pSoccer;
	Vector3                  m_Damage_vec;
    bool                     m_Counter;
    int                      m_Frame;
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
class SoccerState_DamageMotion_Die : public SoccerState
{
public:
	SoccerState_DamageMotion_Die(
		SoccerPlayer*  pSoccer,
		const Vector3& Damage_vec  //ダメージを受けた方向
		);

	// ステート開始
	void Enter(SoccerPlayer* s)override;

	// ステート実行
	void Execute(SoccerPlayer* s)override;

	// ステート終了
	void Exit(SoccerPlayer* s)override;

private:
	CharacterDamageVanish*   m_pDamageVanishClass;	//ひるみ更新クラス
	SoccerPlayer*            m_pSoccer;
	Vector3                  m_Damage_vec;
};

//サッカーのダッシュ後ブレーキクラス
class SoccerState_brake : public SoccerState
{
public:
	SoccerState_brake(
		SoccerPlayer*  pSoccer
		);
	// ステート開始
	void Enter(SoccerPlayer* s)override;

	// ステート実行
	void Execute(SoccerPlayer* s)override;

	// ステート終了
	void Exit(SoccerPlayer* s)override;
private:
	SoccerPlayer*            m_pSoccer;
	CharacterUsualMove*		m_pMoveClass;
};
//サッカーの激突クラス
class SoccerState_clash : public SoccerState
{
public:
	SoccerState_clash(
		SoccerPlayer*  pSoccer
		);
	// ステート開始
	void Enter(SoccerPlayer* s)override;

	// ステート実行
	void Execute(SoccerPlayer* s)override;

	// ステート終了
	void Exit(SoccerPlayer* s)override;
private:
	int m_count;
	SoccerPlayer*            m_pSoccer;
	CharacterUsualMove*		m_pMoveClass;
};

#endif
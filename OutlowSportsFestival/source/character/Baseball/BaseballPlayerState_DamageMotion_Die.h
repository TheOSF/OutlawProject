#ifndef __BASEBALLPLAYER_STATE_DAMAGEMOTION_DIE_H__
#define __BASEBALLPLAYER_STATE_DAMAGEMOTION_DIE_H__

#include "BaseballPlayer.h"
#include "../CharacterDamageVanish.h"

//***************************************************
//		野球キャラクタの死にステート
//***************************************************
class BaseballState_DamageMotion_Die : public BaseballState
{
public:
	BaseballState_DamageMotion_Die(
		BaseballPlayer*  pBaseball,
		const Vector3& Damage_vec  //ダメージを受けた方向
		);

	// ステート開始
	void Enter(BaseballPlayer* b)override;

	// ステート実行
	void Execute(BaseballPlayer* b)override;

	// ステート終了
	void Exit(BaseballPlayer* b)override;

private:
	CharacterDamageVanish*   m_pDamageVanishClass;	//ひるみ更新クラス
	BaseballPlayer*            m_pBaseball;
	Vector3                  m_Damage_vec;
};

#endif
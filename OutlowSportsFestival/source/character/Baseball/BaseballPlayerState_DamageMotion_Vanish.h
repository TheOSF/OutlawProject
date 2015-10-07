#ifndef __BASEBALLPLAYER_STATE_DAMAGEMOTION_VANIS_H__
#define __BASEBALLPLAYER_STATE_DAMAGEMOTION_VANIS_H__

#include "BaseballPlayer.h"
#include "../CharacterDamageVanish.h"

//***************************************************
//		テニスキャラクタの弱ひるみステート
//***************************************************
class BaseballState_DamageVanish : public BaseballState
{
public:
	BaseballState_DamageVanish(
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
	CharacterDamageVanish*      m_pDamageVanishClass;	//ひるみ更新クラス
	BaseballPlayer*             m_pBaseball;
	Vector3                     m_Damage_vec;
};

#endif
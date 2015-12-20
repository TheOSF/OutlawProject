#pragma once
#include "../../Damage/Damage.h"
#include "../CharacterBase.h"

//************************************************************
//	コンピュータのダメージ判定を感知した時の反応クラス
//************************************************************

class CharacterComputerReactionHitEvent :public DamageManager::HitEventBase
{
public:
	enum HitType
	{
		CanCounter,
		Escape
	};

	CharacterComputerReactionHitEvent(CharacterBase* cb);
	bool HitDamage;
	HitType hittype;
	DamageBase* pDmg;
	bool Hit(DamageBase* pDmg);//当たった時に呼ばれる関数(戻り値：当たったかどうか)
private:
	CharacterBase* m_cCharacter;

};
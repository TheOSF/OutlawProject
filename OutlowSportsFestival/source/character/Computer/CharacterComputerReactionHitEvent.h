#pragma once
#include "../../Damage/Damage.h"
#include "../CharacterBase.h"

//************************************************************
//	�R���s���[�^�̃_���[�W��������m�������̔����N���X
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
	bool Hit(DamageBase* pDmg);//�����������ɌĂ΂��֐�(�߂�l�F�����������ǂ���)
private:
	CharacterBase* m_cCharacter;

};
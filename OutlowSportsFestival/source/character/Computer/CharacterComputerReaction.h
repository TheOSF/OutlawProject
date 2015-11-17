#pragma once
#include "../CharacterBase.h"
#include "../CharacterRenderer.h"
#include "../CharacterStateTemplate.h"

#include "CharacterComputerReactionHitEvent.h"
#include "CharacterComputerMove.h"
#include "../../Damage/Damage.h"

//---------------------------------------------------//
/*

要望１
攻撃判定が来た時に、内部クラスである
ActionEventを継承したクラスのReaction関数を呼ぶ。
要望２


*/


class CharacterComputerReaction
{
public:
	//CharacterComputerMove::Param m_params;
	class ActionEvent
	{
	public:
		virtual~ActionEvent() {}
		virtual void Reaction(CharacterComputerReactionHitEvent::HitType hittype,Vector3 vec) = 0;			//攻撃開始時に呼ばれる関数
	};
	CharacterComputerReaction(
		CharacterBase*					pParent,	//操るキャラクタのポインタ
		const CharacterComputerMove::Param&	param,		//移動パラメータ構造体
		ActionEvent*						ActionEvent	//移動イベントに反応するクラス
		);

	~CharacterComputerReaction();
	void Update();
private:
	typedef DamageManager::HitEventBase HitEventBase;

	CharacterBase*	                  m_cCharacter;
	ActionEvent*  	                  m_ActionEvent;
	CharacterComputerMove::Param	  m_Params;
	CharacterComputerReactionHitEvent::HitType hit;
	DamageBase* pDmg;
};

#pragma once
#include "../CharacterBase.h"
#include "../CharacterRenderer.h"
#include "../CharacterStateTemplate.h"

#include "CharacterComputerReactionHitEvent.h"
#include "CharacterComputerMove.h"
#include "../../Damage/Damage.h"

//--------------------------------------------------//
//  CharacterComputerReactionクラスは内部クラスである
//  ReactionEventクラスのAttack関数を
//　コンピュータが攻撃するべき時に呼ぶクラスである
//--------------------------------------------------//


class CharacterComputerReaction
{
public:
	//CharacterComputerMove::Param m_params;
	class ActionEvent
	{
	public:
		virtual~ActionEvent() {}
		virtual void Reaction(CharacterComputerReactionHitEvent::HitType hittype) = 0;			//攻撃開始時に呼ばれる関数
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
};

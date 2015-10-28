#pragma once
#include "../CharacterBase.h"
#include "../CharacterRenderer.h"
#include "../CharacterStateTemplate.h"
#include "CharacterComputerMove.h"
#include "../../Damage/Damage.h"

class CharacterBase;

//--------------------------------------------------//
//  CharacterComputerDoActionクラスは内部クラスである
//  ActionEventクラスのAttack関数を
//　コンピュータが攻撃するべき時に呼ぶクラスである
//--------------------------------------------------//

class CharacterComputerDoAction
{
public:
	//CharacterComputerMove::Param m_params;
	class ActionEvent
	{
	public:
		virtual~ActionEvent() {}
		virtual void Attack(float len) = 0;			//攻撃開始時に呼ばれる関数
	};
	CharacterComputerDoAction(
		CharacterBase*					pParent,	//操るキャラクタのポインタ
		const CharacterComputerMove::Param&	param,		//移動パラメータ構造体
		ActionEvent*						ActionEvent,	//移動イベントに反応するクラス
		DamageManager::HitEventBase*	pHitEventBase//ダメージを受けた時に反応するクラス
		);

	~CharacterComputerDoAction();
	void Update();
	float GetMoveTargetLength();
private:
	typedef DamageManager::HitEventBase HitEventBase;

	CharacterBase*	                  m_cCharacter;
	ActionEvent*  	                  m_ActionEvent;
	HitEventBase*	                  m_pHitEventBase;
	CharacterComputerMove::Param	  m_Params;
};
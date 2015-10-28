#include "CharacterComputerReaction.h"
#include "../CharacterManager.h"
#include "../CharacterFunction.h"
#include"../../Damage/Damage.h"

CharacterComputerReaction::CharacterComputerReaction(
	CharacterBase*					pParent,	//操るキャラクタのポインタ
	const CharacterComputerMove::Param&	param,		//移動パラメータ構造体
	ActionEvent*						pActionEvent,	//移動イベントに反応するクラス
	DamageManager::HitEventBase*	pHitEventBase//ダメージを受けた時に反応するクラス
	) :
	m_cCharacter(pParent), m_Params(param), m_ActionEvent(pActionEvent), m_pHitEventBase(pHitEventBase)
{


}
CharacterComputerReaction::~CharacterComputerReaction()
{

}
void CharacterComputerReaction::Update()
{
	SphereParam sp;
	sp.pos = m_cCharacter->m_Params.pos;
	//反応レーダー展開
	//DefDamageMgr.HitCheckSphere(sp, m_pHitEventBase->Hit());


	//反応する
	m_ActionEvent->Reaction();
		
}
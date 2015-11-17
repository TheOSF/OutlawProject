#include "CharacterComputerReaction.h"
#include "../CharacterManager.h"
#include "../CharacterFunction.h"
#include"../../Damage/Damage.h"

CharacterComputerReaction::CharacterComputerReaction(
	CharacterBase*					pParent,	//操るキャラクタのポインタ
	const CharacterComputerMove::Param&	param,		//移動パラメータ構造体
	ActionEvent*						pActionEvent	//移動イベントに反応するクラス
	) :
	m_cCharacter(pParent), m_Params(param), m_ActionEvent(pActionEvent)
{
}
CharacterComputerReaction::~CharacterComputerReaction()
{
	delete m_ActionEvent;
}
void CharacterComputerReaction::Update()
{
	SphereParam sp;
	sp.pos = m_cCharacter->m_Params.pos;
	sp.size = 10.0f;

	CharacterComputerReactionHitEvent hitevent(m_cCharacter);
	//反応レーダー展開
	DefDamageMgr.HitCheckSphere(sp, hitevent);

	Vector3 outvec(1,1,1);
	Vector3 outpos;

	//反応する
	if (hitevent.HitDamage)
	{
		hit = hitevent.hittype;
		//pDmg->CalcPosVec(m_cCharacter->m_Params.pos, &outpos, &outvec);
		m_ActionEvent->Reaction(hit,outvec);
	}
		
}
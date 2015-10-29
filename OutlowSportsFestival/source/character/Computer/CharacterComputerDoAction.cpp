#include "CharacterComputerDoAction.h"
#include "../CharacterManager.h"
#include "../CharacterFunction.h"
#include"../../Damage/Damage.h"

CharacterComputerDoAction::CharacterComputerDoAction(
	CharacterBase*					pParent,	//操るキャラクタのポインタ
	const CharacterComputerMove::Param&	param,		//移動パラメータ構造体
	ActionEvent*						pActionEvent,	//移動イベントに反応するクラス
	DamageManager::HitEventBase*	pHitEventBase//ダメージを受けた時に反応するクラス
	) :
	m_cCharacter(pParent),m_Params(param), m_ActionEvent(pActionEvent), m_pHitEventBase(pHitEventBase)
{
	

}
CharacterComputerDoAction::~CharacterComputerDoAction()
{

}
void CharacterComputerDoAction::Update()
{
	SphereParam sp;
	sp.pos = m_cCharacter->m_Params.pos;

	//DefDamageMgr.HitCheckSphere(sp, );

	//攻撃する時ならば
	if (rand()%50==0)
	{
		//距離を測り、攻撃する
		m_ActionEvent->Attack(GetMoveTargetLength());
	}
}
float CharacterComputerDoAction::GetMoveTargetLength()
{
	Vector3 ret;
	float MostNear = 10000.0f;

	CharacterBase* pTarget = nullptr;



	struct TargetInfo
	{
		bool      ok;
		Vector3   pos;
	};

	TargetInfo targets[8];

	const CharacterManager::CharacterMap& ChrMap = DefCharacterMgr.GetCharacterMap();

	for (auto it = ChrMap.begin(); it != ChrMap.end(); ++it)
	{

		//　死んでるor自分ならcontinue
		if (chr_func::isDie(it->first) ||
			it->first->m_PlayerInfo.number == m_cCharacter->m_PlayerInfo.number)
		{
			continue;
		}
		float len = Vector3Distance(m_cCharacter->m_Params.pos, it->first->m_Params.pos);
		//最も体力が高い敵をターゲットに
		if (len < MostNear)
		{
			pTarget = it->first;
			MostNear = len;
		}
	}
	//MostNearの変化なしなら0ベク返す
	if (MostNear == 9990.0f)
	{
		return 0;
	}
	return MostNear;
}
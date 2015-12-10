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
	m_cCharacter(pParent), m_Params(param), m_ActionEvent(pActionEvent), m_pHitEventBase(pHitEventBase)
{


}
CharacterComputerDoAction::~CharacterComputerDoAction()
{
	delete m_ActionEvent;
	delete m_pHitEventBase;
}
void CharacterComputerDoAction::Update()
{
	SphereParam sp;
	sp.pos = m_cCharacter->m_Params.pos;



	//攻撃する時ならば
	if (rand() % 10 == 0)
	{
		//距離を測り、攻撃する
		m_ActionEvent->Attack(GetMoveTargetLength());

	}
}

float CharacterComputerDoAction::GetMoveTargetLength()
{
	Vector3 v1, v2;
	const float AngleRange = PI / 4;
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
		//　視野角計算
		chr_func::GetFront(m_cCharacter, &v1);

		v2 = it->first->m_Params.pos - m_cCharacter->m_Params.pos;
		v2.y = 0;

		//角度外なら適していない
		if (Vector3Radian(v1, v2) > AngleRange)
		{
			continue;
		}

		float len = Vector3Distance(m_cCharacter->m_Params.pos, it->first->m_Params.pos);
		//最も距離が近い敵をターゲットに
		if (len < MostNear)
		{
			pTarget = it->first;
			MostNear = len;
		}
	}
	//MostNearの変化なしなら0ベク返す
	if (MostNear >= 9990.0f)
	{
		return 10000;
	}
	return MostNear;
}
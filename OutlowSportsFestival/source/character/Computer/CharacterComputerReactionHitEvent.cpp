#include "CharacterComputerReactionHitEvent.h"
#include "../CharacterFunction.h"
#include "../../Ball/Ball.h"




CharacterComputerReactionHitEvent::CharacterComputerReactionHitEvent(CharacterBase* cb) :
	m_cCharacter(cb),HitDamage(false)
{

}

bool CharacterComputerReactionHitEvent::Hit(DamageBase* pDmg)
{

	//自分の作っているダメージだった場合は何もしない
	if (pDmg->pParent != nullptr &&
		pDmg->pParent->m_PlayerInfo.number == m_cCharacter->m_PlayerInfo.number)
	{
		return false;
	}

	this->pDmg = pDmg;
	HitDamage = true;

    if (pDmg->pBall != nullptr && BallBase::isCanCounter(pDmg->pBall, m_cCharacter))
	{
		hittype = CanCounter;
	}
	else
	{
		hittype = Escape;
	}

	return false;
}

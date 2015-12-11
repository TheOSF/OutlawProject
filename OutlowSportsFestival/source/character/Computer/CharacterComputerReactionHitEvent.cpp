#include "CharacterComputerReactionHitEvent.h"
#include "../CharacterFunction.h"
#include "../../Ball/Ball.h"




CharacterComputerReactionHitEvent::CharacterComputerReactionHitEvent(CharacterBase* cb) :
	m_cCharacter(cb),HitDamage(false)
{

}

bool CharacterComputerReactionHitEvent::Hit(DamageBase* pDmg)
{

	//Ž©•ª‚Ìì‚Á‚Ä‚¢‚éƒ_ƒ[ƒW‚¾‚Á‚½ê‡‚Í‰½‚à‚µ‚È‚¢
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

#include "TennisComputerUtillityClass.h"
#include "TennisComputerState_Attack.h"
#include "../TennisPlayerState_SlowUpBall.h"
#include "../../CharacterFunction.h"
#include "../TennisState_BoundShot.h"
#include "../TennisSpecialAtk.h"

TennisComputerUtillityClass::AttackEvent::AttackEvent(TennisPlayer* pTennis):
m_pTennis(pTennis)
{

}

bool TennisComputerUtillityClass::AttackEvent::Action(CharacterBase* pTarget, float len)
{
   
    if (len < 10.0f)
    {
        if (frand() < 0.5f)
        {
            m_pTennis->SetState(new TennisComputerState_Attack(m_pTennis, false));
        }
        else
        {
            m_pTennis->SetState(new TennisPlayerState_SlowUpBall(new SlowUpBallControllClass(m_pTennis, frand() < 0.3f)));
        }
        
        return true;
    }
    else if (len < 25.0f)
    {
        if (chr_func::isCanSpecialAttack(m_pTennis) && frand() < 0.2f)
        {
            m_pTennis->SetState(new TennisState_SpecialAtk(m_pTennis));
            return true;
        }

        if (m_pTennis->isCanBoundBallAtk() && frand() < 0.5f)
        {
            m_pTennis->SetState(new TennisState_BoundShot());
        }
        else
        {
            m_pTennis->SetState(new TennisPlayerState_SlowUpBall(new SlowUpBallControllClass(m_pTennis, frand() < 0.3f)));
        }
        return true;
    }
    else
    {
        if (chr_func::isCanSpecialAttack(m_pTennis) && frand() < 0.2f)
        {
            m_pTennis->SetState(new TennisState_SpecialAtk(m_pTennis));
            return true;
        }

        m_pTennis->SetState(new TennisPlayerState_SlowUpBall(new SlowUpBallControllClass(m_pTennis, frand() < 0.5f)));
        return true;
    }

    return false;
}

//---------------------------------------------------------//

TennisComputerUtillityClass::SlowUpBallControllClass::SlowUpBallControllClass(TennisPlayer* const pChr, bool Smash) :
m_pChr(pChr),
m_DoSmash(Smash)
{
    m_pCharacterComputerReaction = new CharacterComputerReaction(
        pChr,
        TennisComputerReactionEvent::GetCharacterComputerReactionInParam(),
        new TennisComputerReactionEvent(m_pChr)
        );
}

TennisComputerUtillityClass::SlowUpBallControllClass::~SlowUpBallControllClass()
{
    delete m_pCharacterComputerReaction;
}

Vector3 TennisComputerUtillityClass::SlowUpBallControllClass::GetVec()
{
    CharacterBase* pTarget = nullptr;
    Vector3 ret = Vector3Zero;

    if (chr_func::CalcAtkTarget(m_pChr, PI, 150.0f, &pTarget))
    {
        ret = pTarget->m_Params.pos - m_pChr->m_Params.pos;
    }
    
    return ret;
}

bool TennisComputerUtillityClass::SlowUpBallControllClass::DoOtherAction()
{
    m_pCharacterComputerReaction->Update();

    return m_pCharacterComputerReaction->Reactioned();
}

bool TennisComputerUtillityClass::SlowUpBallControllClass::DoShotAfterAction()
{
    m_pCharacterComputerReaction->Update();
    
    return m_pCharacterComputerReaction->Reactioned();
}

bool TennisComputerUtillityClass::SlowUpBallControllClass::isShot(bool isSmashFrame)
{
    if (m_DoSmash)
    {
        return isSmashFrame == m_DoSmash;
    }
    else
    {
        return true;
    }

    return false;
}
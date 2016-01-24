#include "AmefootComputerUtillityClass.h"
#include "../AmefootPlayerState.h"

#include "../../CharacterFunction.h"

//----------------------------------------------------------------
//      攻撃イベント呼び出しクラス
//----------------------------------------------------------------

AmefootComputerUtillityClass::AttackEvent::AttackEvent(AmefootPlayer* pAmefoot):
m_pAmefoot(pAmefoot)
{

}

bool AmefootComputerUtillityClass::AttackEvent::Action(CharacterBase* pTarget, float len)
{
    if (len < 15.0f)
    {
        if (chr_func::isCanSpecialAttack(m_pAmefoot) && frand() < 0.3f)
        {
            m_pAmefoot->SetState(new AmefootPlayerState_SpecialAtk(m_pAmefoot));
        }
        else
        {
            m_pAmefoot->SetState(new AmefootPlayerState_Tackle(m_pAmefoot));
        }
     
        return true;
    }
    else if (len < 50.0f)
    {
        if (chr_func::isCanSpecialAttack(m_pAmefoot) && frand() < 0.1f)
        {
            m_pAmefoot->SetState(new AmefootPlayerState_SpecialAtk(m_pAmefoot));
            return true;
        }
        
        if (m_pAmefoot->isCanThrowBall())
        {
            m_pAmefoot->SetState(new AmefootPlayerState_ThrowBall());
            return true;
        }
    }

    return false;
}

//----------------------------------------------------------------
//      反応イベント呼び出しクラス
//----------------------------------------------------------------

AmefootComputerUtillityClass::ReactionEvent::ReactionEvent(AmefootPlayer* pAmefoot):
m_pAmefoot(pAmefoot)
{

}

void AmefootComputerUtillityClass::ReactionEvent::Reaction(const TypeParam& param, CrVector3 vec)
{
    if (param.isCanCounter)
    {
        if (frand() < 0.75f)
        {
            m_pAmefoot->SetState(new AmefootPlayerState_Counter());
        }
        else
        {
            m_pAmefoot->SetState(new AmefootPlayerState_Evasion());
        }
        return;
    }

    if (param.isEnemyNear)
    {
        if (frand() < 0.75f)
        {
            m_pAmefoot->SetState(new AmefootPlayerState_Tackle(m_pAmefoot));
        }
        else
        {
            m_pAmefoot->SetState(new AmefootPlayerState_Evasion());
        }
        return;
    }

    if (param.isStrongDamage)
    {
        m_pAmefoot->SetState(new AmefootPlayerState_Evasion());
        return;
    }

}

CharacterComputerReaction::InParam AmefootComputerUtillityClass::ReactionEvent::GetInParam()
{
    CharacterComputerReaction::InParam param;

    param.CanNearAtkAreaSize = 20.0f;
    param.CounterAreaSize = 8.0f;

    return param;
}
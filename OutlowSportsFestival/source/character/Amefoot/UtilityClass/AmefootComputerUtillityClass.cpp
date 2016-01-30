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
    if (chr_func::isCanSpecialAttack(m_pAmefoot) && frand() < 0.1f)
    {
        chr_func::AngleControll(m_pAmefoot, pTarget);
        m_pAmefoot->SetState(new AmefootPlayerState_SpecialAtk(m_pAmefoot));
        return true;
    }

    if (len < 15.0f)
    {
        if (frand() < 0.5f)
        {
            chr_func::AngleControll(m_pAmefoot, pTarget);
            m_pAmefoot->SetState(new AmefootPlayerState_BallTouchDown(m_pAmefoot));
        }
        else
        {
            chr_func::AngleControll(m_pAmefoot, pTarget);
            m_pAmefoot->SetState(new AmefootPlayerState_Tackle(m_pAmefoot));
        }
        return true;
    }
    else if (len < 25.0f)
    {
        if (frand() < 0.5f)
        {
            chr_func::AngleControll(m_pAmefoot, pTarget);
            m_pAmefoot->SetState(new AmefootPlayerState_BallTouchDown(m_pAmefoot));
        }
        else
        {
            m_pAmefoot->SetState(new AmefootPlayerState_ThrowBall());
        }
        return true;
    }
    else if (len < 50.0f)
    {
        
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
            chr_func::AngleControll(m_pAmefoot, m_pAmefoot->m_Params.pos + vec);
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
        if (frand() < 0.5f)
        {
            m_pAmefoot->SetState(new AmefootPlayerState_Evasion());
        }
        else
        {
            chr_func::AngleControll(m_pAmefoot, Vector3Zero);
            m_pAmefoot->SetState(new AmefootPlayerState_BallTouchDown(m_pAmefoot));
        }
        return;
    }

}

CharacterComputerReaction::InParam AmefootComputerUtillityClass::ReactionEvent::GetInParam()
{
    CharacterComputerReaction::InParam param;

    param.CanNearAtkAreaSize = 10.0f;
    param.CounterAreaSize = 8.0f;

    return param;
}
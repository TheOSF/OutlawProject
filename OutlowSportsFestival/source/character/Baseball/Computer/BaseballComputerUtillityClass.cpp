#include "BaseballComputerUtillityClass.h"
#include "../../CharacterFunction.h"
#include "../BaseballState_SPAttack_B.h"
#include "../BaseballPlayerState_Attack_B.h"
#include "../BaseballState_Change.h"
#include "../BaseballState_PlayerControll_ShotAttack_B.h"

#include "../BaseballState_SPAttack_P.h"
#include "../BaseballPlayerState_Attack_P.h"
#include "../BaseballState_PlayerControll_ShotAttack_P.h"

#include "../BaseballPlayerState_Counter.h"
#include "../BaseballState_PlayerControll_Evasion.h"

#include "../BaseballAttackState.h"

#include "../BaseballPlayerState_ChargeBall.h"

//----------------------------------------------------------------
//      攻撃イベント呼び出しクラス
//----------------------------------------------------------------

BaseballComputerUtillityClass::AttackEvent::AttackEvent(BaseballPlayer* pChr) :
m_pChr(pChr)
{

}

bool BaseballComputerUtillityClass::AttackEvent::Action(CharacterBase* pTarget, float len)
{
    if (m_pChr->getBatterFlg())
    {
        return Action_B(pTarget,len);
    }

    return Action_P(pTarget, len);
}

bool BaseballComputerUtillityClass::AttackEvent::Action_B(CharacterBase* pTarget, float len)
{
    if (frand() < 0.1f)
    {
        m_pChr->SetState(new BaseballState_Change());
        return true;
    }

    if (len < 15.0f)
    {
        if (chr_func::isCanSpecialAttack(m_pChr) && len < 10.0f)
        {
            chr_func::AngleControll(m_pChr, pTarget);
            m_pChr->SetState(new BaseballState_SPAttack_B(m_pChr));
        }
        else
        {
            chr_func::AngleControll(m_pChr, pTarget);
            m_pChr->SetState(new BaseballAttackState(m_pChr));
        }

        return true;
    }
    else
    {
        if (frand() < 0.3f)
        {
            m_pChr->SetState(new BaseballState_Change());
        }
        else
        {
            chr_func::AngleControll(m_pChr, pTarget);
            m_pChr->SetState(new BaseballState_PlayerControll_ShotAttack_B(new ShotControllClass_B(m_pChr)));
        }
        return true;
    }
    

    return false;
}

bool BaseballComputerUtillityClass::AttackEvent::Action_P(CharacterBase* pTarget, float len)
{
    if (frand() < 0.1f && chr_func::isCanSpecialAttack(m_pChr) && len < 10.0f)
    {
        chr_func::AngleControll(m_pChr, pTarget);
        m_pChr->SetState(new BaseballState_SPAttack_P());
        return true;
    }

    if (len < 20.0f)
    {
        if (frand() < 0.5f)
        {
            m_pChr->SetState(new BaseballState_Change());
        }
        else
        {
            chr_func::AngleControll(m_pChr, pTarget);
            m_pChr->SetState(new BaseballState_PlayerControll_ShotAttack_P());
        }

        return true;
    }
    else
    {
        if (frand() < 0.2f)
        {
            m_pChr->SetState(new BaseballState_Change());
        }
        else if (frand() < 0.5f)
        {
            chr_func::AngleControll(m_pChr, pTarget);
            m_pChr->SetState(new BaseballPlayerState_ChargeBall());
        }
        else
        {
            chr_func::AngleControll(m_pChr, pTarget);
            m_pChr->SetState(new BaseballState_PlayerControll_ShotAttack_P());
        }
        return true;
    }


    return false;
}

//----------------------------------------------------------------
//      反応イベント呼び出しクラス
//----------------------------------------------------------------

BaseballComputerUtillityClass::ReactionEvent::ReactionEvent(BaseballPlayer* pChr) :
m_pChr(pChr)
{

}

void BaseballComputerUtillityClass::ReactionEvent::Reaction(const TypeParam& param, CrVector3 vec)
{
    if (m_pChr->getBatterFlg())
    {
        Reaction_B(param, vec);
    }
    else
    {
        Reaction_P(param, vec);
    }
}

void BaseballComputerUtillityClass::ReactionEvent::Reaction_B(const TypeParam& param, CrVector3 vec)
{
    if (param.isCanCounter)
    {
        if (frand() < 0.75f)
        {
            m_pChr->SetState(new BaseballState_PlayerControll_Counter());
        }
        else
        {
            Vector3 EscapeVec;

            Vector3Cross(EscapeVec, vec, Vector3AxisY);

            if (frand() < 0.5f)
            {
                EscapeVec = -EscapeVec;
            }

            m_pChr->SetState(new BaseballState_Rolling(EscapeVec));
        }
        return;
    }

    if (param.isEnemyNear)
    {
        if (frand() < 0.5f && chr_func::isCanSpecialAttack(m_pChr))
        {
            chr_func::AngleControll(m_pChr, m_pChr->m_Params.pos + vec);
            m_pChr->SetState(new BaseballState_SPAttack_B(m_pChr));
        }
        else if (frand() < 0.75f)
        {
            chr_func::AngleControll(m_pChr, m_pChr->m_Params.pos + vec);
            m_pChr->SetState(new BaseballAttackState(m_pChr));
        }
        else
        {
            Vector3 EscapeVec;

            if (frand() < 0.5f)
            {
                EscapeVec = -Vector3Normalize(m_pChr->m_Params.pos);
            }
            else
            {
                EscapeVec = -vec;
            }

            m_pChr->SetState(new BaseballState_Rolling(EscapeVec));
        }
        return;
    }
}


void BaseballComputerUtillityClass::ReactionEvent::Reaction_P(const TypeParam& param, CrVector3 vec)
{
    if (param.isCanCounter)
    {
        if (frand() < 0.5f)
        {
            m_pChr->SetState(new BaseballState_PlayerControll_Counter());
        }
        else
        {
            Vector3 EscapeVec;

            Vector3Cross(EscapeVec, vec, Vector3AxisY);

            if (frand() < 0.5f)
            {
                EscapeVec = -EscapeVec;
            }

            m_pChr->SetState(new BaseballState_Rolling(EscapeVec));
        }
        return;
    }

    if (param.isEnemyNear)
    {
        if (frand() < 0.3f)
        {
            chr_func::AngleControll(m_pChr, m_pChr->m_Params.pos + vec);
            m_pChr->SetState(new BaseballState_PlayerControll_ShotAttack_P());
        }
        else if (frand() < 0.3f)
        {
            Vector3 EscapeVec;

            if (frand() < 0.5f)
            {
                EscapeVec = -Vector3Normalize(m_pChr->m_Params.pos);
            }
            else
            {
                EscapeVec = -vec;
            }

            m_pChr->SetState(new BaseballState_Rolling(EscapeVec));
        }
        else
        {
            m_pChr->SetState(new BaseballState_Change());
        }
        return;
    }
}

CharacterComputerReaction::InParam BaseballComputerUtillityClass::ReactionEvent::GetInParam(bool batter)
{
    CharacterComputerReaction::InParam param;

    if (batter)
    {
        param.CanNearAtkAreaSize = 10.0f;
        param.CounterAreaSize = 15.0f;
    }
    else
    {
        param.CanNearAtkAreaSize = 20.0f;
        param.CounterAreaSize = 15.0f;
    }

    return param;
}

//-------------------------------------------------------------
//      バッター遠距離攻撃の操作クラス
//-------------------------------------------------------------

BaseballComputerUtillityClass::ShotControllClass_B::ShotControllClass_B(BaseballPlayer* pBaseball) :
m_pBaseball(pBaseball)
{
    m_pReaction = new CharacterComputerReaction(
        pBaseball, 
        ReactionEvent::GetInParam(m_pBaseball->getBatterFlg()),
        new BaseballComputerUtillityClass::ReactionEvent(pBaseball)
        );
}

BaseballComputerUtillityClass::ShotControllClass_B::~ShotControllClass_B()
{
    delete m_pReaction;
}

Vector3 BaseballComputerUtillityClass::ShotControllClass_B::GetVec()
{
    CharacterBase* pTarget = nullptr;

    if (chr_func::CalcAtkTarget(m_pBaseball, D3DXToRadian(PI), 100.f, &pTarget))
    {
        return Vector3Normalize(pTarget->m_Params.pos - m_pBaseball->m_Params.pos);
    }

    return chr_func::GetFront(m_pBaseball);
}

bool BaseballComputerUtillityClass::ShotControllClass_B::DoOtherAction()
{
    m_pReaction->Update();

    return m_pReaction->Reactioned();
}

bool BaseballComputerUtillityClass::ShotControllClass_B::DoShotAfterAction()
{
    return DoOtherAction();
}
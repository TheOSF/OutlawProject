#include "../Computer/SoccerComputerMove.h"
#include "../../Computer/CharacterComputerReactionHitEvent.h"
#include "../../CharacterFunction.h"
#include "../../CharacterMoveClass.h"
#include "../SoccerPlayerState.h"
#include "../SoccerRolling.h"
#include "../SoccerSliding.h"
#include "SoccerComputerDash.h"
#include "../SoccerPlayerState_PoseMotion.h"
#include "../../CharacterManager.h"
#include "../Computer/SoccerComputerUtilityClass.h"
#include "../../../Camera/Camera.h"
#include "../SoccerHitEvent.h"
#include "../SoccerCommonState.h"
#include "../../../Effect/EffectFactory.h"
#include "../../../Sound/Sound.h"
#include "../../../Collision/Collision.h"


SoccerState_ComputerControll_Dash::SoccerState_ComputerControll_Dash(
    SoccerPlayer* s,
    MoveType type,
    CharacterBase* pTarget):
    m_pTarget(pTarget),
    m_pChr(s)
{
    switch (type)
    {
    case MoveType::TargetAttack:
        m_pStateFunc = &SoccerState_ComputerControll_Dash::State_Attack;
        break;

    case MoveType::Escape:
        m_pStateFunc = &SoccerState_ComputerControll_Dash::State_Attack;
        break;

    case MoveType::SaveDistance:
        m_pStateFunc = &SoccerState_ComputerControll_Dash::State_Attack;
        break;

    default:
        MyAssert(false, "対応していないタイプ");
        break;
    }
}

//ステート開始
void SoccerState_ComputerControll_Dash::Enter(SoccerPlayer* s)
{
    m_pMoveClass = new SoccerDash(
        s
        );

    m_Timer = 0;

    m_isRightTurn = frand() < 0.5f;
}

void SoccerState_ComputerControll_Dash::Execute(SoccerPlayer* s)
{
    ++m_Timer;

    {
        Vector3 Vec = (this->*m_pStateFunc)();

        Vec -= m_pChr->m_Params.pos;

        if (Vec.Length() < 10.0f)
        {
            m_pMoveClass->SetEnd();
        }

        m_pMoveClass->SetDashVec(Vector3Normalize(Vec));
    }

    m_pMoveClass->Update();
}

void SoccerState_ComputerControll_Dash::Exit(SoccerPlayer* s)
{
    delete m_pMoveClass;
}

//-----------------------------------------------------------------//

Vector3 SoccerState_ComputerControll_Dash::State_Attack()
{
    if (m_pMoveClass->isDash() &&  Vector3Distance(m_pChr->m_Params.pos, m_pTarget->m_Params.pos) < 25.0f)
    {
        m_pChr->SetState(new SoccerState_Sliding(m_pChr));
    }

    return m_pTarget->m_Params.pos;
}

Vector3 SoccerState_ComputerControll_Dash::State_Escape()
{
    Vector3 ret(0, 0, 0);

    ret = -m_pTarget->m_Params.pos;
    ret.Normalize();

    ret = Vector3RotateAxis(Vector3AxisY, (frand()*2.0f - 1.0f)*PI*0.25f, ret);

    ret = m_pTarget->m_Params.pos + ret * 80.0f;

    if (Vector3Distance(m_pChr->m_Params.pos, m_pTarget->m_Params.pos) > 60.0f)
    {
        m_pMoveClass->SetEnd();
    }

    if (frand() <= 0.02f&&m_Timer > 30)
    {
        m_pMoveClass->SetEnd();
    }

    return ret;
}

Vector3 SoccerState_ComputerControll_Dash::State_SaveDistance()
{
    Vector3 ret;

    Vector3Cross(ret, Vector3AxisY, m_pTarget->m_Params.pos - m_pChr->m_Params.pos);

    if (m_isRightTurn)
    {
        ret = -ret;
    }

    ret += m_pChr->m_Params.pos;

    if (frand() <= 0.02f&&m_Timer > 30)
    {
        m_pMoveClass->SetEnd();
    }

    return ret;
}
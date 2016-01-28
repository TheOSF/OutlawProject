#include "SoccerComputerUtilityClass.h"
#include "../SoccerAttackState.h"
#include "../../CharacterFunction.h"
#include "SoccerComputerDash.h"
//-------------------------------------------------------------//

Vector3 SocceComputerrUtillityClass::ComputerRollingControll::GetVec()
{
    Vector3 vec = stick;

    const RADIAN Rotate = (frand()*2.0f - 1.0f)*PI*0.5f;

    vec = Vector3RotateAxis(Vector3AxisY, Rotate, vec);

	return vec;
}


//-------------------------------------------------------------//
//-------------------------------------------------------------//


SocceComputerrUtillityClass::AttackEvent::AttackEvent(SoccerPlayer* p):
m_pChr(p)
{
    
}

bool SocceComputerrUtillityClass::AttackEvent::Action(CharacterBase* pTarget, float len)
{
    
    if (len < 15.0f)
    {
        

        if (chr_func::isCanSpecialAttack(m_pChr) && frand() < 0.3f)
        {
            chr_func::AngleControll(m_pChr, pTarget);

            //必殺技ぶっぱ
            m_pChr->SetState(new SoccerState_PlayerControll_Finisher());
        }
        else if (frand() < 0.3f)
        {
            chr_func::AngleControll(m_pChr, pTarget);

            //近距離攻撃
            m_pChr->SetState(new SoccerAttackState(m_pChr));
        }
        else if (frand() < 0.2f)
        {
            //回避(中央に向かって
            Vector3 EscapeVec = m_pChr->m_Params.pos;

            EscapeVec = -EscapeVec;
            EscapeVec.Normalize();

            m_pChr->SetState(
                //ローリング
                new SoccerState_Rolling(new SocceComputerrUtillityClass::ComputerRollingControll(m_pChr, EscapeVec), false)
                );
        }
        else
        {
            //ダッシュで逃げる！！
            m_pChr->SetState(
                new SoccerState_ComputerControll_Dash(
                    m_pChr, 
                    SoccerState_ComputerControll_Dash::MoveType::Escape,
                    pTarget
                    ));
        }
        return true;
    }
    else
    {
        if (chr_func::isCanSpecialAttack(m_pChr) && frand() < 0.2f)
        {
            //必殺技ぶっぱ
            m_pChr->SetState(new SoccerState_PlayerControll_Finisher());
        }
        else if (frand() < 0.3f)
        {
            chr_func::AngleControll(m_pChr, pTarget);

            //遠距離攻撃
            m_pChr->SetState(new SoccerState_PlayerControll_Shot());
        }
        else if (frand() < 0.5f)
        {
            //ダッシュで距離を維持！！
            m_pChr->SetState(
                new SoccerState_ComputerControll_Dash(
                m_pChr,
                SoccerState_ComputerControll_Dash::MoveType::SaveDistance,
                pTarget
                ));
        }
        else 
        {
            //ダッシュで攻撃！！
            m_pChr->SetState(
                new SoccerState_ComputerControll_Dash(
                m_pChr,
                SoccerState_ComputerControll_Dash::MoveType::TargetAttack,
                pTarget
                ));
        }
    }

    return true;
}


//-------------------------------------------------------------//
//-------------------------------------------------------------//

SocceComputerrUtillityClass::ReactionEvent::ReactionEvent(SoccerPlayer* p) :
m_pChr(p)
{

}

void SocceComputerrUtillityClass::ReactionEvent::Reaction(const TypeParam& param, CrVector3 vec)
{
    if (param.isCanCounter)
    {
        if (frand() < 0.35f)
        {
            m_pChr->SetState(new SoccerState_PlayerControll_Counter());
        }
        else
        {
            Vector3 EscapeVec;

            Vector3Cross(EscapeVec, vec, Vector3AxisY);

            if (frand() < 0.5f)
            {
                EscapeVec = -EscapeVec;
            }

            m_pChr->SetState(
                //ローリング
                new SoccerState_Rolling(new SocceComputerrUtillityClass::ComputerRollingControll(m_pChr, EscapeVec), false)
                );
        }

    }
    else
    {
        if (param.isEnemyNear)
        {
            if (frand() < 0.5f)
            {

                Vector3 EscapeVec = m_pChr->m_Params.pos;

                EscapeVec = -EscapeVec;
                EscapeVec.Normalize();

                m_pChr->SetState(
                    //ローリング
                    new SoccerState_Rolling(new SocceComputerrUtillityClass::ComputerRollingControll(m_pChr, EscapeVec), false)
                    );
            }
            else
            {
                chr_func::AngleControll(m_pChr, m_pChr->m_Params.pos + vec);

                m_pChr->SetState(new SoccerAttackState(m_pChr));
            }
            return;
        }

        Vector3 EscapeVec = m_pChr->m_Params.pos;

        EscapeVec = -EscapeVec;
        EscapeVec.Normalize();

        m_pChr->SetState(
            //ローリング
            new SoccerState_Rolling(new SocceComputerrUtillityClass::ComputerRollingControll(m_pChr, EscapeVec), false)
            );
    }

}


CharacterComputerReaction::InParam SocceComputerrUtillityClass::ReactionEvent::GetInParam()
{
    CharacterComputerReaction::InParam param;

    param.CanNearAtkAreaSize = 12.0f;
    param.CounterAreaSize = 5.0f;

    return param;
}
#include "TennisComputerReactionEvent.h"
#include "../TennisPlayerState_Counter.h"
#include "../TennisState_Rolling.h"
#include "TennisComputerUtillityClass.h"
#include "TennisComputerState_Attack.h"
#include "../../CharacterFunction.h"

CharacterComputerReaction::InParam TennisComputerReactionEvent::GetCharacterComputerReactionInParam()
{
    CharacterComputerReaction::InParam param;

    param.CanNearAtkAreaSize = 15.0f;
    param.CounterAreaSize = 12.0f;

    return param;
}

void TennisComputerReactionEvent::Reaction(const TypeParam& param, CrVector3 vec)
{
    if (param.isCanCounter)
    {
        if (frand() < 0.7f)
        {
            m_pTennis->SetState(new TennisState_Counter());
        }
        else
        {
            Vector3 EscapeVec;

            Vector3Cross(EscapeVec, vec, Vector3AxisY);

            if (frand() < 0.5f)
            {
                EscapeVec = -EscapeVec;
            }

            m_pTennis->SetState(
                //ローリング
                new TennisState_Rolling(
                new TennisComputerUtillityClass::RollingCallBack(m_pTennis, EscapeVec)
                ));
        }
        
    }
    else
    {
        if (param.isEnemyNear)
        {
            if (frand() < 0.5f)
            {
                Vector3 EscapeVec;

                if (frand() < 0.5f)
                {
                    EscapeVec = -Vector3Normalize(m_pTennis->m_Params.pos);
                }
                else
                {
                    EscapeVec = -vec;
                }

                EscapeVec = Vector3RotateAxis(Vector3AxisY, (frand() - 0.5f)*PI, EscapeVec);
                
                m_pTennis->SetState(
                    //ローリング
                    new TennisState_Rolling(
                    new TennisComputerUtillityClass::RollingCallBack(m_pTennis, EscapeVec
                    )));
            }
            else
            {
                chr_func::AngleControll(m_pTennis, m_pTennis->m_Params.pos + vec);

                m_pTennis->SetState(
                    //ローリング
                    new TennisComputerState_Attack(m_pTennis, false)
                    );
            }
            
        }
        else
        {
            Vector3 EscapeVec;

            if (frand() < 0.5f)
            {
                EscapeVec = -Vector3Normalize(m_pTennis->m_Params.pos);
            }
            else
            {
                EscapeVec = -vec;
            }

            EscapeVec = Vector3RotateAxis(Vector3AxisY, (frand() - 0.5f)*PI, EscapeVec);

            m_pTennis->SetState(
                //ローリング
                new TennisState_Rolling(
                new TennisComputerUtillityClass::RollingCallBack(m_pTennis, EscapeVec
                )));
        }
    }

    
}


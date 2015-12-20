#include "TennisComputerReactionEvent.h"
#include "../TennisPlayerState_Counter.h"
#include "../TennisState_Rolling.h"
#include "TennisComputerUtillityClass.h"


void TennisComputerReactionEvent::Reaction(CharacterComputerReactionHitEvent::HitType hittype, Vector3 vec)
{
    if (hittype == CharacterComputerReactionHitEvent::HitType::CanCounter && isDoAction(m_Param.BallCounter))
    {
        m_pTennis->SetState(new TennisState_Counter());
    }
    else
    {
        //ƒ[ƒŠƒ“ƒO
        new TennisState_Rolling(
            new TennisComputerUtillityClass::RollingCallBack(m_pTennis, vec)
            );
    }
}


#pragma once


#include "../TennisPlayer.h"
#include "../TennisState_Rolling.h"
#include "../../Computer/CharacterComputerDoAction.h"
#include "../TennisPlayerState_SlowUpBall.h"
#include "TennisComputerReactionEvent.h"

class TennisComputerUtillityClass
{
public:

    class AttackEvent :public CharacterComputerDoAction::ActionEvent
    {
    public:
        AttackEvent(TennisPlayer* pTennis);

        bool Action(CharacterBase* pTarget, float len);

    private:
        TennisPlayer* const m_pTennis;
    };

    class SlowUpBallControllClass :public TennisPlayerState_SlowUpBall::ControllClass
    {
    public:
        SlowUpBallControllClass(TennisPlayer* const pChr, bool Smash);
        ~SlowUpBallControllClass();

        Vector3 GetVec();
        bool    DoOtherAction();
        bool    DoShotAfterAction();
        bool    isShot(bool isSmashFrame);

    private:
        TennisPlayer* const             m_pChr;
        const bool                      m_DoSmash;
        CharacterComputerReaction*      m_pCharacterComputerReaction;
        
    };

    class RollingCallBack :public TennisState_Rolling::CallBackClass
    {
    private:
        TennisPlayer* const m_pTennis;
        Vector3             m_EscapeVec;
    public:

        RollingCallBack(
            TennisPlayer*   pTennis,
            CrVector3       DamageVec
            ) :
            m_pTennis(pTennis)
        {
            //âÒîÇ∑ÇÈï˚å¸ÇéZèo
            Vector3 EscapeVec = Vector3Zero;

            Vector3Cross(EscapeVec, Vector3AxisY, DamageVec);

            EscapeVec.y = 0;
            EscapeVec.Normalize();

            //óêêîÇ≈ç∂âEîΩì]
            if (frand() < 0.5f)
            {
                EscapeVec = -EscapeVec;
            }

            //óêêîí≤êÆ
            EscapeVec += Vector3Rand();

            EscapeVec.y = 0;
            EscapeVec.Normalize();

        }

        Vector3 GetVec()override
        {
            return m_EscapeVec;
        }

        void CancelUpdate()override
        {

        }
    };


};
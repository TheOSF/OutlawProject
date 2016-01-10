#include "AmefootPlayerState_Tackle.h"
#include "../AmefootUsualHitEvent.h"
#include "../MoveClass/AmefootAttackClass.h"
#include "../../../GameSystem/GameController.h"


class AmefootPlayerState_Tackle::PlayerControllEvent : public AmefootAttackClass::Control
{
public:
    PlayerControllEvent(AmefootPlayer*const pCharacter) :
        m_pAmefoot(pCharacter)
    {}

    // ƒ|[ƒY‚ð‘±‚¯‚é‚©‚Ç‚¤‚©
    bool isPoseContinue()override
    {
        return controller::GetPush(controller::button::shikaku, m_pAmefoot->m_PlayerInfo.number);
    }

    Vector2 GetStickValue()const override
    {
        return controller::GetStickValue(controller::stick::left, m_pAmefoot->m_PlayerInfo.number);
    }

private:
    AmefootPlayer* const m_pAmefoot;
};



AmefootPlayerState_Tackle::AmefootPlayerState_Tackle(AmefootPlayer*const pCharacter) :
    m_pAmefoot(pCharacter),
    m_pAmefootAttackClass(nullptr)
{
    m_pAmefootAttackClass = new AmefootAttackClass(pCharacter, new PlayerControllEvent(pCharacter));
}

AmefootPlayerState_Tackle::~AmefootPlayerState_Tackle()
{
    delete m_pAmefootAttackClass;
}


void AmefootPlayerState_Tackle::Enter(AmefootPlayer* pCharacter)
{

}

void AmefootPlayerState_Tackle::Execute(AmefootPlayer* pCharacter)
{
    m_pAmefootAttackClass->Update();
}

void AmefootPlayerState_Tackle::Exit(AmefootPlayer* pCharacter)
{

}



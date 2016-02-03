#include "SoccerState_ControllVanish.h"
#include "../CharacterDamageControll.h"
#include "SoccerCommonState.h"
#include "../CharacterFunction.h"

SoccerState_ControllVanish::SoccerState_ControllVanish(SoccerPlayer*  pSoccer) :
m_pSoccer(pSoccer)
{
    class SoccerDamageControllEvent :public CharacterDamageControll::EventClass
    {
    private:
        SoccerPlayer* const m_pSoccer;
    public:
        SoccerDamageControllEvent(SoccerPlayer* p) :
            EventClass(p, SoccerPlayer::_ms_Vanish_Fly, &p->m_Renderer),
            m_pSoccer(p)
        {
            PosOffset.y = -2;
        }

        void SetVanishState(CrVector3 v)
        {
            if (chr_func::isDie(m_pSoccer))
            {
                m_pSoccer->SetState(new SoccerState_DamageMotion_Die(m_pSoccer, v));
            }
            else
            {
                m_pSoccer->SetState(new SoccerState_DamageVanish(m_pSoccer, v));
            }
        }
    };


    m_pMoveClass = new CharacterDamageControll(new SoccerDamageControllEvent(pSoccer));
}

SoccerState_ControllVanish::~SoccerState_ControllVanish()
{
    delete m_pMoveClass;
}

// ステート開始
void SoccerState_ControllVanish::Enter(SoccerPlayer* t)
{

}

// ステート実行
void SoccerState_ControllVanish::Execute(SoccerPlayer* t)
{
    m_pMoveClass->Update();
}

// ステート終了
void SoccerState_ControllVanish::Exit(SoccerPlayer* t)
{

}
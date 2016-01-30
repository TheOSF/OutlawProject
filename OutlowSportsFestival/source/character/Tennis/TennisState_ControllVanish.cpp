#include "TennisState_ControllVanish.h"
#include "../CharacterDamageControll.h"
#include "TennisPlayerState_Vanish.h"
#include "../CharacterFunction.h"
#include "TennisPlayerState_DamageMotionDie.h"

TennisState_ControllVanish::TennisState_ControllVanish(TennisPlayer*  pTennis):
m_pTennis(pTennis)
{
    class TennisDamageControllEvent :public CharacterDamageControll::EventClass
    {
    private:
        TennisPlayer* const m_pTennis;
    public:
        TennisDamageControllEvent(TennisPlayer* p) :
            EventClass(p, TennisPlayer::_mt_Damage_Vanish_Fly, &p->m_Renderer), 
            m_pTennis(p)
        {

        }

        void SetVanishState(CrVector3 v)
        {
            if (chr_func::isDie(m_pTennis))
            {
                m_pTennis->SetState(new TennisState_DamageMotion_Die(m_pTennis, v));
            }
            else
            {
                m_pTennis->SetState(new TennisState_DamageVanish(m_pTennis, v));
            }
        }
    };


    m_pMoveClass = new CharacterDamageControll(new TennisDamageControllEvent(pTennis));
}

TennisState_ControllVanish::~TennisState_ControllVanish()
{
    delete m_pMoveClass;
}

// ステート開始
void TennisState_ControllVanish::Enter(TennisPlayer* t)
{

}

// ステート実行
void TennisState_ControllVanish::Execute(TennisPlayer* t)
{
    m_pMoveClass->Update();
}

// ステート終了
void TennisState_ControllVanish::Exit(TennisPlayer* t)
{

}
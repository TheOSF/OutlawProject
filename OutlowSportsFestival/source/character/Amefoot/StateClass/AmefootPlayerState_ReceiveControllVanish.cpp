#include "AmefootPlayerState_ReceiveControllVanish.h"
#include "../AmefootPlayerState.h"
#include "../../CharacterDamageControll.h"
#include "../../CharacterFunction.h"

AmefootState_ControllVanish::AmefootState_ControllVanish(AmefootPlayer*  pAmefoot) :
m_pAmefoot(pAmefoot)
{
    class AmefootDamageControllEvent :public CharacterDamageControll::EventClass
    {
    private:
        AmefootPlayer* const m_pAmefoot;
    public:
        AmefootDamageControllEvent(AmefootPlayer* p) :
            EventClass(p, AmefootPlayer::Motion_Damage_Vanish_Start),
            m_pAmefoot(p)
        {

        }

        void SetVanishState(CrVector3 v)
        {
            if (chr_func::isDie(m_pAmefoot))
            {
                m_pAmefoot->SetState(new AmefootPlayerState_Die(m_pAmefoot, v));
            }
            else
            {
                m_pAmefoot->SetState(new AmefootPlayerState_ReceiveVanishDamage(v));
            }
        }
    };


    m_pMoveClass = new CharacterDamageControll(new AmefootDamageControllEvent(pAmefoot));
}

AmefootState_ControllVanish::~AmefootState_ControllVanish()
{
    delete m_pMoveClass;
}

// ステート開始
void AmefootState_ControllVanish::Enter(AmefootPlayer* t)
{

}

// ステート実行
void AmefootState_ControllVanish::Execute(AmefootPlayer* t)
{
    m_pMoveClass->Update();
}

// ステート終了
void AmefootState_ControllVanish::Exit(AmefootPlayer* t)
{

}
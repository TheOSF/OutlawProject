#include "BaseballPlayerState_Controll_Vanish.h"
#include "../CharacterDamageControll.h"
#include "BaseballPlayerState_DamageMotion_Vanish.h"
#include "../CharacterFunction.h"
#include "BaseballPlayerState_DamageMotion_Die.h"

BaseballState_ControllVanish::BaseballState_ControllVanish(BaseballPlayer*  pBaseball) :
m_pBaseBall(pBaseball)
{
    class BaseballDamageControllEvent :public CharacterDamageControll::EventClass
    {
    private:
        BaseballPlayer* const m_pBaseball;
    public:
        BaseballDamageControllEvent(BaseballPlayer* p) :
            EventClass(p, baseball_player::_mb_Damage_Vanish_Fly),
            m_pBaseball(p)
        {

        }

        void SetVanishState(CrVector3 v)
        {
            if (chr_func::isDie(m_pBaseball))
            {
                m_pBaseball->SetState(new BaseballState_DamageMotion_Die(m_pBaseball, v));
            }
            else
            {
                m_pBaseball->SetState(new BaseballState_DamageVanish(m_pBaseball, v));
            }
        }
    };


    m_pMoveClass = new CharacterDamageControll(new BaseballDamageControllEvent(pBaseball));
}

BaseballState_ControllVanish::~BaseballState_ControllVanish()
{
    delete m_pMoveClass;
}

// ステート開始
void BaseballState_ControllVanish::Enter(BaseballPlayer* t)
{

}

// ステート実行
void BaseballState_ControllVanish::Execute(BaseballPlayer* t)
{
    m_pMoveClass->Update();
}

// ステート終了
void BaseballState_ControllVanish::Exit(BaseballPlayer* t)
{

}
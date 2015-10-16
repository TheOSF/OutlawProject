#include "TennisPlayerState_SlowUpBall.h"
#include "../CharacterFunction.h"
#include "Tennis_HitEvent.h"

TennisState_SlowUpBall::TennisState_SlowUpBall(
    ControllClass* pControllClass   //操作コントロールするクラス(終了時にdeleteする)
    ) :
    m_pControllClass(pControllClass),
    m_Timer(0),
    m_EndFrame(80)
{

}


TennisState_SlowUpBall::~TennisState_SlowUpBall()
{

}


void TennisState_SlowUpBall::Enter(TennisPlayer* t)
{
    t->m_Renderer.SetMotion(TennisPlayer::_mt_Smash);
}

void TennisState_SlowUpBall::Execute(TennisPlayer* t)
{
    const int SlowFrame = 3;
    const int CanSwitchFrame = 5;

    TennisHitEvent HitEvent(t);

    chr_func::UpdateAll(t, &HitEvent);
    chr_func::CreateTransMatrix(t, t->m_ModelSize, &t->m_Renderer.m_TransMatrix);
    
    t->m_Renderer.Update(1);
}

void TennisState_SlowUpBall::Exit(TennisPlayer* t)
{

}





RATIO TennisState_SlowUpBall::GetChargePower()const
{
    return (float)m_Timer / (float)m_EndFrame;
}

TennisUpBall* TennisState_SlowUpBall::GetSlowUpBall()
{
    return m_pUpBall;
}


void TennisState_SlowUpBall::DeleteSlowBall()
{
    delete m_pUpBall;
    m_pUpBall = nullptr;
}
#include "TennisPlayerState_PoseMotion.h"
#include "TennisPlayerState_UsualMove.h"
#include "../CharacterFunction.h"


TennisState_PoseMotion::TennisState_PoseMotion(
    TennisPlayer::MotionType    Motion,
    RADIAN                      Speed,
    int                         Frame
    ):
    m_Motion(Motion),
    m_Speed(Speed),
    m_Frame(Frame)
{

}

TennisState_PoseMotion::~TennisState_PoseMotion()
{

}


// ステート開始
void TennisState_PoseMotion::Enter(TennisPlayer* t)
{
    t->m_Renderer.SetMotion(m_Motion);
}


// ステート実行
void TennisState_PoseMotion::Execute(TennisPlayer* t)
{
    if (--m_Frame <= 0)
    {
        t->SetState(TennisState_PlayerControll_Move::GetPlayerControllMove(t));
    }

    chr_func::XZMoveDown(t, 0.1f);

    chr_func::AngleControll(t, t->m_Params.pos - Vector3AxisZ, m_Speed);

    chr_func::UpdateAll(t, &DamageManager::HitEventBase());

    chr_func::CreateTransMatrix(t, &t->m_Renderer.m_TransMatrix);

    t->m_Renderer.Update(1);
}



// ステート終了
void TennisState_PoseMotion::Exit(TennisPlayer* t)
{

}
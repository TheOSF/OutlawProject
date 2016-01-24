#include "AmefootCommonState_PoseMotion.h"
#include "../../CharacterFunction.h"
#include "AmefootPlayerState_UsualMove.h"


AmefootCommonState_PoseMotion::AmefootCommonState_PoseMotion(
    AmefootPlayer::MotionType    Motion,
    RADIAN                      Speed,
    int                         Frame
    ) :
    m_Motion(Motion),
    m_Speed(Speed),
    m_Frame(Frame)
{

}

AmefootCommonState_PoseMotion::~AmefootCommonState_PoseMotion()
{

}


// ステート開始
void AmefootCommonState_PoseMotion::Enter(AmefootPlayer* p)
{
    p->m_Renderer.SetMotion(m_Motion);
}


// ステート実行
void AmefootCommonState_PoseMotion::Execute(AmefootPlayer* p)
{
    if (--m_Frame <= 0)
    {
        p->SetState(AmefootPlayerState_UsualMove::GetPlayerControllMove(p));
    }

    chr_func::XZMoveDown(p, 0.1f);

    chr_func::AngleControll(p, p->m_Params.pos - Vector3AxisZ, m_Speed);

    chr_func::UpdateAll(p, &DamageManager::HitEventBase());

    chr_func::CreateTransMatrix(p, &p->m_Renderer.m_TransMatrix);

    p->m_Renderer.Update(1);
}



// ステート終了
void AmefootCommonState_PoseMotion::Exit(AmefootPlayer* p)
{

}
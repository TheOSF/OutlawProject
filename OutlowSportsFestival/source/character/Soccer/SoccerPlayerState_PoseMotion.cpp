#include "SoccerPlayerState_PoseMotion.h"
#include "SoccerPlayerState.h"
#include "../CharacterFunction.h"


SoccerState_PoseMotion::SoccerState_PoseMotion(
	SoccerPlayer::MotionType    Motion,
	RADIAN                      Speed,
	int                         Frame
	) :
	m_Motion(Motion),
	m_Speed(Speed),
	m_Frame(Frame)
{

}

SoccerState_PoseMotion::~SoccerState_PoseMotion()
{

}


// ステート開始
void SoccerState_PoseMotion::Enter(SoccerPlayer* s)
{
	s->m_Renderer.SetMotion(m_Motion);
}


// ステート実行
void SoccerState_PoseMotion::Execute(SoccerPlayer* s)
{
	if (--m_Frame <= 0)
	{
		s->SetState(SoccerState_PlayerControll_Move::GetPlayerControllMove(s));
	}

	chr_func::AngleControll(s, s->m_Params.pos - Vector3AxisZ, m_Speed);

	chr_func::UpdateAll(s, &DamageManager::HitEventBase());

	chr_func::CreateTransMatrix(s, s->m_ModelSize, &s->m_Renderer.m_TransMatrix);

	s->m_Renderer.Update(1);
}



// ステート終了
void SoccerState_PoseMotion::Exit(SoccerPlayer* s)
{

}
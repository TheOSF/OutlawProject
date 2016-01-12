#include "BaseballPlayerState.h"
#include "BaseballState_PoseMotion.h"
#include "../CharacterFunction.h"


BaseballState_PoseMotion::BaseballState_PoseMotion(
	baseball_player::MotionType    Motion,
	RADIAN                      Speed,
	int                         Frame
	) :
	m_Motion(Motion),
	m_Speed(Speed),
	m_Frame(Frame)
{

}

BaseballState_PoseMotion::~BaseballState_PoseMotion()
{

}


// �X�e�[�g�J�n
void BaseballState_PoseMotion::Enter(BaseballPlayer* b)
{
	b->m_Renderer.SetMotion(m_Motion);
    chr_func::XZMoveDown(b, 1);
}


// �X�e�[�g���s
void  BaseballState_PoseMotion::Execute(BaseballPlayer* b)
{
	if (--m_Frame <= 0)
	{
		b->SetState(BaseballState_PlayerControll_Move::GetPlayerControllMove(b));
	}

	chr_func::AngleControll(b, b->m_Params.pos - Vector3AxisZ, m_Speed);

	chr_func::UpdateAll(b, &DamageManager::HitEventBase());

	chr_func::CreateTransMatrix(b, &b->m_Renderer.m_TransMatrix);

	b->m_Renderer.Update(1);
}



// �X�e�[�g�I��
void  BaseballState_PoseMotion::Exit(BaseballPlayer* b)
{

}
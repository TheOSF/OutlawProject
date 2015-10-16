#include "../CharacterMoveClass.h"
#include "../CharacterFunction.h"
#include "../CharacterBase.h"
#include "Damage/Damage.h"
#include "Ball/Ball.h"
#include "Camera/Camera.h"
#include "SoccerDash.h"
#include "SoccerPlayerState.h"
#include "SoccerRolling.h"
#include "SoccerHitEvent.h"
//****************************************************************
//		�L�����N�^���ʂ̓����N���X
//****************************************************************

SoccerDash::SoccerDash(
	SoccerPlayer*					pParent	//����L�����N�^�̃|�C���^
	) :
	m_StickValue(0, 0),
	m_pParent(pParent),
	m_isRun(false),
	m_Init(false),
	m_Locus(20)
{
	m_Locus.m_Division = 1;

	//�O�Ղ̐ݒ�
	m_Locus.m_Division = 0;
	m_Locus.m_StartParam.Width = 1.0f;
	m_Locus.m_EndParam.Width = 0.2f;


	UpdateLocusColor();
}

SoccerDash:: ~SoccerDash()
{
}

void SoccerDash::UpdateLocusColor()
{
	const DWORD Color = CharacterBase::GetPlayerColor(m_pParent->m_PlayerInfo.number);

	m_Locus.m_StartParam.Color = Vector4(
		float((Color >> 16) & 0xFF) / 255.f,
		float((Color >> 8) & 0xFF) / 255.f,
		float(Color & 0xFF) / 255.f,
		0.5f
		);
	

	m_Locus.m_EndParam.Color = Vector4(1, 1, 1, 0);
}

void SoccerDash::Update()
{
	const float Acceleration = 0.4f;
	const float MaxSpeed = 0.7f;
	const float TurnSpeed = 0.05f;

	{
		Vector3 v;
		Vector3Cross(v, m_pParent->m_Params.move, DefCamera.GetForward());
		v.Normalize();

		Vector3 BasePos = m_pParent->m_Params.pos;
		BasePos.y += 2.0f;

		m_Locus.AddPoint(BasePos, v);
	}

	{
	    //�����Ă��鏈���̎��s
		//�O�ɉ������A�������X�e�B�b�N�̌�����
		chr_func::AddMoveFront(m_pParent, Acceleration,MaxSpeed);

		chr_func::AngleControll(
			m_pParent,
			m_pParent->m_Params.pos + Vector3(m_StickValue.x, 0, m_StickValue.y),//DefCamera.GetRight()*m_StickValue.x + DefCamera.GetForward()*m_StickValue.y,
			TurnSpeed
			);
	}

	{
		SoccerHitEvent HitEvent(m_pParent);

		chr_func::UpdateAll(m_pParent, &HitEvent);
	}

	m_pParent->m_Renderer.Update(1);
	chr_func::CreateTransMatrix(m_pParent, m_pParent->m_ModelSize, &m_pParent->m_Renderer.m_TransMatrix);
}

void SoccerDash::SetStickValue(CrVector2 StickValue)
{
	m_StickValue = StickValue;
}
void SoccerDash::DoSliding()
{
	m_pParent->SetState(new SoccerState_PlayerControll_Sliding(m_pParent));
	return;
}
void SoccerDash::DoShot()
{
	
	
}
void SoccerDash::DoAvoid()
{
	
}
void SoccerDash::DoCounter()
{
	
}
void SoccerDash::DoFinisher()
{
	
}
void SoccerDash::DoStop()
{
	
}
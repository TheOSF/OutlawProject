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
#include "SoccerCommonState.h"
//****************************************************************
//		キャラクタ共通の動きクラス
//****************************************************************

SoccerDash::SoccerDash(
	SoccerPlayer*					pParent	//操るキャラクタのポインタ
	) :
	m_StickValue(0, 0),
	m_pParent(pParent),
	m_isRun(false),
	m_Init(false),
	m_Locus(20),
	m_SpeedEffect(1),
	m_count(0)
{
	m_Locus.m_Division = 1;
	//軌跡の設定
	m_Locus.m_Division = 0;
	m_Locus.m_StartParam.Width = 1.0f;
	m_Locus.m_EndParam.Width = 0.2f;

	m_pParent->m_Renderer.SetMotion(SoccerPlayer::_ms_Run);


	//UpdateLocusColor();
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
	++m_count;

	{
		Vector3 v;
		Vector3Cross(v, m_pParent->m_Params.move, DefCamera.GetForward());
		v.Normalize();

		Vector3 BasePos = m_pParent->m_Params.pos;
		BasePos.y += 2.0f;

		//m_Locus.AddPoint(BasePos, v);
	}

	{
	    //走っている処理の実行
		//前に加速し、方向をスティックの向きに
		chr_func::AddMoveFront(m_pParent, Acceleration,MaxSpeed);

		chr_func::AngleControll(
			m_pParent,
			m_pParent->m_Params.pos + Vector3(m_StickValue.x, 0, m_StickValue.y),//DefCamera.GetRight()*m_StickValue.x + DefCamera.GetForward()*m_StickValue.y,
			TurnSpeed
			);
	}

	//壁に向かって進むの防止
	if (chr_func::IsHitWall(m_pParent) && m_count>10)
	{
		m_pParent->SetState(new SoccerState_clash(m_pParent));

	}

	{
		SoccerHitEvent HitEvent(m_pParent);

		chr_func::UpdateAll(m_pParent, &HitEvent);
	}
	Vector3 chrpos = m_pParent->m_Params.pos;
	chrpos.y += 1.5f;
	m_SpeedEffect.Update(chrpos,-m_pParent->m_Params.move);
	m_pParent->m_Renderer.Update(1);
	chr_func::CreateTransMatrix(m_pParent, m_pParent->m_ModelSize, &m_pParent->m_Renderer.m_TransMatrix);
}

void SoccerDash::SetStickValue(CrVector2 StickValue)
{
	m_StickValue = StickValue;
}
#include "../CharacterMoveClass.h"
#include "../CharacterFunction.h"
#include "../CharacterBase.h"
#include "Damage/Damage.h"
#include "Ball/Ball.h"
#include "Camera/Camera.h"
#include "SoccerDash.h"
//****************************************************************
//		�L�����N�^���ʂ̓����N���X
//****************************************************************

SoccerDash::SoccerDash(
	CharacterBase*					pParent,	//����L�����N�^�̃|�C���^
	const Params&					param,		//�ړ��p�����[�^�\����
	MoveEvent*						pMoveEvent,	//�ړ��C�x���g�ɔ�������N���X
	DamageManager::HitEventBase*	pHitEventBase//�_���[�W���󂯂����ɔ�������N���X
	) :
	m_StickValue(0, 0),
	m_pParent(pParent),
	m_isRun(false),
	m_pMoveEvent(pMoveEvent),
	m_pHitEventBase(pHitEventBase),
	m_Init(false),
	m_Locus(20)
{
	m_Params = param;
	m_Locus.m_Division = 1;

	//�O�Ղ̐ݒ�
	m_Locus.m_Division = 0;
	m_Locus.m_StartParam.Width = 1.0f;
	m_Locus.m_EndParam.Width = 0.2f;


	UpdateLocusColor();
}

SoccerDash:: ~SoccerDash()
{
	delete m_pMoveEvent;
	delete m_pHitEventBase;
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
	//�����Ă��邩�ǂ���
	bool now = Vector2Length(m_StickValue) > 0.1f;

	Vector3 v;
	Vector3Cross(v, m_pParent->m_Params.move, DefCamera.GetForward());
	v.Normalize();

	Vector3 BasePos = m_pParent->m_Params.pos;
	BasePos.y += 2.0f;

	m_Locus.AddPoint(BasePos, v);

	//������
	if (m_Init == false)
	{
		m_Init = true;
		m_pMoveEvent->StandStart();
	}
	//�C�x���g�N���X�̍X�V
	m_pMoveEvent->Update(now, (m_Params.MaxSpeed > 0.0f) ? (Vector3XZLength(m_pParent->m_Params.move) / m_Params.MaxSpeed) : (0));

	//�����Ă���or���Ȃ������̎��s
	if (now)
	{
		//�O�ɉ������A�������X�e�B�b�N�̌�����
		chr_func::AddMoveFront(m_pParent, m_Params.Acceleration, m_Params.MaxSpeed);

		chr_func::AngleControll(
			m_pParent,
			m_pParent->m_Params.pos + Vector3(m_StickValue.x, 0, m_StickValue.y),//DefCamera.GetRight()*m_StickValue.x + DefCamera.GetForward()*m_StickValue.y,
			m_Params.TurnSpeed
			);
	}
	else
	{
		//����
		chr_func::XZMoveDown(m_pParent, m_Params.DownSpeed);
	}

	//�C�x���g�N���X�̊֐��̌Ăяo��
	if (now != m_isRun)
	{
		m_isRun = now;
		if (now)
		{
			m_pMoveEvent->RunStart();
		}
		else
		{
			m_pMoveEvent->StandStart();
		}
	}
	chr_func::UpdateAll(m_pParent, m_pHitEventBase);
	////�����蔻����Ƃ�
	//ShpereParam sp;

	//sp.pos = m_pParent->m_Params.pos;
	//sp.pos.y += BallBase::UsualBallShotY;
	//sp.size = m_pParent->m_Params.hitScale;

	//DefDamageMgr.HitCheckSphere(sp, *m_pHitEventBase);


	////�ʒu�̍X�V
	//chr_func::PositionUpdate(m_pParent);
}

void SoccerDash::SetStickValue(CrVector2 StickValue)
{
	m_StickValue = StickValue;
}
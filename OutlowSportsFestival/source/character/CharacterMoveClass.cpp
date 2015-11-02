#include "CharacterMoveClass.h"
#include "CharacterFunction.h"
#include "CharacterBase.h"
#include "../Damage/Damage.h"
#include "../Ball/Ball.h"
#include "../Camera/Camera.h"



CharacterUsualMove::Params::Params()
{
    Acceleration = 1.0f;
    DownSpeed = 0.1f;
    MaxSpeed = 1.0f;
    TurnSpeed = 1.0f;
    RunEndFrame = 0;
}

//****************************************************************
//		�L�����N�^���ʂ̓����N���X
//****************************************************************

CharacterUsualMove::CharacterUsualMove(
	CharacterBase*					pParent,	//����L�����N�^�̃|�C���^
	const Params&					param,		//�ړ��p�����[�^�\����
	MoveEvent*						pMoveEvent,	//�ړ��C�x���g�ɔ�������N���X
	DamageManager::HitEventBase*	pHitEventBase//�_���[�W���󂯂����ɔ�������N���X
	) :
	m_StickValue(0, 0),
	m_pParent(pParent),
    m_pStateFunc(&CharacterUsualMove::State_Init),
	m_pMoveEvent(pMoveEvent),
	m_pHitEventBase(pHitEventBase),
    m_FrameCounter(0)
{
	m_Params = param;
}

CharacterUsualMove:: ~CharacterUsualMove()
{
	delete m_pMoveEvent;
	delete m_pHitEventBase;
}

void CharacterUsualMove::Update()
{
    (this->*m_pStateFunc)();

    chr_func::UpdateAll(m_pParent, m_pHitEventBase);
}

void CharacterUsualMove::SetStickValue(CrVector2 StickValue)
{
	m_StickValue = StickValue;
}





void CharacterUsualMove::State_Init()
{
    m_pMoveEvent->StandStart();
    m_pStateFunc = &CharacterUsualMove::State_Stand;
}


void CharacterUsualMove::State_Stand()
{
    //�����Ă��邩�ǂ���
    if (Vector2Length(m_StickValue) > 0.1f)
    {
        m_pStateFunc = &CharacterUsualMove::State_Run;
        m_pMoveEvent->RunStart();
    }

    //����
    chr_func::XZMoveDown(m_pParent, m_Params.DownSpeed);

    m_pMoveEvent->Update(false, 0.0f);
}

void CharacterUsualMove::State_Run()
{
    //�O�ɉ������A�������X�e�B�b�N�̌�����
    chr_func::AddMoveFront(m_pParent, m_Params.Acceleration, m_Params.MaxSpeed);

    chr_func::AngleControll(
        m_pParent,
        m_pParent->m_Params.pos + Vector3(m_StickValue.x, 0, m_StickValue.y),
        m_Params.TurnSpeed
        );

    m_pMoveEvent->Update(true, (m_Params.MaxSpeed > 0.0f) ? (Vector3XZLength(m_pParent->m_Params.move) / m_Params.MaxSpeed) : (0));


    m_FrameCounter = min(m_FrameCounter + 1, 100);

    //�����Ă��邩�ǂ���
    if (Vector2Length(m_StickValue) <= 0.1f &&
        m_FrameCounter > 10)
    {
        m_FrameCounter = 0;

        m_pStateFunc = &CharacterUsualMove::State_Runend;
        m_pMoveEvent->RunEnd();
    }
}

void CharacterUsualMove::State_Runend()
{
    m_pMoveEvent->Update(true, (m_Params.MaxSpeed > 0.0f) ? (Vector3XZLength(m_pParent->m_Params.move) / m_Params.MaxSpeed) : (0));

    //����
    chr_func::XZMoveDown(m_pParent, m_Params.DownSpeed);

    //�ړ��l�����ȉ��Ȃ�Ƃ܂�
    if (Vector3XZLength(m_pParent->m_Params.move) <= 0.01f)
    {
        m_pStateFunc = &CharacterUsualMove::State_Stand;
        m_pMoveEvent->StandStart();
    }



    //�X�e�B�b�N���|����Ă����ꍇ�͑���X�e�[�g��
    if (Vector2Length(m_StickValue) > 0.1f)
    {
        m_pStateFunc = &CharacterUsualMove::State_Run;
        m_pMoveEvent->RunStart();
    }
}
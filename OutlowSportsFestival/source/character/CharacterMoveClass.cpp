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
//		キャラクタ共通の動きクラス
//****************************************************************

CharacterUsualMove::CharacterUsualMove(
	CharacterBase*					pParent,	//操るキャラクタのポインタ
	const Params&					param,		//移動パラメータ構造体
	MoveEvent*						pMoveEvent,	//移動イベントに反応するクラス
	DamageManager::HitEventBase*	pHitEventBase//ダメージを受けた時に反応するクラス
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
    //走っているかどうか
    if (Vector2Length(m_StickValue) > 0.1f)
    {
        m_pStateFunc = &CharacterUsualMove::State_Run;
        m_pMoveEvent->RunStart();
    }

    //減速
    chr_func::XZMoveDown(m_pParent, m_Params.DownSpeed);

    m_pMoveEvent->Update(false, 0.0f);
}

void CharacterUsualMove::State_Run()
{
    //前に加速し、方向をスティックの向きに
    chr_func::AddMoveFront(m_pParent, m_Params.Acceleration, m_Params.MaxSpeed);

    chr_func::AngleControll(
        m_pParent,
        m_pParent->m_Params.pos + Vector3(m_StickValue.x, 0, m_StickValue.y),
        m_Params.TurnSpeed
        );

    m_pMoveEvent->Update(true, (m_Params.MaxSpeed > 0.0f) ? (Vector3XZLength(m_pParent->m_Params.move) / m_Params.MaxSpeed) : (0));


    m_FrameCounter = min(m_FrameCounter + 1, 100);

    //走っているかどうか
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

    //減速
    chr_func::XZMoveDown(m_pParent, m_Params.DownSpeed);

    //移動値が一定以下ならとまる
    if (Vector3XZLength(m_pParent->m_Params.move) <= 0.01f)
    {
        m_pStateFunc = &CharacterUsualMove::State_Stand;
        m_pMoveEvent->StandStart();
    }



    //スティックが倒されていた場合は走るステートへ
    if (Vector2Length(m_StickValue) > 0.1f)
    {
        m_pStateFunc = &CharacterUsualMove::State_Run;
        m_pMoveEvent->RunStart();
    }
}
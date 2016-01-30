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
#include "../../Sound/Sound.h"
#include "../../Effect/EffectFactory.h"
#include "../../Collision/Collision.h"



SoccerDash::SoccerDash(SoccerPlayer* pParent):
m_pChr(pParent),
m_pStateFunc(&SoccerDash::State_Start),
m_SpeedEffect(1)
{

}

SoccerDash::~SoccerDash()
{

}


void SoccerDash::SetDashVec(CrVector3 Vec)
{
    if (!isDash())
    {
        return;
    }

    const float TurnSpeed = 0.075f;

    chr_func::AngleControll(
        m_pChr,
        m_pChr->m_Params.pos + Vec,
        TurnSpeed
        );

    //速度の向きを更新
    UpdateSpeed();
}

bool SoccerDash::SetEnd()
{
    if (!isDash())
    {
        return false;
    }

    SetState(&SoccerDash::State_DashEnd);

    return true;
}

bool SoccerDash::isDash()const
{
    return m_pStateFunc == &SoccerDash::State_Dash && m_StateTimer > 5;
}

void SoccerDash::Update()
{
    ++m_StateTimer;

    (this->*m_pStateFunc)();

    chr_func::UpdateAll(m_pChr, &SoccerHitEvent(m_pChr));
    chr_func::CreateTransMatrix(m_pChr, &m_pChr->m_Renderer.m_TransMatrix);

}

bool SoccerDash::isHitWall()
{
    Vector3 out, pos, vec;
    float dist = 5.0f;
    int mat = 0;

    pos = m_pChr->m_Params.pos + Vector3(0, 2, 0);
    vec = chr_func::GetFront(m_pChr);

    if (DefCollisionMgr.RayPick(
        &out,
        &pos,
        &vec,
        &dist,
        &mat,
        CollisionManager::RayType::_Character
        ) != nullptr)
    {
        return Vector3Radian(vec, -chr_func::GetFront(m_pChr)) < D3DXToRadian(30);
    }

    return false;
}

void SoccerDash::UpdateSpeed()
{
    const float MoveSpeed = 0.7f;

    m_pChr->m_Params.move = chr_func::GetFront(m_pChr)*MoveSpeed;
}

void SoccerDash::SetState(StateFunc pState)
{
    m_pStateFunc = pState;
    m_StateTimer = 0;
}


//----------------------------------------------------//
//                   StateFunc
//----------------------------------------------------//

void SoccerDash::State_Start()
{
    m_pChr->m_Renderer.SetMotion(SoccerPlayer::_ms_Run);

    SetState(&SoccerDash::State_Dash);

    Sound::Play(Sound::Soccer_Speed_Up1);

    m_pChr->m_Renderer.Update(1.0f);

    chr_func::SetControllerShock(m_pChr, 0.1f, 0.1f);
}

void SoccerDash::State_Dash()
{
    //速度をセット
    UpdateSpeed();

    if (isHitWall() && m_StateTimer > 15)
    {
        SetState(&SoccerDash::State_HitWall);
    }

    {
        if (m_StateTimer % 21 == 5)
        {
            Sound::Play(Sound::Sand1);
        }

        if (m_StateTimer % 3 == 2)
        {
            EffectFactory::Smoke(
                m_pChr->m_Params.pos + Vector3(frand() - 0.5f, frand() + 0.5f, frand() - 0.5f)*2.0f, 
                Vector3Zero,
                1.8f,
                1.0f,
                true
                );
        }
    }

    Vector3 chrpos = m_pChr->m_Params.pos;
    chrpos.y += 1.5f;

    m_SpeedEffect.Update(chrpos, -m_pChr->m_Params.move);

    m_pChr->m_Renderer.Update(1.0f);
}

void SoccerDash::State_DashEnd()
{
    if (m_StateTimer == 1)
    {
        m_pChr->m_Renderer.SetMotion(SoccerPlayer::_ms_DushStop);

        Sound::Play(Sound::Sand2);
        Sound::Play(Sound::Soccer_Brake);

        chr_func::SetControllerShock(m_pChr, 0.1f, 0.1f);

        for (int i = 0; i < 3; ++i)
        {
            EffectFactory::Smoke(
                m_pChr->m_Params.pos + Vector3(frand() - 0.5f, frand(), frand() - 0.5f)*2.0f,
                Vector3Zero,
                1.8f,
                0.2f,
                false
                );
        }
    }

    chr_func::XZMoveDown(m_pChr, 0.08f);

    if (m_StateTimer > 30)
    {
        SetState(&SoccerDash::State_Finish);
    }

    m_pChr->m_Renderer.Update(0.5f);
}

void SoccerDash::State_HitWall()
{
    if (m_StateTimer == 1)
    {
        m_pChr->m_Renderer.SetMotion(SoccerPlayer::_ms_DushStop);

        Sound::Play(Sound::AtkHit2);

        for (int i = 0; i < 3; ++i)
        {
            EffectFactory::Smoke(
                m_pChr->m_Params.pos + Vector3(frand() - 0.5f, frand(), frand() - 0.5f)*2.0f,
                Vector3Zero,
                1.8f,
                0.2f,
                false
                );
        }

    }

    chr_func::XZMoveDown(m_pChr, 0.08f);

    if (m_StateTimer > 40)
    {
        SetState(&SoccerDash::State_Finish);
    }

    m_pChr->m_Renderer.Update(1.0f);
}

void SoccerDash::State_Finish()
{
    m_pChr->SetState(SoccerState_PlayerControll_Move::GetPlayerControllMove(m_pChr));
}


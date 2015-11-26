#include "BallEffect.h"
#include "../Effect/EffectFactory.h"
#include "../character/CharacterBase.h"


BallEffect::BallEffect() :
m_EffectFrameCount(0),
m_pBall(nullptr)
{

}

BallEffect::~BallEffect()
{

}

void BallEffect::SetParent(BallBase* pBall)
{
    m_pBall = pBall;
}

void BallEffect::Counter()
{
    m_EffectFrameCount = 45;
}

void BallEffect::Update()
{

    if (m_pBall == nullptr)
    {
        return;
    }

    //パーティクル
    {
        const float EffectScale = 0.1f;

        m_EffectFrameCount = max(m_EffectFrameCount - 1, 0);

        if (m_EffectFrameCount % 2 == 0 && m_EffectFrameCount > 0)
        {
            EffectFactory::CircleAnimation(
                m_pBall->m_Params.pos,
                m_pBall->m_Params.move,
                Vector3Zero,
                Vector3Zero,
                Vector2(23.f, 23.f)*EffectScale*((float)m_EffectFrameCount / 45.0f),
                0x80FFFFFF,
                CharacterBase::GetPlayerColor(m_pBall->m_Params.pParent->m_PlayerInfo.number)
                );
        }
    }
}
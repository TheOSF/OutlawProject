#include "AmefootPlayerState_BallTouchDown.h"
#include "../../CharacterFunction.h"
#include "../AmefootUsualHitEvent.h"
#include "AmefootPlayerState_UsualMove.h"
#include "../../../utillity/DamageObject.h"
#include "../../../Sound/Sound.h"
#include "../../../Effect/EffectFactory.h"

AmefootPlayerState_BallTouchDown::AmefootPlayerState_BallTouchDown(AmefootPlayer* pChr) :
m_pChr(pChr),
m_Timer(0),
m_pStateFunc(&AmefootPlayerState_BallTouchDown::State_Jump)
{

}


void AmefootPlayerState_BallTouchDown::Enter(AmefootPlayer* pChr)
{
    
}

void AmefootPlayerState_BallTouchDown::Execute(AmefootPlayer* pChr)
{
    m_Timer++;

    (this->*m_pStateFunc)();

    
    chr_func::CreateTransMatrix(m_pChr, &m_pChr->m_Renderer.m_TransMatrix);
}

void AmefootPlayerState_BallTouchDown::Exit(AmefootPlayer* pChr)
{

}

//--------------------------------------------------------------------------//
//                      private
//--------------------------------------------------------------------------//

void AmefootPlayerState_BallTouchDown::SetState(StateFunc state)
{
    m_pStateFunc = state;
    m_Timer = 0;
}

void AmefootPlayerState_BallTouchDown::State_Jump()
{
    const float MoveY = 0.35f;
    const float MoveXZ = 0.55f;

    const int AllFrame = 23;
    const int NoDamageFrame = 10;

    if (m_Timer == 1)
    {
        //移動量をセット
        m_pChr->m_Params.move = chr_func::GetFront(m_pChr)*MoveXZ + Vector3(0, MoveY, 0);

        //モーションをセット
        m_pChr->m_Renderer.SetMotion(AmefootPlayer::Motion_Touchdown_Finish);
    }

    if (m_Timer == AllFrame - 3)
    {
        m_pChr->m_Params.move.y = -1.0f;
    }

    if (m_Timer > AllFrame)
    {
        SetState(&AmefootPlayerState_BallTouchDown::State_Down);
    }

    //更新
    if (m_Timer < NoDamageFrame)
    {
        //無敵更新
        chr_func::UpdateAll(m_pChr, &DamageManager::HitEventBase());
    }
    else
    {
        //通常更新
        chr_func::UpdateAll(m_pChr, &AmefootUsualHitEvent(m_pChr));
    }

    m_pChr->m_Renderer.Update(2);
}

void AmefootPlayerState_BallTouchDown::State_Down()
{
    const int AllFrame = 40;

    if (m_Timer == 1)
    {
        //ダメージ判定をセット
        {
            DamageShpere* d = new DamageShpere();

            d->HitMotionFrame = 30;
            d->m_Param.pos = m_pChr->m_Params.pos + chr_func::GetFront(m_pChr)*2.0f;
            d->m_Param.size = 5.0f;
            d->m_VecPower.x = 0.1f;
            d->m_VecType = DamageShpere::DamageVecType::CenterToPosXZ;
            d->pParent = m_pChr;
            d->Value = 5.0f;
            d->type = DamageBase::Type::_WeekDamage;

            new DamageObject(d, 3);
        }

        //エフェクトをセット
        const float SmokeSize = 3.0f;
        Vector3 SmokePos;

        for (int i = 0; i < 20; ++i)
        {
            SmokePos = m_pChr->m_Params.pos + chr_func::GetFront(m_pChr)*3.0f;

            SmokePos.x += (frand() - 0.5f)*5.0f;
            SmokePos.z += (frand() - 0.5f)*5.0f;

            SmokePos.y += frand()*3.5f + SmokeSize*0.5f;

            EffectFactory::Smoke(
                SmokePos,
                Vector3(frand() - 0.5f, 0.5f, frand() - 0.5f)*0.15f,
                SmokeSize,
                0.2f
                );
        }

        //SE
        Sound::Play(Sound::Explode);
    }

    //移動量をダウン
    chr_func::XZMoveDown(m_pChr, 0.1f);


    //通常ステートに戻る
    if (m_Timer > AllFrame)
    {
        m_pChr->SetState(AmefootPlayerState_UsualMove::GetPlayerControllMove(m_pChr));
    }

    //更新
    chr_func::UpdateAll(m_pChr, &AmefootUsualHitEvent(m_pChr));

    m_pChr->m_Renderer.Update(0.5f);
}


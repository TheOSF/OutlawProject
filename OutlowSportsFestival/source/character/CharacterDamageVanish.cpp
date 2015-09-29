#include "CharacterDamageVanish.h"
#include "CharacterFunction.h"
#include "CharacterBase.h"

#include "../Effect/EffectFactory.h"
#include "../Sound/Sound.h"

CharacterDamageVanish::CharacterDamageVanish(
    CharacterBase*    pCharacter,//吹き飛ぶキャラクタ
    const Param&      param, //吹きとびパラメーター
    Event*            pEvent, //イベントクラスへのポインタ(デストラクタでdeleteする)
    DamageManager::HitEventBase* pHitEvent
    ) :
    m_pCharacter(pCharacter),
    m_pEvent(pEvent),
    m_pStateFunc(&CharacterDamageVanish::Initialize),
    m_Count(0),
    m_Rotate(0,0,0),
    m_pHitEvent(pHitEvent)
{
    m_Param = param;
}


CharacterDamageVanish::~CharacterDamageVanish()
{
    delete m_pEvent;
    delete m_pHitEvent;
}


// 更新
void CharacterDamageVanish::Update()
{
    (this->*m_pStateFunc)();
}


void CharacterDamageVanish::Initialize()
{
    //キャラクタをダメージの方向に向かせる
    chr_func::AngleControll(
        m_pCharacter,
        m_pCharacter->m_Params.pos - m_Param.move
        );

    //移動方向を設定
    m_pCharacter->m_Params.move = m_Param.move;

    //スタートイベント呼び出し
    m_pEvent->FlyStart();

    //吹き飛びステートへ
    m_pStateFunc = &CharacterDamageVanish::Flying;

    //移動更新
    chr_func::UpdateAll(m_pCharacter, &DamageManager::HitEventBase());

    //サウンド
    Sound::Play(Sound::Damage2);
}

void CharacterDamageVanish::Flying()
{
    ++m_Count;

    //エフェクト
    {
        EffectFactory::Smoke(
            m_pCharacter->m_Params.pos + Vector3(0,2,0) +Vector3Rand()*0.2f, 
            Vector3Zero,
            1.0f + frand()*0.5f,
            0x20FFFFFF
            );
    }

    {
        //飛行中の回転行列を作成する
        Matrix R;

        m_Rotate += m_Param.rotate_speed;

        D3DXMatrixRotationYawPitchRoll(
            &R,
            m_Rotate.y, m_Rotate.x, m_Rotate.z
            );

        //吹き飛び中関数呼び出し
        m_pEvent->Flying(R);
    }

    {
        //移動更新
        chr_func::UpdateAll(m_pCharacter, &DamageManager::HitEventBase());
    }


    //地面についていた場合はステート移行
    if (chr_func::isTouchGround(m_pCharacter) &&
        m_pCharacter->m_Params.move.y <= 0)
    {
        m_pEvent->DownStart();
        m_pStateFunc = &CharacterDamageVanish::Dowing;
        m_Count = 0;

        //ズザー音
        Sound::Play(Sound::Sand2);
    }
}


void CharacterDamageVanish::Dowing()
{
    ++m_Count;

    chr_func::XZMoveDown(m_pCharacter, 0.08f);

    m_pEvent->Downing();


    //エフェクト
    if (m_Count > 4 && m_Count < 10)
    {
        EffectFactory::Smoke(
            m_pCharacter->m_Params.pos + Vector3(frand() - 0.5f, frand(), frand() - 0.5f)*2.0f,
            Vector3(frand() - 0.5f, frand()*0.1f, frand() - 0.5f)*0.05f, 
            2.0f + frand()*2.0f,
            0xFFFFA080,
            true
            );
    }

    //移動更新
    if (m_Count > m_Param.down_muteki_frame)
    {
        //通常当たり判定
        chr_func::UpdateAll(m_pCharacter, m_pHitEvent);
    }
    else
    {
        //ノーダメージ版
        chr_func::UpdateAll(m_pCharacter, &DamageManager::HitEventBase());
    }


    //ステート移行
    if (m_Count > m_Param.down_frame)
    {
        m_pEvent->StandUpStart();
        m_pStateFunc = &CharacterDamageVanish::StandUping;
        m_Count = 0;
    }
}


void CharacterDamageVanish::StandUping()
{
    ++m_Count;

    chr_func::XZMoveDown(m_pCharacter, 0.08f);

    m_pEvent->StandUping();


    //移動更新
    if (m_Count > m_Param.standup_muteki_frame)
    {
        //通常当たり判定
        chr_func::UpdateAll(m_pCharacter, m_pHitEvent);
    }
    else
    {
        //ノーダメージ版
        chr_func::UpdateAll(m_pCharacter, &DamageManager::HitEventBase());
    }



    //ステート移行
    if (m_Count > m_Param.standup_frame)
    {
        m_pEvent->End();
        m_pStateFunc = &CharacterDamageVanish::End;
        m_Count = 0;
    }
}


void CharacterDamageVanish::End()
{
    //移動更新
    {
        chr_func::UpdateAll(m_pCharacter, m_pHitEvent);
    }

    if (m_Count == 0)
    {
        m_Count = 1;
        m_pEvent->End();
    }
}
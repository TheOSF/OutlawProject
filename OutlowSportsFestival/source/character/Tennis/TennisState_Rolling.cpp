#include "TennisState_Rolling.h"
#include "../CharacterFunction.h"
#include "TennisPlayerState_UsualMove.h"
#include "Tennis_HitEvent.h"
#include "../../Effect/EffectFactory.h"
#include "../../Sound/Sound.h"

TennisState_Rolling::TennisState_Rolling(CallBackClass* pCallBackClass) :
m_pCallBackClass(pCallBackClass)
{

}

TennisState_Rolling::~TennisState_Rolling()
{
    delete m_pCallBackClass;
}


// ステート開始
void TennisState_Rolling::Enter(TennisPlayer* t)
{
    m_Timer = 0;

    //デフォルトのローリング方向としてキャラの前ベクトルを入れる
    chr_func::GetFront(t, &m_Vec);

    //キャラクタの移動量を初期化
    chr_func::ResetMove(t);
}

// ステート実行
void TennisState_Rolling::Execute(TennisPlayer* t)
{
    const int EndFrame = 52;          //終了までのフレーム
    const int CanControllFrame = 2;   //移動方向をコントロールできるフレーム
    const int NoDamageFrame = 10;     //無敵時間

    const float MoveValue = 0.7f;    //移動量
    const float DownValue = 0.05f;     //減速量

    //フレームカウント更新
    ++m_Timer;

    //モーションセット
    if (m_Timer == 1)
    {
        t->m_Renderer.SetMotion(TennisPlayer::_mt_Rolling);
    }

    //移動方向をコントロール
    if (m_Timer < CanControllFrame)
    {
        Vector3 Vec = m_pCallBackClass->GetVec();

        //値があった場合は更新
        if (Vec != Vector3Zero)
        {
            m_Vec = Vec;
        }
    }

    //コントロールできるフレームが終わった場合向きと移動を設定
    if (m_Timer == CanControllFrame)
    {
        m_Vec.y = 0;
        m_Vec.Normalize();

        chr_func::AngleControll(t, t->m_Params.pos + m_Vec);

        chr_func::AddMoveFront(t, MoveValue, MoveValue);
    }

    //終了
    if (m_Timer == EndFrame)
    {
        //通常ステートに移行
        t->SetState(TennisState_PlayerControll_Move::GetPlayerControllMove(t));
    }

    //煙エフェクト
    {
        //スタート時
        if (m_Timer == 2)
        {
            for (int i = 0; i < 2; ++i)
            {
                EffectFactory::Smoke(
                    t->m_Params.pos + Vector3(frand() - 0.5f, frand(), frand() - 0.5f)*2.0f,
                    Vector3Zero,
                    1.5f,
                    0xFFFFA080,
                    true
                    );
            }
        }

        //軌跡
        if (m_Timer < 5)
        {
            for (int i = 0; i < 2; ++i)
            {
                EffectFactory::Smoke(
                    t->m_Params.pos + Vector3(0, 2, 0) + Vector3Rand() * 0.2f, 
                    Vector3Zero,
                    1.0f + frand()*0.5f,
                    0x20FFA080
                    );
            }
        }

        //着地時
        if (m_Timer == 43)
        {
            for (int i = 0; i < 2; ++i)
            {
                EffectFactory::Smoke(
                    t->m_Params.pos + Vector3(frand() - 0.5f, frand(), frand() - 0.5f)*2.0f,
                    Vector3Zero,
                    2.5f,
                    0xFFFFA080,
                    true
                    );
            }
        }

    }

    //サウンド
    if (m_Timer == EndFrame - 25)
    {
        //ズザー音
        Sound::Play(Sound::Sand1);
    }

    //基本的な更新
    {
        DamageManager::HitEventBase NoDmgHitEvent;   //ノーダメージ
        TennisHitEvent              UsualHitEvent(t);//通常


        //移動量の減衰
        chr_func::XZMoveDown(t, DownValue);

        //無敵フレームかによってヒットイベントクラスの分岐
        if (m_Timer < NoDamageFrame)
        {
            chr_func::UpdateAll(t, &NoDmgHitEvent);
        }
        else
        {
            chr_func::UpdateAll(t, &UsualHitEvent);
        }
        
        //モデル更新
        t->m_Renderer.Update(1);

        //行列更新
        chr_func::CreateTransMatrix(t, t->m_ModelSize, &t->m_Renderer.m_TransMatrix);
    }
}

// ステート終了
void TennisState_Rolling::Exit(TennisPlayer* t)
{

}
#include "TennisState_Rolling.h"
#include "../CharacterFunction.h"
#include "TennisPlayerState_UsualMove.h"
#include "Tennis_HitEvent.h"
#include "../../Effect/EffectFactory.h"
#include "../../Sound/Sound.h"
#include "../../Camera/Camera.h"
#include "../../Effect/SpeedEffect.h"

TennisState_Rolling::TennisState_Rolling(CallBackClass* pCallBackClass) :
m_pCallBackClass(pCallBackClass),
m_SpeedEffect(1)
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
    const int EndFrame = 27;          //終了までのフレーム
    const int CanControllFrame = 4;   //移動方向をコントロールできるフレーム
    const int NoDamageFrame = 15;     //無敵時間
    const int CanCancel = 15;         //キャンセル可能フレーム

    const float MoveValue = 0.5f;    //移動量
    const float DownValue = 0.135f;     //減速量

    //フレームカウント更新
    ++m_Timer;

    //モーションセット
    if (m_Timer == 1)
    {
        t->m_Renderer.SetMotion(TennisPlayer::_mt_EscapeStep);
    }

    //移動方向をコントロール
    if (m_Timer < CanControllFrame)
    {
        Vector3 Vec = m_pCallBackClass->GetVec();

        //値があった場合は更新
        if (Vec != Vector3Zero)
        {
            m_Vec = Vec;
            chr_func::AngleControll(t, t->m_Params.pos + m_Vec, 0.3f);
        }
    }

    //コントロールできるフレームが終わった場合向きと移動を設定
    if (m_Timer == CanControllFrame)
    {
        chr_func::AngleControll(t, t->m_Params.pos + m_Vec);

        chr_func::AddMoveFront(t, MoveValue, MoveValue);
    }

    //エフェクト
    if (m_Timer < 13 && m_Timer > CanControllFrame)
    {
        m_SpeedEffect.Update(t->m_Params.pos + Vector3(0, 2, 0) + chr_func::GetFront(t)*2.8f, -chr_func::GetFront(t)*0.35f);
    }

    //キャンセル行動
    if (CanCancel < m_Timer)
    {
        m_pCallBackClass->CancelUpdate();
    }

    //終了
    if (m_Timer == EndFrame)
    {
        //通常ステートに移行
        t->SetState(TennisState_PlayerControll_Move::GetPlayerControllMove(t));
    }

    //基本的な更新
    {
        DamageManager::HitEventBase NoDmgHitEvent;   //ノーダメージ
        TennisHitEvent              UsualHitEvent(t);//通常


        //移動量の減衰
        if (m_Timer > EndFrame - 13)
        {
            chr_func::XZMoveDown(t, DownValue);
        }
        

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
        chr_func::CreateTransMatrix(t, &t->m_Renderer.m_TransMatrix);
    }

    //軌跡
    //{
    //    EffectFactory::SmokeParticle(
    //        t->m_Params.pos + Vector3(0, 2, 0) + Vector3Rand() * 0.2f,
    //        Vector3Zero,
    //        25,
    //        0.5f + frand()*0.5f,
    //        0x10FFA080
    //        );
    //}
}

// ステート終了
void TennisState_Rolling::Exit(TennisPlayer* t)
{

}
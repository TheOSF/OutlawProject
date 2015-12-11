#include "AmefootThrowBall.h"
#include "../AmefootPlayer.h"
#include "../../CharacterFunction.h"
#include "../AmefootPlayerState.h"


//-----------------------------------------------------------------------------------------
// AmefootThrowBall
// [ アメフト ] 遠距離攻撃(ボール投げ)クラス
// アメフト専用なのでイベントを使用しない
//-----------------------------------------------------------------------------------------
AmefootThrowBall::AmefootThrowBall(
    AmefootPlayer* pAmefoot,
    const Params& params,
    DamageManager::HitEventBase* pHitEventBase
    ) :
    m_pAmefoot(pAmefoot),
    m_Params(params),
    m_pHitEventBase(pHitEventBase),
    m_pStateFunc(&AmefootThrowBall::Begin)
{}
//-----------------------------------------------------------------------------------------
// スティックの値セット(投げる方向を決める)
void AmefootThrowBall::SetStickValue(CrVector2 stick)
{
    m_StickValue = stick;

    //カメラ基準に変換
    Vector3 temp(m_StickValue.x, m_StickValue.y, 0);

    temp = Vector3MulMatrix3x3(temp, matView);

    m_StickValue.x = temp.x;
    m_StickValue.y = temp.y;
}
//-----------------------------------------------------------------------------------------
// 更新
bool AmefootThrowBall::Update()
{
    if ( m_pStateFunc )
    {
        (this->*m_pStateFunc)();
    }
    return true;
}
//-----------------------------------------------------------------------------------------
// 開始
void AmefootThrowBall::Begin()
{
    // カウンタ更新
    ++m_Counter;

    if ( m_Counter == 1 )
    {

        // ボール生成


        // モーションセット
        m_pAmefoot->m_Renderer.SetMotion(m_Params.ThrowMotionNumber);
    }

    // 投げステートへ
    if ( m_Counter < m_Params.ThrowFrame )
    {
        SetState(&AmefootThrowBall::Throw);
    }

    // 基本的な更新
    chr_func::UpdateAll(m_pAmefoot, m_pHitEventBase);
}
//-----------------------------------------------------------------------------------------
// 投げ
void AmefootThrowBall::Throw()
{
    // カウンタ更新
    ++m_Counter;

    if ( m_Counter == 1 )
    {
        // スティックの値による角度補正
        chr_func::AngleControll(
            m_pAmefoot,
            m_pAmefoot->m_Params.pos + Vector3(m_StickValue.x, 0, m_StickValue.y),
            m_Params.MaxTurnRadian
            );

        // キャラクターの前方向に飛ばす

        // ボールの位置補正

        // ゲージ上昇？

        // ボールの設定

        // エフェクト

        // サウンド

        // 前に進む？
    }

    // ステート終了
    if ( m_Counter > m_Params.AfterThrowFrame )
    {
        SetState(&AmefootThrowBall::End);
        return;
    }

    // 基本的な更新
    chr_func::UpdateAll(m_pAmefoot, m_pHitEventBase);

}
//-----------------------------------------------------------------------------------------
// 終了
void AmefootThrowBall::End()
{
    if ( m_Counter == 0 )
    {
        //一回しか実行しないように
        ++m_Counter;

        // ステート移行
        // プレイヤー専用しかないのでとりあえずそうする
        m_pAmefoot->SetState(new AmefootPlayerState_UsualMove());
    }

    // 基本的な更新
    chr_func::UpdateAll(m_pAmefoot, m_pHitEventBase);
}
//-----------------------------------------------------------------------------------------
// ステート設定
void AmefootThrowBall::SetState(StateFunc func)
{
    m_pStateFunc = func;
    m_Counter = 0;
}



#pragma once

#include "../../../Damage/Damage.h"

class AmefootPlayer;

//-----------------------------------------------------------------------------------------
// AmefootThrowBall
// [ アメフト ] 遠距離攻撃(ボール投げ)クラス
// アメフト専用なのでイベントを使用しない
//-----------------------------------------------------------------------------------------
class AmefootThrowBall
{
public:
    struct Params
    {
        int ThrowMotionNumber; // 投げモーションの番号

        UINT ThrowFrame; // 投げるまでのフレーム
        UINT AfterThrowFrame; // 投げ終わった後のフレーム

        UINT ThrowBoneNumber; // ボールをどのボーンにつけるか

        float ShotPowr; // 打ち出す強さ
        float MoveDownSpeed; // 減速
        float Power; // 攻撃力
        float MaxTurnRadian; // 向き補正の角度制限
        float AngleControlSpeed; // 角度変更速度
    };

public:
    AmefootThrowBall(
        AmefootPlayer* pAmefoot,
        const Params& params,
        DamageManager::HitEventBase* pHitEventBase
        );

    // スティックの値セット(投げる方向を決める)
    void SetStickValue(CrVector2 stick);

    // 更新
    bool Update();


private:
    void Begin(); // 開始
    void Throw(); // 投げ
    void End(); // 終了

private:
    typedef void(AmefootThrowBall::*StateFunc)();

    // ステート設定
    void SetState(StateFunc func);

private:
    DamageManager::HitEventBase* m_pHitEventBase;
    UINT m_Counter; // カウンタ
    StateFunc m_pStateFunc; // ステート関数
    AmefootPlayer* m_pAmefoot; // オーナーキャラクタ
    const Params m_Params; // パラメータ
    Vector2 m_StickValue;

};





#pragma once

#include "../AmefootPlayer.h"
#include "../../../Damage/Damage.h"
#include "../../../Effect/SpeedEffect.h"

class AmefootPlayerState_Tackle : public AmefootState
{
public:

    AmefootPlayerState_Tackle(AmefootPlayer* const pCharacter);

    ~AmefootPlayerState_Tackle();


    void Enter(AmefootPlayer* pCharacter)override;

    void Execute(AmefootPlayer* pCharacter)override;

    void Exit(AmefootPlayer* pCharacter)override;

    void StateMachineExit(AmefootPlayer* pCharacter)override;

private:

    AmefootPlayer* const m_pAmefootPlayer;

public:
    typedef void(AmefootPlayerState_Tackle::*StateFunc)();

    // ステート切り替え
    void SetState(StateFunc state);

    void Pose();                // 構え中
    void Tackle();              // タックル中
    void DriveAway();           // タックル打ち上げ中
    void Jump();                // 飛び上がり中
    void Touchdown();           // タッチダウン中
    void Standup();             // 立ち上がり中
    void Failed();              // 失敗
    void End();                 // 終了
    void HitStop();             // ヒットストップ

    void PoseStart();           // 構え開始の瞬間
    void TackleStart(float MoveValue); // タックル開始の瞬間
    void JumpStart();           // ジャンプ開始の瞬間
    void DrivaAwayStart();      // タックル打ち上げ開始の瞬間
    void TouchdownStart();      // タッチダウン開始の瞬間
    void StandupStart();        // 立ち上がり開始の瞬間
    void FailedStart();         // 失敗開始の瞬間

    void SmokeEffect();

    //パワーから突進のパラメータを得るゲッタ
    void GetTackleParamByPower(RATIO p, int *pOutFrame, float* pOutMoveValue)const;

    //引っ付きクラスを腕の位置に更新する
    void UpdateDamageTransform();

    //向きたい方向をゲット！
    Vector2 GetControllVec()const;

    //まだ構えるかどうかを得る
    bool isPoseContinue()const;

    //通常更新
    void CharacterUsualUpdate();

    //アーマー状態の更新
    void CharacterNoDamageUpdate();

private:
    DamageControllVanish*      m_pControllDamage;
    DamageControll_Transform*  m_pDamageTransform;
    SpeedEffect                m_SpeedEffect;

    int m_Timer = 0;
    RATIO m_TacklePower = 0.0f;
    StateFunc m_pStateFunc;
};


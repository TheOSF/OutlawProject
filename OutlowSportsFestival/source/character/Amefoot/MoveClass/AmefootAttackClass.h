#pragma once
#include "../../../Damage/Damage.h"

class AmefootPlayer;

class AmefootAttackClass
{
public:
    class Control
    {
    public:
        // ポーズを続けるかどうか
        virtual bool isPoseContinue() = 0;

        // スティックの入力値を取得
        virtual Vector2 GetStickValue()const = 0;
    };

    class DamageHitCallback : public DamageBase::HitCallBack
    {
    public:
        DamageHitCallback(AmefootAttackClass* pAmefootAttackClass);

        //ダメージが当たったときに自動で呼ばれる
        void Hit(const SphereParam* sp)override;

        //キャラクタに当たったときに呼ばれる
        void HitCharacter(CharacterBase* pHitCharacter)override;

    private:
        AmefootAttackClass* m_pOwner;
    };
    friend DamageHitCallback;


    AmefootAttackClass(
        AmefootPlayer* const pAmefootPlayer,
        Control* pControl
        );


    ~AmefootAttackClass();

    // 更新
    void Update();


private:
    enum StateType
    {
        _Error,
        _Pose,
        _Tackle,
        _DriveAway,
        _Jump,
        _Touchdown,
        _Standup,
        _TackleFailed,
        _End,
        _HitStop,
    };

    typedef void(AmefootAttackClass::*StateFunc)();

    // ステート切り替え
    void SetState(StateType type);


    // ステート更新
    void StateUpdate();

    void Pose(); // 構え中
    void Tackle(); // タックル中
    void DriveAway(); // タックル打ち上げ中
    void Jump(); // 飛び上がり中
    void Touchdown(); // タッチダウン中
    void Standup(); // 立ち上がり中
    void Failed(); // 失敗
    void End(); // 終了
    void HitStop(); // ヒットストップ

    void PoseStart(); // 構え開始の瞬間
    void TackleStart(); // タックル開始の瞬間
    void JumpStart(); // ジャンプ開始の瞬間
    void DrivaAwayStart(); // タックル打ち上げ開始の瞬間
    void TouchdownStart(); // タッチダウン開始の瞬間
    void StandupStart(); // 立ち上がり開始の瞬間
    void FailedStart(); // 失敗開始の瞬間

    // キャラクターの基本的な更新
    void CharacterUsualUpdate();

    // 無敵中の更新
    void CharacterNoDamageUpdate();

    // ヒットストップ時の更新
    void CharacterHitStopUpdate();

private:
    AmefootPlayer* const m_pAmefootPlayer;
    Control* m_pControl;
    DamageShpere m_Damage;

    int m_Timer = 0;
    int m_HitStopFrame = 0;
    //float m_TacklePower = 0.0f;

    StateType m_NowStateType = _Pose;
    StateType m_NextStateType = _Error;
    StateFunc m_pNowState;

};


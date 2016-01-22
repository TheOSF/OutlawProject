#pragma once

#include "CharacterBase.h"
#include "../Damage/Damage.h"
#include "../Ball/Ball.h"

//-------------------------------------------------------//
//  　　　キャラクタの通常カウンタークラス
//-------------------------------------------------------//

class CharacterDefaultCounter
{
public:

    //カウンター中のコールバック関数クラス
    class Event
    {
    public:
        virtual ~Event(){}

        virtual void Pose() = 0;      //構え開始(ステートの開始)
        virtual void Move(BallBase* pCounterBall) = 0;      //カウンターするボールに向かって移動開始
 
        virtual void Catch(BallBase* pCounterBall) {} // キャッチ時

        virtual void Shot(BallBase* pCounterBall) = 0;      //打ち返し時
        virtual void ModelUpdate(float t){}

        virtual void ShotFaild() = 0; //移動時にボールが打ち返せない状態になった時

        virtual Vector3 ClacLocalOffset(bool Right){ return Vector3Zero; }

        virtual void End() = 0;       //クラス終了
    };

    //カウンターパラメータ
    struct Param
    {
        Param();

        UINT     PoseFrame;       //構えを行うフレーム
        UINT     CanCounterFrame; //カウンター移行が可能なフレーム
        UINT     ShotFrame;       //ボールを検知してからキャッチするまでのフレーム
        UINT     AfterShotFrame;  //うち返した後のフレーム
        UINT     FailedFrame;     //打ち返しに失敗してからクラス終了までのフレーム

        UINT     CatchFrame = 0;     // キャッチしてから投げるまでのフレーム ※ 0なら即打ち返す
        UINT     CatchBoneNumber; // キャッチしたボールをどのボーンにつけるか
                                  
        float    CatchAriaSize;   //カウンター検知範囲
        float    ControllRadian;  //操作できる打ち返し角度 (PI/2を超えると見た目的におかしくなるので注意)
        float    BallSpeed;       //打ち返したときのボールの速度(0.8fあたりがお勧め)
    };

    
    CharacterDefaultCounter(
        CharacterBase*               pOwner,       //オーナーキャラクタ
        const Param&                 param,        //カウンターパラメタ
        Event*                       pEventClass,  //イベントクラスへのポインタ(中でdeleteする)
        DamageManager::HitEventBase* pHitEventClass//ヒットイベント(中でdeleteする)
        );

    ~CharacterDefaultCounter();

    //毎フレームの更新
    void Update();

    //スティック値のセット(打ち返し時の方向を決める)
    void SetStickValue(CrVector2 stick);

private:

    //ボールのダメージをカットするクラス(デコレートパターン？)
    class HitEventClass_NoBallDamageFileter :public DamageManager::HitEventBase
    {
        DamageManager::HitEventBase*const m_pInHitEvent;
        BallBase*                         m_pCounterBall;
        CharacterBase*                    m_pOwner;
    public:
        HitEventClass_NoBallDamageFileter(
            DamageManager::HitEventBase* pInHitEvent,
            BallBase*                    pCounterBall,
            CharacterBase*               pOwner
            );
        
        bool Hit(DamageBase* pDmg)override;
    };

    void Pose();  //構え中
    void Move();  //移動中
    void HitStop(); //ヒットストップ
    void Catch(); // キャッチ中
    void Shot();  //打ち返し中
    void Failed();//打ち返し失敗
    void End();   //終了

    //ステートセット
    void SetState(void(CharacterDefaultCounter::*pNewState)());

    //スティックによる角度調整
    void SetStickAngle(CrVector3 OriginVec,RADIAN controllRad);

    void EffectApper();
        

    void SetAutoCounter();

    static const float                  m_SkillUpValue; //スキルゲージが上がる量
    CharacterBase* const                m_pOwner;       //オーナーキャラクタ
    Param  const                        m_Param;        //カウンターパラメタ
    Event* const                        m_pEventClass;  //イベントクラスへのポインタ(中でdeleteする)
    DamageManager::HitEventBase* const  m_pHitEventClass;//ヒットイベント(中でdeleteする)
    UINT                                m_HitStopFrame;

    void(CharacterDefaultCounter::*m_pNowState)();       //ステート関数
    UINT        m_Count;
    BallBase*   m_pCounterBall;
    Vector3     m_MoveTargetPos;
    Vector3     m_BallFreezePos;
    Vector2     m_Stick;
    float       m_MoveValue;
    BallBase::BallID m_counterBallID;
};


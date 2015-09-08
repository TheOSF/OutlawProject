#pragma once

#include "CharacterBase.h"
#include "../Damage/Damage.h"

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
        virtual void Move() = 0;      //カウンターするボールに向かって移動開始
 
        virtual void Shot() = 0;      //打ち返し時
        virtual void ShotFaild() = 0; //移動時にボールが打ち返せない状態になった時

        virtual void End() = 0;       //クラス終了
    };

    //カウンターパラメータ
    struct Param
    {
        UINT     PoseFrame;       //構えを行うフレーム
        UINT     CanCounterFrame; //カウンター移行が可能なフレーム
        UINT     ShotFrame;       //ボールを検知してから打つまでのフレーム
        UINT     AfterShotFrame;  //うち返した後のフレーム
        UINT     FailedFrame;     //打ち返しに失敗してからクラス終了までのフレーム
                                  
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

    void Pose();  //構え中
    void Move();  //移動中
    void Shot();  //打ち返し中
    void Failed();//打ち返し失敗
    void End();   //終了

    //ステートセット
    void SetState(void(CharacterDefaultCounter::*pNewState)());

    //スティックによる角度調整
    void SetStickAngle(CrVector3 OriginVec);

    CharacterBase* const                m_pOwner;       //オーナーキャラクタ
    Param  const                        m_Param;        //カウンターパラメタ
    Event* const                        m_pEventClass;  //イベントクラスへのポインタ(中でdeleteする)
    DamageManager::HitEventBase* const  m_pHitEventClass;//ヒットイベント(中でdeleteする)

    void(CharacterDefaultCounter::*m_pNowState)();       //ステート関数
    UINT        m_Count;
    BallBase*   m_pCounterBall;
    Vector3     m_MoveTargetPos;
    Vector2     m_Stick;
    float       m_MoveValue;
};


#include "CharacterDefaultCounterClass.h"

#include "../Ball/Ball.h"
#include "CharacterFunction.h"

#include "../Effect/HitEffectObject.h"
#include "../Effect/EffectFactory.h"
#include "../Effect/ImpactLightObject.h"

#include "../Sound/Sound.h"

#include "CharacterManager.h"
#include "../Ball/UsualBall.h"
#include "../GameSystem/GameController.h"


CharacterDefaultCounter::CharacterDefaultCounter(
    CharacterBase*               pOwner,        //オーナーキャラクタ
    const Param&                 param,         //カウンターパラメタ
    Event*                       pEventClass,   //イベントクラスへのポインタ(中でdeleteする)
    DamageManager::HitEventBase* pHitEventClass //ヒットイベント(中でdeleteする)
    ) :
    m_pOwner(pOwner),
    m_Param(param),
    m_pEventClass(pEventClass),
    m_pHitEventClass(pHitEventClass),
    m_pNowState(&CharacterDefaultCounter::Pose),
    m_Count(0),
    m_Stick(0, 0),
    m_pCounterBall(nullptr)
{

}

CharacterDefaultCounter::~CharacterDefaultCounter()
{
    delete m_pEventClass;
    delete m_pHitEventClass;
}


//毎フレームの更新
void CharacterDefaultCounter::Update()
{
    (this->*m_pNowState)();
}

//スティック値のセット(打ち返し時の方向を決める)
void CharacterDefaultCounter::SetStickValue(CrVector2 stick)
{
    m_Stick = stick;

    //カメラ基準に変換
    Vector3 temp(m_Stick.x, m_Stick.y, 0);
    
    temp = Vector3MulMatrix3x3(temp, matView);

    m_Stick.x = temp.x;
    m_Stick.y = temp.y;
}


//-----------------------------------------------//
//  カウンター中のダメージ判定クラス
//-----------------------------------------------//

CharacterDefaultCounter::HitEventClass_NoBallDamageFileter::HitEventClass_NoBallDamageFileter(
    DamageManager::HitEventBase* pInHitEvent,
    BallBase*                    pCounterBall
    ):
m_pInHitEvent(pInHitEvent),
m_pCounterBall(pCounterBall)
{

}

bool CharacterDefaultCounter::HitEventClass_NoBallDamageFileter::Hit(DamageBase* pDmg)
{
    //カウンターする予定のボールだった場合は処理せずfalseを返す
    if (pDmg->pBall != nullptr &&
        BallBase::isCanCounter(pDmg->pBall) &&
        m_pCounterBall == pDmg->pBall
        )
    {
        return false;
    }

    //通常のダメージ処理
    return m_pInHitEvent->Hit(pDmg);
}

//-----------------------------------------------//
//  ステート関数
//-----------------------------------------------// 

//構え中
void CharacterDefaultCounter::Pose()  
{
    //カウンタ更新
    ++m_Count;
    
    if (m_Count == 1)
    {
        //エフェクト
        {
            const Vector3 pos = m_pOwner->m_Params.pos + Vector3(0, 3.5f, 0);
            EffectFactory::Counter(pos, 6.5f);

            new ImpactLightObject(
                pos,
                15.0f,
                Vector3(1, 1, 1)*0.5f, 
                0.05f
                );
        }

        //サウンド
        Sound::Play(Sound::Counter);

        //イベントクラス通知
        m_pEventClass->Pose();

        //移動量を初期化
        chr_func::XZMoveDown(m_pOwner, 1);
    }

    //カウンター可能なフレームなら
    if (m_Count < m_Param.CanCounterFrame)
    {



        //ボール検知
        if (DefBallMgr.GetCounterBall(
            &m_pCounterBall,
            m_pOwner->m_Params.pos,
            &m_MoveTargetPos,
            m_Param.CatchAriaSize,
            (int)m_Param.ShotFrame))
        {
            //ボールの場所の少し先にずらす(2キャラ分)
            Vector3 V = Vector3Normalize(m_pCounterBall->m_Params.move);
            V.y = 0;
            m_MoveTargetPos += V*m_pOwner->m_Params.size;

            //カウンターできるボールが見つかった場合、ステートを移動ステートに移行する
            SetState(&CharacterDefaultCounter::Move);
        }
    }


    //時間で終了
    if (m_Count > m_Param.PoseFrame)
    {
        SetState(&CharacterDefaultCounter::End);
    }


    //基本的な更新
    if (m_Count < m_Param.CanCounterFrame)
    {
        HitEventClass_NoBallDamageFileter BallDmgFilter(m_pHitEventClass, m_pCounterBall);

        chr_func::UpdateAll(m_pOwner, &BallDmgFilter);
    }
    else
    {
        chr_func::UpdateAll(m_pOwner, m_pHitEventClass);
    }
}


//移動中
void CharacterDefaultCounter::Move()
{

    //カウンタ更新
    ++m_Count;

    //イベントクラス通知
    if (m_Count == 1)
    {
        m_pEventClass->Move(m_pCounterBall);
    }

    //１フレームあたりの移動値を算出
    if (m_Count == 1)
    {
        m_MoveValue = 1.0f / (float)m_Param.ShotFrame;
    }

    //カウンターするボールがカウンターできない状態なら打ち返し失敗ステートへ移行
    if (BallBase::isCanCounter(m_pCounterBall) == false)
    {
        SetState(&CharacterDefaultCounter::Failed);
        return;
    }

    //移動値をセット
    {
        Vector3 m = m_MoveTargetPos - m_pOwner->m_Params.pos;

        m.y = 0;
        m *= m_MoveValue;

        m_pOwner->m_Params.move = m;
    }


    //アングル
    {   
        if (m_pCounterBall->m_Params.pParent != m_pOwner)
        {
            Vector3 ViewPos = m_pOwner->m_Params.pos + (m_pCounterBall->m_Params.pParent->m_Params.pos - m_MoveTargetPos);
            chr_func::AngleControll(m_pOwner, ViewPos, PI / (float)m_Param.ShotFrame);
        }
        else
        {
            SetStickAngle(-m_pCounterBall->m_Params.move, PI);
        }
    }


    //時間で打ちステートへ移行
    if (m_Count > m_Param.ShotFrame)
    {
        SetState(&CharacterDefaultCounter::Shot);
    }

    ////すでにボールとの距離が一定以下なら打ちステートへ移行
    //if (Vector3Distance(m_pCounterBall->m_Params.pos, m_pOwner->m_Params.pos + Vector3(0, BallBase::UsualBallShotY, 0)) < 1.0f) 
    //{
    //    SetState(&CharacterDefaultCounter::Shot);
    //    return;
    //}

    //基本的な更新
    {
        HitEventClass_NoBallDamageFileter BallDmgFilter(m_pHitEventClass, m_pCounterBall);

        chr_func::UpdateAll(m_pOwner, &BallDmgFilter);
    }
}

//打ち返し失敗
void CharacterDefaultCounter::Failed()
{
    //カウンタ更新
    ++m_Count;

    //イベントクラス通知
    if (m_Count == 1)
    {
        m_pEventClass->ShotFaild();
    }

    //時間で終了ステートへ
    if (m_Count > m_Param.FailedFrame)
    {
        SetState(&CharacterDefaultCounter::End);
        return;
    }

    //基本的な更新
    {
        chr_func::XZMoveDown(m_pOwner,0.1f);
        chr_func::UpdateAll(m_pOwner, m_pHitEventClass);
    }
}


//打ち返し中
void CharacterDefaultCounter::Shot()
{
    //カウンタ更新
    ++m_Count;

    //イベントクラス通知
    if (m_Count == 1)
    {
        m_pEventClass->Shot(m_pCounterBall);
    }

    //ボール打ち返すぜ！
    if (m_Count == 1)
    {

        //スティックによる角度調整
        if (m_pCounterBall->m_Params.pParent == m_pOwner)
        {
            Vector3 vec;
            chr_func::GetFront(m_pOwner, &vec);
            SetStickAngle(vec, m_Param.ControllRadian);
        }
        else
        {
            //自動的に親の元に返す
            SetAutoCounter();
        }

        {
            float m = m_pCounterBall->m_Params.move.Length()*1.1f;
            m = min(m, 2.5f);

            //ボールの設定
            chr_func::GetFront(m_pOwner, &m_pCounterBall->m_Params.move);

            m_pCounterBall->m_Params.move *= m;
        }


        {
            m_pCounterBall->m_Params.pos = m_pOwner->m_Params.pos;
            m_pCounterBall->m_Params.pos.y = UsualBall::UsualBallShotY;
        }

        //ボール側のカウンター処理
        m_pCounterBall->Counter(m_pOwner);

        COLORf EffectColor(CharacterBase::GetPlayerColor(m_pCounterBall->m_Params.pParent->m_PlayerInfo.number));
        
        //エフェクトの設定
        new HitEffectObject(
            m_pCounterBall->m_Params.pos,
            Vector3Normalize(m_pCounterBall->m_Params.move),
            0.1f,
            0.1f,
            Vector3(1, 1, 1)
            );

        //カウンター音再生
        Sound::Play(Sound::AtkHit2);

        //コントローラを振動
        controller::SetVibration(
            10000,
            0.10f,
            m_pOwner->m_PlayerInfo.number
            );

        //若干前に進む
        chr_func::AddMoveFront(m_pOwner, 0.2f, 1.0f);
    }


    //時間で終了ステートへ
    if (m_Count > m_Param.AfterShotFrame)
    {
        SetState(&CharacterDefaultCounter::End);
        return;
    }

    //基本的な更新
    {
        HitEventClass_NoBallDamageFileter BallDmgFilter(m_pHitEventClass, m_pCounterBall);

        DamageManager::HitEventBase* pHitEvent = &BallDmgFilter;

        if (m_Count > 1)
        {
            pHitEvent = m_pHitEventClass;
        }

        //打ち終わっているのでスピードダウン
    //    chr_func::XZMoveDown(m_pOwner, 0.1f);

        chr_func::UpdateAll(m_pOwner, &BallDmgFilter);
    }

}


//終了
void CharacterDefaultCounter::End()
{
    
    if (m_Count == 0)
    {
        //一回しか実行しないように
        ++m_Count; 

        //終了通知をイベントクラスに送る
        m_pEventClass->End();
    }

    //基本的な更新
    {
        chr_func::UpdateAll(m_pOwner, m_pHitEventClass);
    }
}





//ステートセット
void CharacterDefaultCounter::SetState(void(CharacterDefaultCounter::*pNewState)())
{
    m_pNowState = pNewState;
    m_Count = 0;
}


////スティックによる角度調整
//void CharacterDefaultCounter::SetStickAngle(CrVector3 OriginVec, RADIAN controllRad)
//{
//    if (m_Stick.x == 0 && m_Stick.y == 0)
//    {
//        Vector3 ViewPos = m_pOwner->m_Params.pos + OriginVec;
//        ViewPos.y = m_pOwner->m_Params.pos.y;
//
//        chr_func::AngleControll(m_pOwner, ViewPos);
//        return;
//    }
//
//    Vector3 RotateVec;
//
//    //回転する
//    float toStickAngle = Vector2Dot(m_Stick, Vector2(OriginVec.x, OriginVec.z));
//
//    m_Stick.x = -m_Stick.x;
//
//    toStickAngle /= Vector2Length(m_Stick)*Vector2Length(Vector2(OriginVec.x, OriginVec.z));
//    toStickAngle = acosf(toStickAngle);
//
//    //角度制限
//    toStickAngle = min(toStickAngle, controllRad);
//
//    //左右判定
//    float CrossY = m_Stick.y*OriginVec.x + m_Stick.x*OriginVec.z;
//    if (CrossY > 0)
//    {
//        toStickAngle = -toStickAngle;
//    }
//
//    Matrix m;
//    D3DXMatrixRotationY(&m, toStickAngle);
//
//    RotateVec = Vector3MulMatrix3x3(OriginVec, m);
//
//
//    RotateVec = m_pOwner->m_Params.pos + RotateVec;
//    RotateVec.y = m_pOwner->m_Params.pos.y;
//
//    chr_func::AngleControll(m_pOwner, RotateVec);
//}



//スティックによる角度調整
void CharacterDefaultCounter::SetStickAngle(CrVector3 OriginVec, RADIAN controllRad)
{

    Vector3 RotateVec;

    //スティックの倒した方向のベクトルを作成
    //if (m_Stick.x != 0 || m_Stick.y != 0)
    if (0)
    {
        //回転する
        float toStickAngle = Vector2Dot(m_Stick, Vector2(OriginVec.x, OriginVec.z));

        m_Stick.x = -m_Stick.x;

        toStickAngle /= Vector2Length(m_Stick)*Vector2Length(Vector2(OriginVec.x, OriginVec.z));
        toStickAngle = acosf(toStickAngle);

        //角度制限
        toStickAngle = min(toStickAngle, controllRad);

        //左右判定
        float CrossY = m_Stick.y*OriginVec.x + m_Stick.x*OriginVec.z;
        if (CrossY > 0)
        {
            toStickAngle = -toStickAngle;
        }

        Matrix m;
        D3DXMatrixRotationY(&m, toStickAngle);

        RotateVec = Vector3MulMatrix3x3(OriginVec, m);
        RotateVec.Normalize();
    }
    else
    {
        RotateVec = OriginVec;
    }


    //各的キャラクタへのベクトルの中で一番　↑のベクトルと近いものを算出
    {

        RADIAN  most_min_angle = PI, temp_angle;
        Vector3 v, answer;

        const CharacterManager::CharacterMap& ChrMap = DefCharacterMgr.GetCharacterMap();

        answer = RotateVec;


        for (auto& it : ChrMap)
        {
            if (it.first == m_pOwner ||
                chr_func::isDie(it.first))
            {
                continue;
            }

            v = it.first->m_Params.pos - m_pOwner->m_Params.pos;
            v.y = 0;

 
            temp_angle = Vector3Radian(v, RotateVec);

            if (temp_angle < most_min_angle)
            {
                most_min_angle = temp_angle;
                answer = v;
            }
        }

        RotateVec = answer;
    }

    //回転
    {
        chr_func::AngleControll(m_pOwner, m_pOwner->m_Params.pos + RotateVec);
    }
}



void CharacterDefaultCounter::SetAutoCounter()
{
    chr_func::AngleControll(m_pOwner, m_pCounterBall->m_Params.pParent->m_Params.pos);
}
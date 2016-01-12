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


const float CharacterDefaultCounter::m_SkillUpValue = 0.05f; //スキルゲージが上がる量


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
    m_pCounterBall(nullptr),
     m_counterBallID(BallBase::BallID::ErrorID),
     m_HitStopFrame(0),
     m_BallFreezePos(Vector3Zero)
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

    if (m_pNowState != &CharacterDefaultCounter::HitStop)
    {
        m_pEventClass->ModelUpdate(1);
    }
    else{
        m_pEventClass->ModelUpdate(0.2f);
    }
}

//スティック値のセット(打ち返し時の方向を決める)
void CharacterDefaultCounter::SetStickValue(CrVector2 stick)
{
    m_Stick = stick;
}


//-----------------------------------------------//
//  カウンター中のダメージ判定クラス
//-----------------------------------------------//

CharacterDefaultCounter::HitEventClass_NoBallDamageFileter::HitEventClass_NoBallDamageFileter(
    DamageManager::HitEventBase* pInHitEvent,
    BallBase*                    pCounterBall,
    CharacterBase*               pOwner
    ):
m_pInHitEvent(pInHitEvent),
m_pCounterBall(pCounterBall),
m_pOwner(pOwner)
{

}

bool CharacterDefaultCounter::HitEventClass_NoBallDamageFileter::Hit(DamageBase* pDmg)
{
    //カウンターする予定のボールだった場合は処理せずfalseを返す
    if (pDmg->pBall != nullptr &&
        BallBase::isCanCounter(pDmg->pBall, m_pOwner) &&
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

            /*new ImpactLightObject(
                pos,
                15.0f,
                Vector3(1, 1, 1)*0.5f, 
                0.05f
                );
                */
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
            (int)m_Param.ShotFrame,
            m_pOwner))
        {
            //ボールの場所の少し先にずらす(2キャラ分)
            Vector3 V = Vector3Normalize(m_pCounterBall->m_Params.move);
            V.y = 0;
            m_MoveTargetPos += V*m_pOwner->m_Params.size;

            {
                Matrix R;
                Vector3 V;

                V = m_pCounterBall->m_Params.pParent->m_Params.pos - m_MoveTargetPos;

                RADIAN angle = acosf(fClamp(V.z, 1, -1));

                if (angle < 0)
                {
                    angle = -angle;
                }

                D3DXMatrixRotationY(&R, angle);

                m_MoveTargetPos -= Vector3MulMatrix(m_pEventClass->ClacLocalOffset(
                    chr_func::isRight(m_MoveTargetPos, m_MoveTargetPos - m_pCounterBall->m_Params.pParent->m_Params.pos, m_pOwner->m_Params.pos)
                    ), R);

            }

            m_counterBallID = m_pCounterBall->GetID();

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
        HitEventClass_NoBallDamageFileter BallDmgFilter(m_pHitEventClass, m_pCounterBall,m_pOwner);

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

    //カウンターするボールがカウンターできない状態なら打ち返し失敗ステートへ移行
    if (DefBallMgr.isBallEnable(m_counterBallID) == false ||
        BallBase::isCanCounter(m_pCounterBall, m_pOwner) == false
         )
    {
         SetState(&CharacterDefaultCounter::Failed);
         return;
    }

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
        //ヒットストップカウント
        //m_HitStopFrame = (UINT)fClamp(m_pCounterBall->m_Params.move.Length(), 8,8);
        m_HitStopFrame = 0;
        m_BallFreezePos = m_pCounterBall->m_Params.pos;

        SetState(&CharacterDefaultCounter::HitStop);
    }

    ////すでにボールとの距離が一定以下なら打ちステートへ移行
    //if (Vector3Distance(m_pCounterBall->m_Params.pos, m_pOwner->m_Params.pos + Vector3(0, BallBase::UsualBallShotY, 0)) < 1.0f) 
    //{
    //    SetState(&CharacterDefaultCounter::Shot);
    //    return;
    //}

    //基本的な更新
    {
        HitEventClass_NoBallDamageFileter BallDmgFilter(m_pHitEventClass, m_pCounterBall, m_pOwner);

        chr_func::UpdateAll(m_pOwner, &BallDmgFilter);
    }
}


void CharacterDefaultCounter::HitStop() //ヒットストップ
{
    //カウンターするボールがカウンターできない状態なら打ち返し失敗ステートへ移行
    if (DefBallMgr.isBallEnable(m_counterBallID) == false)
    {
        SetState(&CharacterDefaultCounter::Failed);
        return;
    }

    m_pCounterBall->m_Params.pos = m_BallFreezePos;

    ++m_Count;

    //ステート分岐
    if (m_Count >= m_HitStopFrame)
    {
        if (m_Param.CatchFrame <= 0)
        {
            SetState(&CharacterDefaultCounter::Shot);
        }
        else
        {
            SetState(&CharacterDefaultCounter::Catch);
        }
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


// キャッチ中
void CharacterDefaultCounter::Catch()
{
     //カウンタ更新
     ++m_Count;

     //カウンターするボールがカウンターできない状態なら打ち返し失敗ステートへ移行
     if ( DefBallMgr.isBallEnable(m_counterBallID) == false ||
         BallBase::isCanCounter(m_pCounterBall, m_pOwner) == false
          )
     {
          SetState(&CharacterDefaultCounter::Failed);
          return;
     }

     if ( m_Count == 1 )
     {
          // イベントクラス通知
          m_pEventClass->Catch(m_pCounterBall);

          //コントローラを振動
          chr_func::SetControllerShock(
              m_pOwner,
              1,
              0.10f
              );
     }

     // ボールをボーンに引っ付ける
     Vector3 bonePos = m_pOwner->m_Renderer.GetWorldBonePos(m_Param.CatchBoneNumber);
     m_pCounterBall->m_Params.pos = bonePos;

     chr_func::XZMoveDown(m_pOwner, 0.2f);

     // とりあえず移動量はゼロに
     //m_pCounterBall->m_Params.move = Vector3Zero;

     //**************************************************************
     // @TODO  ボールのあたり判定を消さないと持ってるときに当たってしまう！！！
     // とりあえず置いておく
     //**************************************************************

     // 時間で打ち返しステートへ
     if ( m_Count > m_Param.CatchFrame )
     {
          SetState(&CharacterDefaultCounter::Shot);
     }

     //基本的な更新
     {
         HitEventClass_NoBallDamageFileter BallDmgFilter(m_pHitEventClass, m_pCounterBall, m_pOwner);
          chr_func::UpdateAll(m_pOwner, &BallDmgFilter);
     }

}


//打ち返し中
void CharacterDefaultCounter::Shot()
{
    //カウンタ更新
    ++m_Count;


    //ボール打ち返すぜ！
    if (m_Count == 1)
    {
        //カウンターするボールがカウンターできない状態なら打ち返し失敗ステートへ移行
        if (DefBallMgr.isBallEnable(m_counterBallID) == false)
        {
            SetState(&CharacterDefaultCounter::Failed);
            return;
        }

        //ボールの移動値を計算
        float m = m_pCounterBall->m_Params.move.Length()*1.1f;
        m = min(m, 2.5f);

        m_pEventClass->Shot(m_pCounterBall);

        {
            //座標戻し
            Vector3 m = m_pCounterBall->m_Params.move;
            Vector3 v = Vector3Normalize(m)*Vector3Dot(Vector3Normalize(m), m_pOwner->m_Params.pos - m_pCounterBall->m_Params.pos);
        
            v.y = 0;
            m_pCounterBall->m_Params.pos += v;
            m_pCounterBall->m_Params.pos.y = UsualBall::UsualBallShotY;
        }

        if (m_Param.CatchFrame > 0)
        {
            m_pCounterBall->m_Params.pos += Vector3Normalize(m_pCounterBall->m_Params.move)*1.0f;
        }

        EffectApper();

        //スティックによる角度調整
        if (m_pCounterBall->m_Params.pParent == m_pOwner)
        {
            Vector3 vec;
            chr_func::GetFront(m_pOwner, &vec);
            SetStickAngle(vec, m_Param.ControllRadian);

            //ボールの設定
            chr_func::GetFront(m_pOwner, &m_pCounterBall->m_Params.move);
        }
        else
        {
            //自動的に親の元に返す
            SetAutoCounter();
        }

        {
            m_pCounterBall->m_Params.move *= m;
        }

        //ゲージ上昇
        {
            chr_func::AddSkillGauge(m_pOwner, m_SkillUpValue);
        }

        //振動
        chr_func::SetControllerShock(m_pOwner, 1.0f, 0.1f);

        //ボール側のカウンター処理
        m_pCounterBall->Counter(m_pOwner);


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
        HitEventClass_NoBallDamageFileter BallDmgFilter(m_pHitEventClass, m_pCounterBall, m_pOwner);

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


//スティックによる角度調整
void CharacterDefaultCounter::SetStickAngle(CrVector3 OriginVec, RADIAN controllRad)
{

    Vector3 RotateVec = OriginVec;


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

void CharacterDefaultCounter::EffectApper()
{

    //エフェクトの設定
    new HitEffectObject(
        m_pCounterBall->m_Params.pos,
        -Vector3Normalize(m_pCounterBall->m_Params.move),
        0.1f,
        0.1f,
        Vector3(1, 1, 1)
        );

    //カウンター音再生
    Sound::Play(Sound::AtkHit2);

    //コントローラを振動

    chr_func::SetControllerShock(
        m_pOwner,
        1,
        0.10f
        );
}

void CharacterDefaultCounter::SetAutoCounter()
{
    Vector3 target = m_pCounterBall->m_Params.pParent->m_Params.pos;
    chr_func::AngleControll(m_pOwner, target);
    m_pCounterBall->m_Params.move = target - m_pCounterBall->m_Params.pos;
    m_pCounterBall->m_Params.move.y = 0;

    m_pCounterBall->m_Params.move.Normalize();
}
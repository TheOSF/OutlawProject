#include "CharacterDefaultCounterClass.h"

#include "../Ball/Ball.h"
#include "CharacterFunction.h"

#include "../Effect/HitEffectObject.h"


CharacterDefaultCounter::CharacterDefaultCounter(
    CharacterBase*               pOwner,       //オーナーキャラクタ
    const Param&                 param,        //カウンターパラメタ
    Event*                       pEventClass,  //イベントクラスへのポインタ(中でdeleteする)
    DamageManager::HitEventBase* pHitEventClass//ヒットイベント(中でdeleteする)
    ):
    m_pOwner(pOwner),
    m_Param(param),
    m_pEventClass(pEventClass),
    m_pHitEventClass(pHitEventClass),
    m_pNowState(&CharacterDefaultCounter::Pose),
    m_Count(0),
    m_Stick(0,0)
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
//  ステート関数
//-----------------------------------------------// 

//構え中
void CharacterDefaultCounter::Pose()  
{
    //カウンタ更新
    ++m_Count;

    //イベントクラス通知
    if (m_Count == 1)
    {
        m_pEventClass->Pose();

        {
            chr_func::XZMoveDown(m_pOwner, 1);
        }
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
            //ボールの場所の少し先にずらす(１キャラ分)
            Vector3 V = Vector3Normalize(m_pCounterBall->m_Params.move);
            V.y = 0;
            m_MoveTargetPos += V*m_pOwner->m_Params.size * 2;

            //カウンターできるボールが見つかった場合、ステートを移動ステートに移行する
            SetState(&CharacterDefaultCounter::Move);

            return;
        }
    }


    //時間で終了
    if (m_Count > m_Param.PoseFrame)
    {
        SetState(&CharacterDefaultCounter::End);
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
        m_pEventClass->Move();
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

    //移動
    {
        Vector3 m = m_MoveTargetPos - m_pOwner->m_Params.pos;

        m.y = 0;
        //m.Normalize();
        m *= m_MoveValue;

        m_pOwner->m_Params.pos += m;
    }


    //アングル
    {
        Vector3 ViewPos = m_pOwner->m_Params.pos + (m_pCounterBall->m_Params.pParent->m_Params.pos - m_MoveTargetPos);
        
        ViewPos.y = m_pOwner->m_Params.pos.y;

        chr_func::AngleControll(m_pOwner, ViewPos, PI / (float)m_Param.ShotFrame);
    }


    //時間で打ちステートへ移行
    if (m_Count > m_Param.ShotFrame)
    {
        SetState(&CharacterDefaultCounter::Shot);
        return;
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
}


//打ち返し中
void CharacterDefaultCounter::Shot()
{
    //カウンタ更新
    ++m_Count;

    //イベントクラス通知
    if (m_Count == 1)
    {
        m_pEventClass->Shot();
    }

    //ボール打ち返すぜ！
    if (m_Count == 1)
    {
        //スティックによる角度調整
        SetStickAngle(m_pCounterBall->m_Params.pParent->m_Params.pos - m_pOwner->m_Params.pos);

        //ボールの設定
        chr_func::GetFront(m_pOwner, &m_pCounterBall->m_Params.move);
        
        m_pCounterBall->m_Params.move *= m_Param.BallSpeed;
        m_pCounterBall->m_Params.pParent = m_pOwner;

        //ボール側のカウンター処理
        m_pCounterBall->Counter();

        COLORf EffectColor(CharacterBase::GetPlayerColor(m_pCounterBall->m_Params.pParent->m_PlayerInfo.number));

        //エフェクトの設定
        new HitEffectObject(
            m_pCounterBall->m_Params.pos,
            Vector3Normalize(m_pCounterBall->m_Params.move),
            0.1f,
            0.1f,
            Vector3(EffectColor.r, EffectColor.g, EffectColor.b)
            );

    }

    //時間で終了ステートへ
    if (m_Count > m_Param.AfterShotFrame)
    {
        SetState(&CharacterDefaultCounter::End);
        return;
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
}





//ステートセット
void CharacterDefaultCounter::SetState(void(CharacterDefaultCounter::*pNewState)())
{
    m_pNowState = pNewState;
    m_Count = 0;
}


//スティックによる角度調整
void CharacterDefaultCounter::SetStickAngle(CrVector3 OriginVec)
{
    if (m_Stick.x == 0 && m_Stick.y == 0)
    {
        Vector3 ViewPos = m_pOwner->m_Params.pos + OriginVec;
        ViewPos.y = m_pOwner->m_Params.pos.y;

        chr_func::AngleControll(m_pOwner, ViewPos);
        return;
    }

    Vector3 RotateVec;

    //回転する
    float toStickAngle = Vector2Dot(m_Stick, Vector2(OriginVec.x, OriginVec.z));

    m_Stick.x = -m_Stick.x;

    toStickAngle /= Vector2Length(m_Stick)*Vector2Length(Vector2(OriginVec.x, OriginVec.z));
    toStickAngle = acosf(toStickAngle);

    //角度制限
    toStickAngle = min(toStickAngle, m_Param.ControllRadian);

    //左右判定
    float CrossY = m_Stick.y*OriginVec.x + m_Stick.x*OriginVec.z;
    if (CrossY > 0)
    {
        toStickAngle = -toStickAngle;
    }

    Matrix m;
    D3DXMatrixRotationY(&m, toStickAngle);

    RotateVec = Vector3MulMatrix3x3(OriginVec, m);


    RotateVec = m_pOwner->m_Params.pos + RotateVec;
    RotateVec.y = m_pOwner->m_Params.pos.y;

    chr_func::AngleControll(m_pOwner, RotateVec);
}
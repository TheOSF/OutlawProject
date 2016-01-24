#include "CharacterComputerMove.h"
#include "../CharacterManager.h"
#include "../CharacterFunction.h"
#include "../../Collision/Collision.h"
#include "../../debug/DebugDraw.h"

////////////////////////////ベスト距離をランダム入れてない

CharacterComputerMove::InParam::InParam(const InParam& p)
{
    m_BestLenList = p.m_BestLenList;
}

//------------------------------------------------------------//
//            コンピュータ用の移動クラス
//------------------------------------------------------------//


CharacterComputerMove::CharacterComputerMove(CharacterBase* p, const InParam& param) :
m_pChr(p),
m_InParam(param),
m_MoveVec(0, 0),
m_StateTimer(0),
m_pTargetChr(nullptr),
m_MoveTargetPos(Vector3Zero),
m_StopFrame(0),
m_pStateFunc(&CharacterComputerMove::State_TargetSelect),
m_ViewTargetChr(false),
m_SelectBestLen(0),
m_MovePosRotate(0)
{
    MyAssert(m_InParam.m_BestLenList.size() > 0, "キャラのベスト距離が設定されていません");
}

CharacterComputerMove::~CharacterComputerMove()
{

}


void CharacterComputerMove::Update()
{
    //フレームカウント
    m_StateTimer = min(m_StateTimer + 1, 10000);

    //ステート関数を実行
    (this->*m_pStateFunc)();

#ifdef _DEBUG
    //デバッグ
    new DebugDrawSphere(m_MoveTargetPos + Vector3(0, 2, 0), 0.5f, COLORf(1, 0, 0, 1));
#endif
}

Vector2 CharacterComputerMove::GetMoveVec()
{
    return m_MoveVec;
}

//このクラスが現在ターゲットにしているキャラクタを得る
bool CharacterComputerMove::GetTargetCharacter(CharacterBase** ppOutTargetChr)const
{
    *ppOutTargetChr = m_pTargetChr;

    return *ppOutTargetChr != nullptr;
}

//ステートセット
void CharacterComputerMove::SetNewState(StateFunc state)
{
    m_pStateFunc = state;
    m_StateTimer = 0;
}

CharacterComputerMove::StrongParam CharacterComputerMove::GetStrongParam()const
{
    StrongParam ret;

    switch (m_pChr->m_PlayerInfo.strong_type)
    {
    case StrongType::_Weak:
        ret.UpdateTargetChrRatio = 0.01f;
        ret.UpdateMovePosRatio = 0.02f;
        ret.StopRatio = 0.002f;
        ret.RandBestLen = 10.0f;
        break;

    case StrongType::_Usual:
        ret.UpdateTargetChrRatio = 0.02f;
        ret.UpdateMovePosRatio = 0.04f;
        ret.StopRatio = 0.001f;
        ret.RandBestLen = 5.0f;
        break;

    case StrongType::_Strong:
        ret.UpdateTargetChrRatio = 0.1f;
        ret.UpdateMovePosRatio = 0.1f;
        ret.StopRatio = 0.00f;
        ret.RandBestLen = 1.0f;
        break;

    default:
        MyAssert(false, "対応できない難易度タイプです");
        break;
    }

    return ret;
}

//ターゲットキャラクタを更新するフレームかどうか
bool CharacterComputerMove::isUpdateTargetFrame()const
{
    return frand() <= GetStrongParam().UpdateTargetChrRatio;
}

//移動目標位置を更新するフレームかどうか
bool CharacterComputerMove::isUpdatePositionFrame()const
{
    return frand() <= GetStrongParam().UpdateMovePosRatio && m_StateTimer > 30;
}

//とまるフレームかどうか
bool CharacterComputerMove::isStopStartFrame(int MoveingFrame)const
{
    return frand() <= GetStrongParam().UpdateMovePosRatio;
}

//移動目標の地点と近いかどうか
bool CharacterComputerMove::isNearTargetPos(float OkLen)const
{
    return Vector3Distance(m_pChr->m_Params.pos, m_MoveTargetPos) < OkLen;
}

//ベスト距離にいるキャラクタを得る
bool CharacterComputerMove::GetAttackTargetChr(CharacterBase** ppOut)
{
    const CharacterManager::CharacterMap& ChrMap = DefCharacterMgr.GetCharacterMap();
    const float OKlen = 2.5f;

    float MostBestNearLen = OKlen;  //もっとも近いキャラクタとベスト距離との距離を保存する
    float TempLen = 0.0f;           //調べるキャラクタと親キャラクタへの距離を保存
    float TempBestLen = 0.0f;       //計算用に

    //初期化
    *ppOut = nullptr;

    for (auto &ChrMapIterator : ChrMap)
    {
        CharacterBase* const it = ChrMapIterator.first;

        //自身と死んでいるキャラを除外
        if (it == m_pChr || chr_func::isDie(it))
        {
            continue;
        }

        //調べるキャラクタと親キャラクタへの距離を保存
        TempLen = Vector3Distance(m_pChr->m_Params.pos, it->m_Params.pos);

        //ベスト距離との長さの差を測る
        for (auto& BestLenIterator : m_InParam.m_BestLenList)
        {
            TempBestLen = BestLenIterator.Len - TempLen;
            TempBestLen = fabsf(TempBestLen);

            //距離が近かったら更新
            if (TempBestLen < MostBestNearLen)
            {
                MostBestNearLen = TempBestLen;
                *ppOut = it;
            }
        }
    }

    return *ppOut != nullptr && 0;
}

bool CharacterComputerMove::GetMostHighHpChr(CharacterBase** ppOut)
{
    const CharacterManager::CharacterMap& ChrMap = DefCharacterMgr.GetCharacterMap();
    RATIO MostHighHpRatio = 0.0f;
    RATIO TempRatio = 0.0f;

    //初期化
    *ppOut = nullptr;

    for (auto &ChrMapIterator : ChrMap)
    {
        CharacterBase* const it = ChrMapIterator.first;

        //自身と死んでいるキャラを除外
        if (it == m_pChr || chr_func::isDie(it))
        {
            continue;
        }

        //ＨＰの残ってる率を取得
        TempRatio = chr_func::GetLifeRatio(it);

        //多かったら更新
        if (MostHighHpRatio < TempRatio)
        {
            MostHighHpRatio = TempRatio;
            *ppOut = it;
        }
    }

    return *ppOut != nullptr;
}

//ターゲットを得る
bool CharacterComputerMove::GetTargetChr(CharacterBase** ppOut)
{
    //基本的にHPが高いキャラクタを狙うが、攻撃に適したキャラクタがいる場合はそのキャラにする

    ////攻撃に適したキャラクタを取得
    //if (GetAttackTargetChr(ppOut))
    //{
    //    //取得できた！！
    //    return true;
    //}

    //ＨＰの多いキャラクタを取得
    if (GetMostHighHpChr(ppOut))
    {
        //取得できた！！
        return true;
    }
    
    *ppOut = nullptr;

    return false;
}

float CharacterComputerMove::GetBestLen()
{
    RATIO value = frand();

    //ベスト距離との長さの差を測る
    for (auto& BestLenIterator : m_InParam.m_BestLenList)
    {
        if (value < BestLenIterator.Ratio)
        {
            return BestLenIterator.Len;
        }

        value -= BestLenIterator.Ratio;
    }


    MyAssert(false, "確立がおかしい？");

    return 0.0f;
}

//目標地点をみてるか
bool CharacterComputerMove::isViewTargetPos()const
{
    const RADIAN OKangle = D3DXToRadian(20);

    return Vector3Radian(chr_func::GetFront(m_pChr), m_pTargetChr->m_Params.pos - m_pChr->m_Params.pos) < OKangle;
}

//移動目標を得る
Vector3 CharacterComputerMove::GetMovePosition()
{
    Vector3 ret = Vector3Zero;

    //距離から移動すべき座標を算出する
    {
        Vector3 Vec = m_pTargetChr->m_Params.pos;

        Vec.y = 0.0f;
        Vec.Normalize();
        Vec *= -m_SelectBestLen;

        //回転
        {
            Vec =  Vector3RotateAxis(Vector3AxisY, m_MovePosRotate, Vec);
        }

        ret = m_pTargetChr->m_Params.pos + Vec;
    }

    return ret;
}

//とまっているフレームを取得
int CharacterComputerMove::GetStopFrame()const
{
    const int Min = 20;
    const int Max = 50;
    
    //乱数オンリー！(それっぽくなる)
    return Min + (rand() % (Max - Min));
}

//ターゲット選考
void CharacterComputerMove::State_TargetSelect()  
{
    //ターゲットを取得
    if (GetTargetChr(&m_pTargetChr))
    {
        //取得できた場合

        //保持する距離を取得
        m_SelectBestLen = GetBestLen();

        //移動目標を計算
        m_MoveTargetPos = GetMovePosition();

        //適度に回転させる
        m_MovePosRotate = (frand()*2.0f - 1.0f) * D3DXToRadian(90);

        //動きステートへ
        SetNewState(&CharacterComputerMove::State_Move);
    }
    else
    {
        //移動目標が無かった！？
        m_MoveVec = Vector2(0, 0);
    }
}

//目標に向かって移動
void CharacterComputerMove::State_Move()
{

    //もし座標更新フレームなら更新
    if (isUpdatePositionFrame())
    {
        m_MoveTargetPos = GetMovePosition();
    }

    //移動ベクトルを作成
    {
        //移動目標と近かった場合 or 一定時間経過
        if (isNearTargetPos(4.0f) || m_StateTimer > 150)
        {
            //ステート移行
            SetNewState(&CharacterComputerMove::State_NearTargetPos);
        }
        else
        {
            //目標にむかって移動する
            m_MoveVec.x = m_MoveTargetPos.x - m_pChr->m_Params.pos.x;
            m_MoveVec.y = m_MoveTargetPos.z - m_pChr->m_Params.pos.z;
        }
    }

    //一応正規化
    m_MoveVec = Vector2Normalize(m_MoveVec);

}

//微妙な更新はするが現状を維持する
void CharacterComputerMove::State_NearTargetPos()
{

    //すでに目標を向いていてた場合何もしない
    if (m_ViewTargetChr)
    {
        m_MoveVec = Vector2(0, 0);
    }
    else
    {
        //目標のほうを向く(ちょっと目標にむけて移動する
        Vector3 MoveVec = m_pTargetChr->m_Params.pos - m_pChr->m_Params.pos;

        m_MoveVec = Vector2(MoveVec.x, MoveVec.z);

        m_ViewTargetChr = isViewTargetPos();
    }

    //一応正規化
    m_MoveVec = Vector2Normalize(m_MoveVec);

    //時間経過で動きステートへ
    if (m_StateTimer >= 40)
    {
        m_ViewTargetChr = false;

        SetNewState(&CharacterComputerMove::State_TargetSelect);
    }

}

//一定時間立ち止まる
void CharacterComputerMove::State_Stop()
{
    //移動しない！
    m_MoveVec = Vector2(0, 0);

    //フレーム経過で動きステートへ
    if (m_StateTimer >= m_StopFrame)
    {
        SetNewState(&CharacterComputerMove::State_Move);
    }
}
#include "TennisPlayerState_ComMove.h"
#include "../TennisState_Shot.h"
#include "../TennisPlayerState_Counter.h"
#include "../../../GameSystem/GameController.h"
#include "../../CharacterFunction.h"
#include "../../../Ball/UsualBall.h"
#include "../../../Damage/Damage.h"
#include "../../CharacterCounterClass.h"
#include "../../CharacterMoveClass.h"
#include "../../CharacterShotAttackClass.h"
#include "../../CharacterManager.h"
#include "../Tennis_HitEvent.h"
#include "../TennisPlayerState_Attack.h"


//ステート開始
void TennisState_ComputerControll_Move::Enter(TennisPlayer* t)
{
    //移動イベントクラス
    class TennisMoveEvent :public CharacterUsualMove::MoveEvent
    {
        TennisPlayer* m_pTennis;
    public:
        TennisMoveEvent(TennisPlayer* pTennis) :
            m_pTennis(pTennis){}

        //アニメーションの更新
        void Update(bool isRun, RATIO speed_ratio)
        {
            m_pTennis->m_Renderer.Update(1);
        }
        //走り始めにモーションをセット
        void RunStart()
        {
            m_pTennis->m_Renderer.SetMotion(TennisPlayer::_mt_Run);
        }
        //立ちはじめにモーションをセット
        void StandStart()
        {
            m_pTennis->m_Renderer.SetMotion(TennisPlayer::_mt_Stand);
        }
    };

    //移動パラメータを代入
    CharacterUsualMove::Params p;

    p.Acceleration = 0.2f;
    p.MaxSpeed = 0.2f;
    p.TurnSpeed = 0.3f;
    p.DownSpeed = 0.2f;

    //移動クラスの作成
    m_pMoveClass = new CharacterUsualMove(
        t,
        p,
        new TennisMoveEvent(t),
        new TennisHitEvent(t)
        );


    //移動コントロールクラスの作成
    if (t->m_PlayerInfo.strong_type == StrongType::__ErrorType)
    {
        m_pMoveControllClass = new SandBagMoveControllClass();
    }
    else
    {
        m_pMoveControllClass = new ComMoveControllClass(t);
    }
    
}


void TennisState_ComputerControll_Move::Execute(TennisPlayer* t)
{
    //スティック値をセット
    m_pMoveClass->SetStickValue(m_pMoveControllClass->Move(t));

    //更新
    m_pMoveClass->Update();

    //モデルのワールド変換行列を更新
    chr_func::CreateTransMatrix(t, t->m_ModelSize, &t->m_Renderer.m_TransMatrix);

}

void TennisState_ComputerControll_Move::Exit(TennisPlayer* t)
{
    delete m_pMoveClass;
    delete m_pMoveControllClass;
}

//---------------------------------------------------------------------
//                      SandBagMoveControllClass
//---------------------------------------------------------------------

Vector2 SandBagMoveControllClass::Move(TennisPlayer* t)
{
    //スティックの値としてコンピュータ操作の移動を計算する
    const Vector3 Target = DefCharacterMgr.GetRoundStartPos(t->m_PlayerInfo.number);
    Vector3 v = Target - t->m_Params.pos;

    if (v.Length() < 1.0f)
    {
        v = Vector3Zero;
    }

    return Vector2Normalize(Vector2(v.x, v.z));
}

//---------------------------------------------------------------------
//                      ComMoveControllClass
//---------------------------------------------------------------------

ComMoveControllClass::ComMoveControllClass(TennisPlayer* t) :
m_pTennis(t)
{
    GetParams(m_Param, t->m_PlayerInfo.strong_type);
    m_pStateFunc = &ComMoveControllClass::StateStop;
    m_Count = rand() % 20;
}

ComMoveControllClass::~ComMoveControllClass()
{

}

Vector2 ComMoveControllClass::Move(TennisPlayer* t)
{
    //スティックの値としてコンピュータ操作の移動を計算する
    const Vector3 Target = DefCharacterMgr.GetRoundStartPos(t->m_PlayerInfo.number);
    Vector3 v = Target - t->m_Params.pos;

    if (v.Length() < 1.0f)
    {
        v = Vector3Zero;
    }

    return Vector2Normalize(Vector2(v.x, v.z));
}


void ComMoveControllClass::GetParams(Param& out, StrongType::Value st)
{
    switch (st)
    {
        //弱い
    case StrongType::_Weak:
        out.BallCounter = 0.3f;
        out.BallCounterSpeed = 0.5f;
        out.BallCounterTec = 0.1f;
        out.RunStop = 0.8f;
        out.RunPlaceTec = 0.1f;
        out.DangerEscape = 0.2f;
        out.ActionFrequence = 0.3f;
        out.NamePlay = 1.0f;
        break;

        //普通
    case StrongType::_Usual:
        out.BallCounter = 0.6f;
        out.BallCounterSpeed = 0.7f;
        out.BallCounterTec = 0.7f;
        out.RunStop = 0.4f;
        out.RunPlaceTec = 0.5f;
        out.DangerEscape = 0.5f;
        out.ActionFrequence = 0.6f;
        out.NamePlay = 0.5f;
        break;

        //強い
    case StrongType::_Strong:
        out.BallCounter = 1.0f;
        out.BallCounterSpeed = 1.0f;
        out.BallCounterTec = 1.0f;
        out.RunStop = 0.2f;
        out.RunPlaceTec = 1.0f;
        out.DangerEscape = 1.0f;
        out.ActionFrequence = 1.0f;
        out.NamePlay = 0.0f;
        break;

    default:
        MyAssert(false, "認識できないstrongTypeがComMoveControllClass::GetParamsに送られました");
        break;
    }
}


void ComMoveControllClass::StateMove(Vector2& out)
{
    const float GoalOKlen = 2.0f; //ゴールとみなす距離(誤差の対処)

    //目標に到達していたらとまる
    if (Vector3Distance(m_MoveTargetPos, m_pTennis->m_Params.pos) < GoalOKlen)
    {
        m_Count = (int)(m_Param.RunStop * 100.0f);
        m_pStateFunc = &ComMoveControllClass::StateStop;
    }
    

    //目標に到達できない or 新目標があればそこに変更する


    //目標に向かって移動
}



void ComMoveControllClass::StateStop(Vector2& out)
{

}


Vector3 ComMoveControllClass::GetMoveTargetPos()
{
    Vector3 ret;

    //テニス　＝　遠距離が有利なので、
    //全キャラクタからもっとも遠い場所を算出するのがBest

    //弱い場合、近い距離もたまに出しちゃう
    
    struct TargetInfo
    {
        bool      ok;
        Vector3   pos;
    };

    TargetInfo targets[8];

    const CharacterManager::CharacterMap& ChrMap = DefCharacterMgr.GetCharacterMap();

    for (int i = 0; i < (int)ARRAYSIZE(targets); ++i)
    {
        //ランダムに座標を作成
        targets[i].pos = Vector3(frand() - 0.5f, 0, frand() - 0.5f) * 50.f;
        targets[i].pos += Vector3Normalize(targets[i].pos) * 5.0f;


        //その点が良いかどうか
        for (auto& it : ChrMap)
        {
            if (it.first == m_pTennis||
                chr_func::isDie(it.first) )
            {
                continue;
            }


        }
    }

    return Vector3Zero;
}
#include "TennisPlayerState_SlowUpBall.h"
#include "../CharacterFunction.h"
#include "Tennis_HitEvent.h"
#include "../../GameSystem/GameController.h"
#include "../CharacterFunction.h"
#include "../../Ball/UsualBall.h"
#include "../../Damage/Damage.h"
#include "../CharacterCounterClass.h"
#include "../CharacterMoveClass.h"
#include "../CharacterShotAttackClass.h"
#include "../CharacterManager.h"

#include "TennisState_BoundBallAtk.h"
#include "../../Effect/HitEffectObject.h"
#include "../../Sound/Sound.h"
#include "../../GameSystem/GameController.h"
#include "TennisPlayerState_UsualMove.h"



TennisPlayerState_SlowUpBall::TennisPlayerState_SlowUpBall(
    ControllClass*       pControllClass //終了時にdeleteする
    ) :
    m_pTargetEnemy(nullptr),
    m_pUpBall(nullptr),
    m_pControllClass(pControllClass),
    m_Timer(0),
    m_pStateFunc(&TennisPlayerState_SlowUpBall::State_SlowUp)
{

}


TennisPlayerState_SlowUpBall::~TennisPlayerState_SlowUpBall()
{
    delete m_pControllClass;
}


void TennisPlayerState_SlowUpBall::Enter(TennisPlayer* p)
{
    m_pTennis = p;
}

void TennisPlayerState_SlowUpBall::Execute(TennisPlayer* p)
{
    (this->*m_pStateFunc)();

    //基本的な更新
    {
        TennisHitEvent HitEvent(p);
        chr_func::UpdateAll(p, &HitEvent);

        //モデル更新
        p->m_Renderer.Update(1);

        //行列更新
        chr_func::CreateTransMatrix(p, p->m_ModelSize, &p->m_Renderer.m_TransMatrix);

    }
}

void TennisPlayerState_SlowUpBall::Exit(TennisPlayer* p)
{

}


TennisPlayerState_SlowUpBall::ShotType TennisPlayerState_SlowUpBall::GetShotType()const
{
    //撃ち始め
    if (m_Timer < 20)
    {
        return ShotType::CutBall;
    }
    else 
    if (m_Timer < 40) //ボール最頂点
    {
        return ShotType::Smash;
    }

    //それ以外
    return ShotType::CutBall;
}

void TennisPlayerState_SlowUpBall::SetState(void(TennisPlayerState_SlowUpBall::*pStateFunc)())
{
    m_pStateFunc = pStateFunc;
    m_Timer = 0;
}



void TennisPlayerState_SlowUpBall::State_SlowUp()
{
    const int SlowFrame = 5;

    ++m_Timer;

    //徐々にスピードダウン
    if (m_Timer < SlowFrame)
    {
        chr_func::XZMoveDown(m_pTennis, 0.2f);
    }


    if (m_Timer == SlowFrame)
    {
        //移動値を０に
        chr_func::XZMoveDown(m_pTennis, 1.0f);

        //モーション
        m_pTennis->m_Renderer.SetMotion(TennisPlayer::_mt_UpBall);

        //投げ上げボール作成
        BallBase::Params param;

        //移動は上向き
        param.move = m_pTennis->m_Params.move;
        param.move.y = 0.40f;

        //キャラの場所に(最終的に腕の位置に？)
        param.pos = m_pTennis->m_Params.pos + Vector3(0, 2.0f, 0);
        //親を自分に
        param.pParent = m_pTennis;
        //カウンターできないタイプ
        param.type = BallBase::Type::_CantCounter;

        //生成
        m_pUpBall = new PhysicallyMoveBall(param, DamageBase::Type::_WeekDamage, 1, -0.015f);

        //打ち上げ中はあたり判定なし
        m_pUpBall->m_Damage.m_Enable = false;

        //サウンド
        Sound::Play(Sound::Swing1);
    }

    if (m_Timer == 60)
    {
        SetState(&TennisPlayerState_SlowUpBall::State_Finish);
    }

    //うち命令が出たら
    if (m_pControllClass->isShot())
    {
        //ショット分岐
        switch (GetShotType())
        {
        case ShotType::Smash:
            SetState(&TennisPlayerState_SlowUpBall::State_Smash);
            break;

        case ShotType::CutBall:
            SetState(&TennisPlayerState_SlowUpBall::State_CutShot);
            break;

        default:
            break;
        }
    }
}


void TennisPlayerState_SlowUpBall::State_Smash()
{
    const RADIAN angle_speed = D3DXToRadian(15);
    const int SmashFrame = 20;
    const int EndFrame = 30;

    ++m_Timer;

    if (m_Timer == 1)
    {
        //モーション
        m_pTennis->m_Renderer.SetMotion(TennisPlayer::_mt_Smash);
    }


    if (m_Timer == SmashFrame)
    {
        //打ち上げボールを消去
        m_pUpBall->m_DeleteFlag = true;
        m_pUpBall = nullptr;

        //ボール生成
        BallBase::Params param;

        //移動は前向き
        chr_func::GetFront(m_pTennis, &param.move);
        //スピードは適当
        param.move *= 1.2f;

        //キャラの場所に(最終的に腕の位置に？)
        param.pos = m_pTennis->m_Params.pos + Vector3(0, UsualBall::UsualBallShotY, 0);
        //高さをキャラ共通ボール発射のYに
        param.pos.y = BallBase::UsualBallShotY;

        //親を自分に
        param.pParent = m_pTennis;
        //通常タイプ
        param.type = BallBase::Type::_Usual;

        //生成
        (new UsualBall(param, DamageBase::Type::_VanishDamage, 15))->Counter(m_pTennis);


        //コントローラを振動
        controller::SetVibration(
            10000,
            0.15f,
            m_pTennis->m_PlayerInfo.number
            );

        //サウンド
        Sound::Play(Sound::AtkHit2);

        //エフェクト
        {
            //エフェクトの設定
            new HitEffectObject(
                param.pos,
                Vector3Normalize(param.move),
                0.1f,
                0.1f,
                Vector3(1, 1, 1)
                );
        }
    }

    if (m_Timer < SmashFrame)
    {
        //角度調整
        Vector3 v = m_pControllClass->GetVec();

        chr_func::AngleControll(m_pTennis, m_pTennis->m_Params.pos + v, angle_speed);

    }

    //一定時間で終了ステートへ
    if (m_Timer >= EndFrame)
    {
        SetState(&TennisPlayerState_SlowUpBall::State_Finish);
    }
}


void TennisPlayerState_SlowUpBall::State_CutShot()
{
    const RADIAN angle_speed = D3DXToRadian(10);
    const int ShothFrame = 8;
    const int EndFrame = 15;

    ++m_Timer;

    if (m_Timer == 1)
    {
        //モーション
        m_pTennis->m_Renderer.SetMotion(TennisPlayer::_mt_CutShot);
    }


    if (m_Timer == ShothFrame)
    {
        //打ち上げボールを消去
        m_pUpBall->m_DeleteFlag = true;
        m_pUpBall = nullptr;

        //ボール生成
        Vector3 pos, move;

        pos = m_pTennis->m_Params.pos + Vector3(0, BallBase::UsualBallShotY, 0);

        chr_func::GetFront(m_pTennis, &move);
        move *= 0.3f;
        move.y = 0.6f;

        new TennisBoundBall(
            pos,
            move,
            m_pTennis
            );


        //コントローラを振動
        controller::SetVibration(
            5000,
            0.15f,
            m_pTennis->m_PlayerInfo.number
            );

        //サウンド
        Sound::Play(Sound::Tennis_BallAtk);

        //エフェクト
        {
            //エフェクトの設定
            new HitEffectObject(
                pos,
                Vector3Normalize(move),
                0.1f,
                0.1f,
                Vector3(1, 1, 1)
                );
        }
    }

    if (m_Timer < ShothFrame)
    {
        //角度調整
        Vector3 v = m_pControllClass->GetVec();

        chr_func::AngleControll(m_pTennis, m_pTennis->m_Params.pos + v, angle_speed);

    }

    //一定時間で終了ステートへ
    if (m_Timer >= EndFrame)
    {
        SetState(&TennisPlayerState_SlowUpBall::State_Finish);
    }
}


void TennisPlayerState_SlowUpBall::State_Finish()
{
    m_pTennis->SetState(TennisState_PlayerControll_Move::GetPlayerControllMove(m_pTennis));
}
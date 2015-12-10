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
    m_pStateFunc(&TennisPlayerState_SlowUpBall::State_SlowUp),
    m_isDoShot(false)
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


TennisPlayerState_SlowUpBall::ShotType TennisPlayerState_SlowUpBall::GetShotType(int Timer)const
{
    //撃ち始め
    if (Timer < 20)
    {
        return ShotType::Weak;
    }
    else if (Timer < 32)//通常
    {
        return ShotType::Usual;
    }
    if (Timer < 50) //ボール最頂点
    {
        return ShotType::Smash;
    }

    //それ以外
    return ShotType::Usual;
}

void TennisPlayerState_SlowUpBall::SetState(void(TennisPlayerState_SlowUpBall::*pStateFunc)())
{
    m_pStateFunc = pStateFunc;
    m_Timer = 0;
}



void TennisPlayerState_SlowUpBall::State_SlowUp()
{
    const int SlowFrame = 10;

    ++m_Timer;

    //徐々にスピードダウン
    if (m_Timer < SlowFrame)
    {
        chr_func::XZMoveDown(m_pTennis, 0.2f);
    }

    //打ちフラグの更新
    if (m_isDoShot == false)
    {
        m_isDoShot = m_pControllClass->isShot();
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
        param.move.y = 0.4f;

        //キャラの場所に(最終的に腕の位置に？)
        param.pos = m_pTennis->m_Params.pos + Vector3(0, 2.0f, 0);
        //親を自分に
        param.pParent = m_pTennis;
        //カウンターできないタイプ
        param.type = BallBase::Type::_CantCounter;

        //生成
        m_pUpBall = new PhysicallyMoveBall(param, DamageBase::Type::_WeekDamage, 1, -0.0135f);// -0.015f);

        //打ち上げ中はあたり判定なし
        m_pUpBall->m_Damage.m_Enable = false;

        //サウンド
        Sound::Play(Sound::Swing1);
    }

    if (m_Timer == 60)
    {
        SetState(&TennisPlayerState_SlowUpBall::State_Finish);
        return;
    }

    if (m_Timer > SlowFrame)
    {
        //キャンセル
        if (m_pControllClass->DoOtherAction())
        {
            return;
        }

        //角度調整
        AngleControll(D3DXToRadian(8), 100.0f);

        //うち命令が出たら
        if (m_isDoShot)
        {
            //ショット分岐
            switch (GetShotType(m_Timer))
            {
            case ShotType::Weak:
                SetState(&TennisPlayerState_SlowUpBall::State_Weak);
                return;
            case ShotType::Usual:
                SetState(&TennisPlayerState_SlowUpBall::State_Usual);
                return;
            case ShotType::Smash:
                SetState(&TennisPlayerState_SlowUpBall::State_Smash);
                return;
            }
        }
    }
}

void TennisPlayerState_SlowUpBall::State_Weak()
{
    const RADIAN angle_speed = D3DXToRadian(15);
    const int ShotFrame = 9;
    const int EndFrame = 36;

    ++m_Timer;

    if (m_Timer == 1)
    {
        //モーション
        m_pTennis->m_Renderer.SetMotion(TennisPlayer::_mt_Shot);
    }


    if (m_Timer == ShotFrame)
    {
        //打ち上げボールを消去
        m_pUpBall->m_DeleteFlag = true;
        m_pUpBall = nullptr;

        //ボール生成
        BallBase::Params param;

        //移動は前向き
        chr_func::GetFront(m_pTennis, &param.move);
        //スピードは適当
        param.move *= 0.5f;

        //キャラの場所に(最終的に腕の位置に？)
        param.pos = m_pTennis->m_Params.pos + Vector3(0, UsualBall::UsualBallShotY, 0);
        //高さをキャラ共通ボール発射のYに
        param.pos.y = BallBase::UsualBallShotY;

        //親を自分に
        param.pParent = m_pTennis;
        //通常タイプ
        param.type = BallBase::Type::_Usual;

        //生成
        new UsualBall(param, DamageBase::Type::_WeekDamage, 3, 1, 120);


        //コントローラを振動
        chr_func::SetControllerShock(
            m_pTennis,
            0.5f,
            0.15f
            );

        //サウンド
        Sound::Play(Sound::Tennis_BallAtk);

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

    //キャンセル
    if (m_Timer > ShotFrame + 3)
    {
        m_pControllClass->DoOtherAction();
    }

    if (m_Timer < ShotFrame)
    {
        //角度調整
        AngleControll(D3DXToRadian(15), 100.0f);
    }

    //一定時間で終了ステートへ
    if (m_Timer >= EndFrame)
    {
        SetState(&TennisPlayerState_SlowUpBall::State_Finish);
    }
}

void TennisPlayerState_SlowUpBall::State_Usual()
{
    const RADIAN angle_speed = D3DXToRadian(15);
    const int ShotFrame = 3;
    const int EndFrame = 15;

    ++m_Timer;

    if (m_Timer == 1)
    {
        //モーション
        m_pTennis->m_Renderer.SetMotion(TennisPlayer::_mt_Smash);
    }


    if (m_Timer == ShotFrame)
    {
        //打ち上げボールを消去
        m_pUpBall->m_DeleteFlag = true;
        m_pUpBall = nullptr;

        //ボール生成
        BallBase::Params param;

        //移動は前向き
        chr_func::GetFront(m_pTennis, &param.move);
        //スピードは適当
        param.move *= 0.55f;

        //キャラの場所に(最終的に腕の位置に？)
        param.pos = m_pTennis->m_Params.pos + Vector3(0, UsualBall::UsualBallShotY, 0);
        //高さをキャラ共通ボール発射のYに
        param.pos.y = BallBase::UsualBallShotY;

        //親を自分に
        param.pParent = m_pTennis;
        //通常タイプ
        param.type = BallBase::Type::_Usual;

        //生成
        new UsualBall(param, DamageBase::Type::_WeekDamage, 5);


        //コントローラを振動
        chr_func::SetControllerShock(
            m_pTennis,
            0.5f,
            0.15f
            );

        //サウンド
        Sound::Play(Sound::Tennis_BallAtk);

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

    if (m_Timer < ShotFrame)
    {
        //角度調整
        AngleControll(D3DXToRadian(15), 100.0f);
    }

    //一定時間で終了ステートへ
    if (m_Timer >= EndFrame)
    {
        SetState(&TennisPlayerState_SlowUpBall::State_Finish);
    }
}

void TennisPlayerState_SlowUpBall::State_Smash()
{
    const RADIAN angle_speed = D3DXToRadian(15);
    const int SmashFrame = 3;
    const int EndFrame = 15;

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
        chr_func::SetControllerShock(
            m_pTennis,
            1.0f,
            0.15f
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
        AngleControll(D3DXToRadian(15), 100.0f);
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

void TennisPlayerState_SlowUpBall::AngleControll(RADIAN Speed, float CheckLen)
{
    //角度調整
    CharacterBase* pTarget = nullptr;

    if (chr_func::CalcAtkTarget(m_pTennis, D3DXToRadian(45), CheckLen, &pTarget))
    {
        chr_func::AngleControll(m_pTennis, pTarget->m_Params.pos, D3DXToRadian(10));
    }
    else
    {
        chr_func::AngleControll(m_pTennis, m_pTennis->m_Params.pos + m_pControllClass->GetVec(), Speed);
    }
}
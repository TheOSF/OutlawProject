#include "TennisState_BoundShot.h"
#include "TennisPlayerState_UsualMove.h"
#include "Tennis_HitEvent.h"
#include "../../GameSystem/GameController.h"
#include "../CharacterFunction.h"
#include "../../Ball/UsualBall.h"
#include "../../Damage/Damage.h"
#include "../CharacterMoveClass.h"
#include "../CharacterShotAttackClass.h"
#include "../CharacterManager.h"
#include "TennisPlayerState_UsualMove.h"

#include "../../Effect/EffectFactory.h"
#include "../../Effect/HitEffectObject.h"
#include "../../Sound/Sound.h"
#include "../../GameSystem/GameController.h"
#include "TennisState_BoundBall.h"
#include"TennisHeartBall.h"

TennisState_BoundShot::TennisState_BoundShot(
    ControllClass*       pControllClass //終了時にdeleteする
    ):
    m_pControllClass(pControllClass)
{

}

TennisState_BoundShot::~TennisState_BoundShot()
{
    delete m_pControllClass;
}

void TennisState_BoundShot::Enter(TennisPlayer* t)
{
    //カウント初期化
    m_Timer = 0;

    //モーションセット
    t->m_Renderer.SetMotion(TennisPlayer::_mt_Shot);
}

void TennisState_BoundShot::Execute(TennisPlayer* t)
{
    const int EndFrame = 38; //終了フレーム
    const int ShotFrame = 9;//打つフレーム
    const int AfterAction = 15;//ショット後のアクションが可能になるフレーム

    //カウント更新
    ++m_Timer;

    //撃ち始めの方向補正
    if (m_Timer < 3)
    {
        Vector3 vec = m_pControllClass->GetVec();
        chr_func::AngleControll(t, t->m_Params.pos + vec);
    }

    //移動値ダウン
    {
        chr_func::XZMoveDown(t, 0.1f);
    }

    //ショット後のアクション
    if (m_Timer > AfterAction && m_Timer < EndFrame)
    {
        m_pControllClass->DoShotAfterAction();
    }

    //方向補正
    if (m_Timer < ShotFrame)
    {
        const float AngleSpeed = D3DXToRadian(10);

        
            Vector3 vec = m_pControllClass->GetVec();
            chr_func::AngleControll(t, t->m_Params.pos + vec, AngleSpeed);
        
    }
    
    //サウンド
    if (m_Timer == ShotFrame - 3)
    {
        Sound::Play(Sound::Tennis_BallAtk);
    }
    

    //打つ！
    if (m_Timer == ShotFrame)
    {
        Vector3 pos, move;

        //キャラの場所に
        pos = t->m_Params.pos;
        //高さをキャラ共通ボール発射のYに
        pos.y = BallBase::UsualBallShotY;


        //移動は前向き
        chr_func::GetFront(t, &move);
        //スピードは適当
        move *= 0.2f;

        new TennisHeartBall(pos, move, t);
         

        /*
        //ボール発射
        BallBase::Params param;

        //移動は前向き
        chr_func::GetFront(t, &param.move);
        //スピードは適当
        param.move *= 0.3f;

        //キャラの場所に(最終的に腕の位置に？)
        param.pos = t->m_Params.pos;
        //高さをキャラ共通ボール発射のYに
        param.pos.y = BallBase::UsualBallShotY;

        //親を自分に
        param.pParent = t;
        //通常タイプ
        param.type = BallBase::Type::_Usual;

        //生成
        new UsualBall(param, DamageBase::Type::_WeekDamage, 5, 2, 80);

        */
        
        /*


        

        //ボール生成
        Vector3 pos, move;

        pos = t->m_Params.pos + Vector3(0, BallBase::UsualBallShotY, 0) + chr_func::GetFront(t)*0.8f;

        chr_func::GetFront(t, &move);
        move *= 0.4f;
        move.y = 0.3f;

        new TennisBoundBall(
            pos,
            move,
            t
            );

        //コントローラを振動
        chr_func::SetControllerShock(
            t,
            0.5f,
            0.15f
            );

        //エフェクト
        {
            COLORf EffectColor(CharacterBase::GetPlayerColor(t->m_PlayerInfo.number));

            //エフェクトの設定
            new HitEffectObject(
                pos,
                Vector3Normalize(move),
                0.08f,
                0.03f,
                Vector3(EffectColor.r, EffectColor.g, EffectColor.b),
                1,
                0
                );
        }

        //後ろに移動
        chr_func::AddMoveFront(t, 0.25f, 0.5f);

        */
    }

    //ステート終了
    if (m_Timer > EndFrame)
    {
        t->SetState(TennisState_PlayerControll_Move::GetPlayerControllMove(t));
    }

    //基本的な更新
    {
        TennisHitEvent HitEvent(t);
        chr_func::UpdateAll(t, &HitEvent);

        //モデル関連の更新
        t->m_Renderer.Update(1);
        chr_func::CreateTransMatrix(t, t->m_ModelSize, &t->m_Renderer.m_TransMatrix);
    }
}

void TennisState_BoundShot::Exit(TennisPlayer* t)
{
	
}
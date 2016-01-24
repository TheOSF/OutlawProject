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
    )
{

}

TennisState_BoundShot::~TennisState_BoundShot()
{

}

void TennisState_BoundShot::Enter(TennisPlayer* t)
{
    //カウント初期化
    m_Timer = 0;

    //モーションセット
    t->m_Renderer.SetMotion(TennisPlayer::_mt_HeartShot);
}

void TennisState_BoundShot::Execute(TennisPlayer* t)
{
    const int EndFrame = 38; //終了フレーム
    const int ShotFrame = 25;//打つフレーム
    const int AfterAction = 15;//ショット後のアクションが可能になるフレーム

    //カウント更新
    ++m_Timer;

    //撃ち始めの方向補正
    if (m_Timer < 3)
    {
        Vector3 vec = GetVec();
        chr_func::AngleControll(t, t->m_Params.pos + vec);
    }

    //移動値ダウン
    {
        chr_func::XZMoveDown(t, 0.1f);
    }

    //方向補正
    if (m_Timer < ShotFrame)
    {
        const float AngleSpeed = D3DXToRadian(10);

 
        Vector3 vec = GetVec();
        chr_func::AngleControll(t, t->m_Params.pos + vec, AngleSpeed);
        
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
        chr_func::CreateTransMatrix(t, &t->m_Renderer.m_TransMatrix);
    }
}

void TennisState_BoundShot::Exit(TennisPlayer* t)
{
	
}

Vector3 TennisState_BoundShot::GetVec()
{
    return Vector3Zero;
}
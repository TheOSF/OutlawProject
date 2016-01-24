#include "AmefootPlayerState_ThrowBall.h"
#include "AmefootPlayerState_UsualMove.h"
#include "../AmefootUsualHitEvent.h"
#include "../../../GameSystem/GameController.h"
#include "../../CharacterFunction.h"
#include "../../../Ball/UsualBall.h"
#include "../../../Damage/Damage.h"
#include "../../CharacterMoveClass.h"
#include "../../CharacterShotAttackClass.h"
#include "../../CharacterManager.h"
#include "../../../Effect/EffectFactory.h"
#include "../../../Effect/HitEffectObject.h"
#include "../../../Sound/Sound.h"
#include "../../../Ball/AmefootBoundBall.h"



//-----------------------------------------------------------------------------------------
// AmefootPlayerState_ThrowBall
// [ アメフト ] 遠距離攻撃(ボール投げ)をするステートクラス
//-----------------------------------------------------------------------------------------
AmefootPlayerState_ThrowBall::AmefootPlayerState_ThrowBall() 
{}
//-----------------------------------------------------------------------------------------
AmefootPlayerState_ThrowBall::~AmefootPlayerState_ThrowBall()
{
    
}
//-----------------------------------------------------------------------------------------
void AmefootPlayerState_ThrowBall::Enter(AmefootPlayer* pCharacter)
{
    //カウント初期化
    m_Timer = 0;

    //モーションセット
    pCharacter->m_Renderer.SetMotion(AmefootPlayer::Motion_Throw_Ball);

    //撃てないタイマーセット
    pCharacter->SetDontThrowBallTimer(50);
}
//-----------------------------------------------------------------------------------------
void AmefootPlayerState_ThrowBall::Execute(AmefootPlayer* pCharacter)
{
    const int EndFrame = 21; //終了フレーム
    const int ShotFrame = 10;//打つフレーム
    const int AfterAction = 15;//ショット後のアクションが可能になるフレーム
    
    //カウント更新
    ++m_Timer;

    //移動値ダウン
    {
        chr_func::XZMoveDown(pCharacter, 0.1f);
    }

    //サウンド
    if ( m_Timer == ShotFrame - 3 )
    {
        //Sound::Play(Sound::Tennis_BallAtk);
    }

    //打つ！
    if ( m_Timer == ShotFrame )
    {
        Vector3 pos, move;

        pos = pCharacter->m_Params.pos + Vector3(0, BallBase::UsualBallShotY, 0) + chr_func::GetFront(pCharacter)*0.8f;

        chr_func::GetFront(pCharacter, &move);
        move *= 0.35f;
        move.y = 0.7f;

        new AmefootBoundBall(
        pos,
        move,
        pCharacter
        );

        //コントローラを振動
        chr_func::SetControllerShock(
            pCharacter,
            0.5f,
            0.15f
            );

        //エフェクト
        {
        COLORf EffectColor(CharacterBase::GetPlayerColor(pCharacter->m_PlayerInfo.number));

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
        //chr_func::AddMoveFront(pCharacter, 0.25f, 0.5f);
    }

    //ステート終了
    if ( m_Timer > EndFrame )
    {
        pCharacter->SetState(AmefootPlayerState_UsualMove::GetPlayerControllMove(pCharacter));
    }

    //基本的な更新
    {
        AmefootUsualHitEvent HitEvent(pCharacter);
        chr_func::UpdateAll(pCharacter, &HitEvent);

        //モデル関連の更新
        pCharacter->m_Renderer.Update(2.0f);
        chr_func::CreateTransMatrix(pCharacter, &pCharacter->m_Renderer.m_TransMatrix);
    }
}
//-----------------------------------------------------------------------------------------
void AmefootPlayerState_ThrowBall::Exit(AmefootPlayer* pCharacter)
{}
//-----------------------------------------------------------------------------------------



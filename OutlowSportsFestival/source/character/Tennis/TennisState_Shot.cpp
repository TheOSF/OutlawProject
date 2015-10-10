#include "TennisState_Shot.h"
#include "TennisPlayerState_UsualMove.h"
#include "Tennis_HitEvent.h"
#include "../../GameSystem/GameController.h"
#include "../CharacterFunction.h"
#include "../../Ball/UsualBall.h"
#include "../../Damage/Damage.h"
#include "../CharacterCounterClass.h"
#include "../CharacterMoveClass.h"
#include "../CharacterShotAttackClass.h"
#include "../CharacterManager.h"
#include "TennisPlayerState_UsualMove.h"

#include "../../Effect/HitEffectObject.h"
#include "../../Sound/Sound.h"


TennisState_Shot::TennisState_Shot(
    ControllClass*       pControllClass //終了時にdeleteする
    ):
    m_pControllClass(pControllClass)
{

}

TennisState_Shot::~TennisState_Shot()
{
    delete m_pControllClass;
}

void TennisState_Shot::Enter(TennisPlayer* t)
{
    //カウント初期化
    m_Timer = 0;

    //モーションセット
    t->m_Renderer.SetMotion(TennisPlayer::_mt_Shot);

    {
        //移動量制限
        const float MaxMove = 0.06f;
        
        if (t->m_Params.move.Length() > MaxMove)
        {
            t->m_Params.move.Normalize();
            t->m_Params.move *= MaxMove;
        }
    }

    {
        //打ち上げボール生成
        BallBase::Params param;

        //移動は上向き
        param.move = t->m_Params.move;
        param.move.y = 0.5f;
        
        //キャラの場所に(最終的に腕の位置に？)
        param.pos = t->m_Params.pos + Vector3(0, 2.0f, 0);
        //親を自分に
        param.pParent = t;
        //カウンターできないタイプ
        param.type = BallBase::Type::_CantCounter;

        //生成
        m_pUpBall = new PhysicallyMoveBall(param, DamageBase::Type::_WeekDamage, 1, -0.025f);
   
        //打ち上げ中はあたり判定なし
        m_pUpBall->m_Damage.m_Enable = false;
    }
}

void TennisState_Shot::Execute(TennisPlayer* t)
{
    const int EndFrame = 48; //終了フレーム
    const int ShotFrame = 30;//打つフレーム
    const int CancelStart = 10;//キャンセル行動可能なフレーム
    const int AfterAction = 33;//ショット後のアクションが可能になるフレーム

    //カウント更新
    ++m_Timer;

    //打ちキャンセル
    if (m_Timer > CancelStart && m_Timer < ShotFrame - 3)
    {        
        if (m_pUpBall != nullptr && m_pControllClass->DoOtherAction())
        {
            m_pUpBall->m_Damage.m_Enable = true;
            m_pUpBall->m_Params.type = BallBase::Type::_CantCounter;
        }
    }

    //ショット後のアクション
    if (m_Timer > AfterAction && m_Timer < EndFrame)
    {
        m_pControllClass->DoShotAfterAction();
    }

    //方向補正
    if (m_Timer < ShotFrame)
    {
        const CharacterBase* const pTargetCharacter = GetFrontTarget(t);
        const float AngleSpeed = D3DXToRadian(3);

        if (pTargetCharacter != nullptr)
        {
            //自動回転
            chr_func::AngleControll(t, pTargetCharacter->m_Params.pos, AngleSpeed*2.0f);
        }
        else
        {   
            Vector3 vec = m_pControllClass->GetVec();
            chr_func::AngleControll(t, t->m_Params.pos + vec, AngleSpeed);
        }
    }

    //打ちあげたボールが落下中の場合ダメージ判定を有効に
    if (m_pUpBall != nullptr&&
        m_pUpBall->m_Params.move.y < 0)
    {
        m_pUpBall->m_Damage.m_Enable = true;
        m_pUpBall->m_Params.type = BallBase::Type::_CantCounter;
    }
    
    //サウンド
    if (m_Timer == ShotFrame - 3)
    {
        Sound::Play(Sound::Tennis_BallAtk);
    }
    

    //打つ！
    if (m_Timer == ShotFrame)
    {
        //打ちあげたボールの状態チェック
        if (
            m_pUpBall != nullptr&&
            m_pUpBall->m_Params.pParent == t&&
            m_pUpBall->m_Params.type == BallBase::Type::_CantCounter
            )
        {
            
            //ボール発射
            BallBase::Params param;
            
            //移動は前向き
            chr_func::GetFront(t, &param.move);
            //スピードは適当
            param.move *= 0.6f;

            //キャラの場所に(最終的に腕の位置に？)
            param.pos = m_pUpBall->m_Params.pos;
            //高さをキャラ共通ボール発射のYに
            param.pos.y = BallBase::UsualBallShotY;	

            //親を自分に
            param.pParent = t;
            //通常タイプ
            param.type = BallBase::Type::_Usual;
            
            //生成
            new UsualBall(param, DamageBase::Type::_WeekDamage, 5);


            //エフェクト
            {
                COLORf EffectColor(CharacterBase::GetPlayerColor(t->m_PlayerInfo.number));

                //エフェクトの設定
                new HitEffectObject(
                    param.pos,
                    Vector3Normalize(param.move),
                    0.1f,
                    0.1f,
                    Vector3(EffectColor.r, EffectColor.g, EffectColor.b)
                    );
            }

            //上に上げたボールを消去
            m_pUpBall->m_DeleteFlag = true;
            m_pUpBall = nullptr;
        }
    }

    //ステート終了
    if (m_Timer > EndFrame)
    {
        t->SetState(TennisState_PlayerControll_Move::GetPlayerControllMove(t));
    }

    //打ち終わっていた場合
    if (m_Timer > ShotFrame)
    {
        chr_func::XZMoveDown(t, 0.1f);
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

void TennisState_Shot::Exit(TennisPlayer* t)
{
	
}


const CharacterBase* TennisState_Shot::GetFrontTarget(TennisPlayer* t)const
{
    CharacterManager::CharacterMap ChrMap = DefCharacterMgr.GetCharacterMap();

    const CharacterBase* pTargetEnemy = nullptr;    //ターゲット保持のポインタ
    RADIAN MostMinAngle = D3DXToRadian(33);         //もっとも狭い角度
    RADIAN TempAngle;

    Vector3 MyFront;      //自身の前方ベクトル
    chr_func::GetFront(t, &MyFront);

    auto it = ChrMap.begin();

    while (it != ChrMap.end())
    {
        //自身を除外
        if (t->m_PlayerInfo.number == it->first->m_PlayerInfo.number ||
            chr_func::isDie(it->first))
        {
            ++it;
            continue;
        }

        //前ベクトルと敵へのベクトルの角度を計算する
        TempAngle = Vector3Radian(MyFront, (it->first->m_Params.pos - t->m_Params.pos));

        //角度が一番狭かったら更新
        if (TempAngle < MostMinAngle)
        {
            pTargetEnemy = it->first;
            MostMinAngle = TempAngle;
        }

        ++it;
    }
    return pTargetEnemy;
}
#include "TennisPlayerState_Vanish.h"
#include "TennisPlayerState_UsualMove.h"
#include "Tennis_HitEvent.h"
#include "../CharacterFunction.h"

#include "../../Effect/HitEffectObject.h"
#include "../../Effect/SoccerSpecialHitEffect.h"
#include "../../Effect/BlurImpact.h"

#include "../../GameSystem/GameController.h"
#include "../../Effect/EffectFactory.h"
#include "Tennis_DoCancelAction.h"


TennisState_DamageVanish::TennisState_DamageVanish(
    TennisPlayer* pTennis,
    const Vector3& Damage_vec  //ダメージを受けた方向
    ) :
    m_pTennis(pTennis),
    m_Damage_vec(Damage_vec)
{

}

void TennisState_DamageVanish::Enter(TennisPlayer* t)
{
    //キャラクタ共通ひるみクラスのテニス固有イベントクラス
    class TennisEvent :public CharacterDamageVanish::Event
    {
        Tennis_DoCancelAction* m_pDoCancelAction;
    public:
        TennisEvent(TennisPlayer* pTennis) :m_pTennis(pTennis)
        {
            if (pTennis->m_PlayerInfo.player_type == PlayerType::_Player)
            {
                m_pDoCancelAction = new Tennis_DoCancelAction_Player(pTennis);

                m_pDoCancelAction->SetDontActions(Tennis_DoCancelAction_Player::Action::BoundBallShot);
                m_pDoCancelAction->SetDontActions(Tennis_DoCancelAction_Player::Action::Counter);
                m_pDoCancelAction->SetDontActions(Tennis_DoCancelAction_Player::Action::FarAtk);
                m_pDoCancelAction->SetDontActions(Tennis_DoCancelAction_Player::Action::NearAtk);
            }
            else
            {
                m_pDoCancelAction = new Tennis_DoCancelAction_Computer(pTennis);
            }
        }

        ~TennisEvent()
        {
            delete m_pDoCancelAction;
        }

        void FlyStart()
        {
            //吹き飛びモーションをセット
            m_pTennis->m_Renderer.SetMotion(TennisPlayer::_mt_Damage_Vanish_Fly);
        }

        void Flying(const Matrix& Rotate, RATIO t)
        {
            //モデルのアニメーション更新
            m_pTennis->m_Renderer.Update(t);

            //位置にもとずき、ワールド変換行列を計算
            chr_func::CreateTransMatrix(m_pTennis, &m_pTennis->m_Renderer.m_TransMatrix);

            //吹き飛びの回転を入れる
            m_pTennis->m_Renderer.m_TransMatrix = Rotate*m_pTennis->m_Renderer.m_TransMatrix;
        }

        void DownStart()
        {
            //ダウンモーションをセット
            m_pTennis->m_Renderer.SetMotion(TennisPlayer::_mt_Damage_Vanish_Down);
        }

        void Downing()
        {
            m_pTennis->m_Renderer.Update(1);

            //ワールド変換行列を計算
            chr_func::CreateTransMatrix(m_pTennis, &m_pTennis->m_Renderer.m_TransMatrix);
        }

        void StandUpStart()
        {
            //起き上がりモーションをセット
            m_pTennis->m_Renderer.SetMotion(TennisPlayer::_mt_Damage_Vanish_StandUp);
        }

        void StandUping()
        {
            //モデルのアニメーション更新
            m_pTennis->m_Renderer.Update(1);

            //ワールド変換行列を計算
            chr_func::CreateTransMatrix(m_pTennis, &m_pTennis->m_Renderer.m_TransMatrix);
        }

        void End()
        {
            //通常ステートをセット
            m_pTennis->SetState(
                TennisState_PlayerControll_Move::GetPlayerControllMove(m_pTennis)
                );
        }

        void HitWall()
        {
            //壁に当たったモーションをセット
            m_pTennis->m_Renderer.SetMotion(TennisPlayer::_mt_Damage_Vanish_HitWallAndDown);
        }

        void HitFloor()
        {
            //床に当たったモーションをセット
            m_pTennis->m_Renderer.SetMotion(TennisPlayer::_mt_Damage_Vanish_HitFloor);
        }

        void HitFloorAndStandUp()
        {
            //立ち上がりモーションをセット
            m_pTennis->m_Renderer.SetMotion(TennisPlayer::_mt_Damage_Vanish_HitFloorAndStandUp);
        }

        void HitWallUpdate()
        {
            //モデルのアニメーション更新
            m_pTennis->m_Renderer.Update(1);

            //ワールド変換行列を計算
            chr_func::CreateTransMatrix(m_pTennis, &m_pTennis->m_Renderer.m_TransMatrix);
        }

        void CanActionUpdate()
        {
            //行動分岐が可能なときに呼ばれる
            m_pDoCancelAction->DoAction();
        }

    private:
        TennisPlayer*  m_pTennis;
    };


    //ダメージモーションパラメーターを作成する
    CharacterDamageVanish::Param Param;

    Param.rotate_speed = Vector3(0.0f, 0.0f, 0.0);
    Param.move = m_Damage_vec;

    Param.down_frame = 15;
    Param.standup_frame = 45;


    //ひるみクラスを作成
    m_pDamageVanishClass = new CharacterDamageVanish(
        m_pTennis,
        Param,
        new TennisEvent(t),
        new TennisHitEvent(t)
        );

    //エフェクト
    EffectFactory::VanishEffect(
        m_pTennis,
        m_Damage_vec
        );

}

void TennisState_DamageVanish::Execute(TennisPlayer* t)
{
    m_pDamageVanishClass->Update();
}

void TennisState_DamageVanish::Exit(TennisPlayer* t)
{
    delete m_pDamageVanishClass;
}

void TennisState_DamageVanish::StateMachineExit(TennisPlayer* t)
{
    Exit(t);
}
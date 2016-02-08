#include "AmefootPlayerState_Die.h"
#include "../AmefootUsualHitEvent.h"
#include "../../CharacterFunction.h"
#include "../../../Effect/HitEffectObject.h"
#include "../../../Ball/Ball.h"
#include "../../../Camera/Camera.h"
#include "../../../Effect/GlavityLocus.h"
#include "../../../Effect/EffectFactory.h"
#include "../../../GameSystem/GameController.h"
#include "../../CharacterDamageVanish.h"


AmefootPlayerState_Die::AmefootPlayerState_Die(
    AmefootPlayer* pAmefootPlayer,
    const Vector3& Damage_vec
    ) :
    m_pAmefootPlayer(pAmefootPlayer),
    m_Damage_vec(Damage_vec)
{}

void AmefootPlayerState_Die::Enter(AmefootPlayer* pCharacter)
{
    //キャラクタ共通ひるみクラスのテニス固有イベントクラス
    class AmefootEvent :public CharacterDamageVanish::Event
    {
    public:
        AmefootEvent(AmefootPlayer* pAmefootPlayer) :m_pAmefootPlayer(pAmefootPlayer) {}

        void FlyStart()
        {
            //吹き飛びモーションをセット
            m_pAmefootPlayer->m_Renderer.SetMotion(AmefootPlayer::Motion_Damage_Vanish_Start);
        }

        void Flying(const Matrix& Rotate, RATIO t)
        {
            //モデルのアニメーション更新
            m_pAmefootPlayer->m_Renderer.Update(t);

            //位置にもとずき、ワールド変換行列を計算
            chr_func::CreateTransMatrix(m_pAmefootPlayer, &m_pAmefootPlayer->m_Renderer.m_TransMatrix);

            //吹き飛びの回転を入れる
            m_pAmefootPlayer->m_Renderer.m_TransMatrix = Rotate*m_pAmefootPlayer->m_Renderer.m_TransMatrix;
        }

        void DownStart()
        {
            //ダウンモーションをセット
            m_pAmefootPlayer->m_Renderer.SetMotion(AmefootPlayer::Motion_Damage_Vanish_Landing);
        }

        void Downing()
        {
            m_pAmefootPlayer->m_Renderer.Update(1);

            //ワールド変換行列を計算
            chr_func::CreateTransMatrix(m_pAmefootPlayer, &m_pAmefootPlayer->m_Renderer.m_TransMatrix);

        }

        void StandUpStart()
        {
            //何もしない
        }

        void StandUping()
        {
            //モデルのアニメーション更新
            m_pAmefootPlayer->m_Renderer.Update(1);

            //ワールド変換行列を計算
            chr_func::CreateTransMatrix(m_pAmefootPlayer, &m_pAmefootPlayer->m_Renderer.m_TransMatrix);
        }

        void End()
        {
            //カメラ写すフラグをfalseに
            m_pAmefootPlayer->m_DrawObject.m_isDraw = false;
        }
        void HitWall()
        {
            //壁に当たったモーションをセット
            m_pAmefootPlayer->m_Renderer.SetMotion(AmefootPlayer::Motion_Damage_Vanish_Hit_Wall_Fall);
        }

        void HitFloor()
        {
            //床に当たったモーションをセット
            m_pAmefootPlayer->m_Renderer.SetMotion(AmefootPlayer::Motion_Damage_Vanish_Hit_Wall_Landing);
        }

        void HitFloorAndStandUp()
        {
            //何もしない(立ち上がらない
        }

        void HitWallUpdate()
        {
            //モデルのアニメーション更新
            m_pAmefootPlayer->m_Renderer.Update(1);

            //ワールド変換行列を計算
            chr_func::CreateTransMatrix(m_pAmefootPlayer, &m_pAmefootPlayer->m_Renderer.m_TransMatrix);
        }
    private:
        AmefootPlayer*  m_pAmefootPlayer;
    };

    //ダメージモーションパラメーターを作成する
    CharacterDamageVanish::Param  Param;

    Param.rotate_speed = Vector3(0.0f, 0.0f, 0.0f);

    Param.move = m_Damage_vec;
    Param.down_frame = 10;
    Param.standup_frame = 50;

    //ひるみクラスを作成
    m_pDamageVanishClass = new CharacterDamageVanish(
        pCharacter,
        Param,
        new AmefootEvent(pCharacter),
        new DamageManager::HitEventBase()
        );

    //死亡エフェクト
    EffectFactory::DieEffect(
        pCharacter,
        m_Damage_vec
        );

    //コントローラを振動
    chr_func::SetControllerShock(
        m_pAmefootPlayer,
        0.8f,
        0.5f
        );
}

void AmefootPlayerState_Die::Execute(AmefootPlayer* pCharacter)
{
    m_pDamageVanishClass->Update();
}

void AmefootPlayerState_Die::Exit(AmefootPlayer* pCharacter)
{
    delete m_pDamageVanishClass;
}



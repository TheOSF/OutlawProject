#include "TennisPlayerState_Vanish.h"
#include "TennisPlayerState_UsualMove.h"
#include "Tennis_HitEvent.h"
#include "../CharacterFunction.h"

#include "../../Effect/HitEffectObject.h"
#include "../../Effect/BlurImpact.h"


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
    public:
        TennisEvent(TennisPlayer* pTennis) :m_pTennis(pTennis){}

        void FlyStart()
        {
            //吹き飛びモーションをセット
            m_pTennis->m_Renderer.SetMotion(TennisPlayer::_mt_Damage_Vanish_Fly);
        }

        void Flying(const Matrix& Rotate)
        {
            //モデルのアニメーション更新
            m_pTennis->m_Renderer.Update(1);

            //位置にもとずき、ワールド変換行列を計算
            chr_func::CreateTransMatrix(m_pTennis, m_pTennis->m_ModelSize, &m_pTennis->m_Renderer.m_TransMatrix);

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
            chr_func::CreateTransMatrix(m_pTennis, m_pTennis->m_ModelSize, &m_pTennis->m_Renderer.m_TransMatrix);
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
            chr_func::CreateTransMatrix(m_pTennis, m_pTennis->m_ModelSize, &m_pTennis->m_Renderer.m_TransMatrix);
        }

        void End()
        {
            //通常ステートをセット
            m_pTennis->SetState(
                TennisState_PlayerControll_Move::GetPlayerControllMove(m_pTennis)
                );
        }


    private:
        TennisPlayer*  m_pTennis;
    };

    //ダメージモーションパラメーターを作成する
    CharacterDamageVanish::Param Param;

    Param.rotate_speed = Vector3(0.0f, 0.0f, 0.0);
    Param.move = Vector3Normalize(m_Damage_vec)*0.6f;
    Param.move.y = 0.2f;

    Param.down_frame = 15;
    Param.down_muteki_frame = 15;
    Param.standup_frame = 50;
    Param.standup_muteki_frame = 10;


    //ひるみクラスを作成
    m_pDamageVanishClass = new CharacterDamageVanish(
        m_pTennis,
        Param,
        new TennisEvent(t),
        new TennisHitEvent(t)
        );

    //ヒットエフェクト作成
    new HitEffectObject(
        m_pTennis->m_Params.pos + Vector3(0, 3, 0),
        m_Damage_vec,
        0.05f,
        0.15f,
        Vector3(1.0f, 1.0f, 1.0f)
        );

    //ブラーエフェクト
    new BlurImpactSphere(
        m_pTennis->m_Params.pos + Vector3(0, 3, 0),
        25,
        10,
        30
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
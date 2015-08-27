#include "TennisPlayerState_DamageMotionDie.h"
#include "Tennis_HitEvent.h"
#include "../CharacterFunction.h"

TennisState_DamageMotion_Die::TennisState_DamageMotion_Die(
    TennisPlayer* pTennis,
    const Vector3& Damage_vec  //ダメージを受けた方向
    ) :
    m_pTennis(pTennis),
    m_Damage_vec(Damage_vec)
{

}

void TennisState_DamageMotion_Die::Enter(TennisPlayer* t)
{
    //キャラクタ共通ひるみクラスのテニス固有イベントクラス
    class TennisEvent :public CharacterDamageVanish::Event
    {
    public:
        TennisEvent(TennisPlayer* pTennis) :m_pTennis(pTennis), m_Timer(0){}

        void Flying(const Matrix& Rotate)
        {
            //モデルのアニメーション更新
            m_pTennis->m_Renderer.Update(1);

            //位置にもとずき、ワールド変換行列を計算
            chr_func::CreateTransMatrix(m_pTennis, m_pTennis->m_ModelSize, &m_pTennis->m_Renderer.m_TransMatrix);

            //吹き飛びの回転を入れる
            m_pTennis->m_Renderer.m_TransMatrix = Rotate*m_pTennis->m_Renderer.m_TransMatrix;
        }

        void StandUpStart()
        {
            //吹き飛びモーションをセット
            m_pTennis->m_Renderer.SetMotion(TennisPlayer::_mt_Damage_Vanish_Down);
        }

        void StandUping()
        {
            //モデルのアニメーション更新
            m_pTennis->m_Renderer.Update(1);

            //ワールド変換行列を計算
            chr_func::CreateTransMatrix(m_pTennis, m_pTennis->m_ModelSize, &m_pTennis->m_Renderer.m_TransMatrix);
        }

        void Start()
        {
            //吹き飛びモーションをセット
            m_pTennis->m_Renderer.SetMotion(TennisPlayer::_mt_Damage_Vanish_Fly);

        }

        void End()
        {

        }
    private:
        TennisPlayer*  m_pTennis;
        int            m_Timer;
    };

    //ダメージモーションパラメーターを作成する
    CharacterDamageVanish::Param Param;

    Param.rotate_speed = Vector3(0.0f, 0.0f, 0.0);
    Param.move = m_Damage_vec * 0.9f;
    Param.move.y = 0.2f;
    Param.standup_frame = 30;

    //ひるみクラスを作成
    m_pDamageVanishClass = new CharacterDamageVanish(
        m_pTennis,
        Param,
        new TennisEvent(t)
        );

}

void TennisState_DamageMotion_Die::Execute(TennisPlayer* t)
{
    m_pDamageVanishClass->Update();
}

void TennisState_DamageMotion_Die::Exit(TennisPlayer* t)
{
    delete m_pDamageVanishClass;
}
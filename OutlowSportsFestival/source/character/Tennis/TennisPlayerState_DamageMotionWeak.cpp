#include "TennisPlayerState_DamageMotionWeak.h"
#include "TennisPlayerState_UsualMove.h"
#include "Tennis_HitEvent.h"
#include "../CharacterFunction.h"
#include "../../Effect/HitEffectObject.h"
#include "../../Effect/BlurImpact.h"


TennisState_DamageMotion_Weak::TennisState_DamageMotion_Weak(
	TennisPlayer*  pTennis,
	const Vector3& Damage_vec  //ダメージを受けた方向
	):
	m_pTennis(pTennis),
	m_Damage_vec(Damage_vec)
{

}

void TennisState_DamageMotion_Weak::Enter(TennisPlayer* t)
{
	//キャラクタ共通ひるみクラスのテニス固有イベントクラス
	class TennisEvent :public CharacterDamageMotion::Event
	{
	public:
		TennisEvent(TennisPlayer* pTennis) :m_pTennis(pTennis){}

		void Update(float speed)
		{
			//モデルの更新のみ
			m_pTennis->m_Renderer.Update(1);
			chr_func::CreateTransMatrix(m_pTennis, m_pTennis->m_ModelSize, &m_pTennis->m_Renderer.m_TransMatrix);
		}
		void Start()
		{
			//ひるみモーションをセット
			m_pTennis->m_Renderer.SetMotion(TennisPlayer::_mt_Damage_Weak);
		}
		void End()
		{
			//通常ステートをセット
			m_pTennis->SetState(
				TennisState_PlayerControll_Move::GetPlayerControllMove(m_pTennis)
				);

            m_pTennis->m_Renderer.m_HDR = Vector3Zero;
		}

        void SetLight(float power)
        {
            m_pTennis->m_Renderer.m_HDR = Vector3(1, 1, 1) * power;
        }
	private:
		TennisPlayer*  m_pTennis;
	};
	 
	//ダメージモーションパラメーターを作成する
	CharacterDamageMotion::Params Param;

	Param.AllFrame = 35;
	Param.damage_vec = m_Damage_vec;
	Param.hitBack = 0.2f;
	Param.hitStopFrame = 5;
	Param.NoDamageFrame = 5;

	//ひるみクラスを作成
	m_pDamageMotionClass = new CharacterDamageMotion(
		m_pTennis,
		new TennisEvent(m_pTennis),
		new TennisHitEvent(m_pTennis),
		Param
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
        10,
        15,
        30
        );
}

void TennisState_DamageMotion_Weak::Execute(TennisPlayer* t)
{
	m_pDamageMotionClass->Update();
}

void TennisState_DamageMotion_Weak::Exit(TennisPlayer* t)
{
    delete m_pDamageMotionClass;
}
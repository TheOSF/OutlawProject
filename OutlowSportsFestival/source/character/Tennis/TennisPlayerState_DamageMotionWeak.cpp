#include "TennisPlayerState_DamageMotionWeak.h"
#include "TennisPlayerState_UsualMove.h"
#include "Tennis_HitEvent.h"
#include "../CharacterFunction.h"
#include "../../Effect/HitEffectObject.h"
#include "../../Effect/BlurImpact.h"
#include "../../GameSystem/GameController.h"
#include "../../Effect/EffectFactory.h"

TennisState_DamageMotion_Weak::TennisState_DamageMotion_Weak(
	TennisPlayer*  pTennis,
	const Vector3& Damage_vec,  //ダメージを受けた方向
    bool           CounterHit,
    int            Frame
	):
	m_pTennis(pTennis),
	m_Damage_vec(Damage_vec),
    m_CounterHit(CounterHit),
    m_Frame(Frame)
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
            m_pTennis->m_Renderer.Update(speed);
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

	Param.damage_vec = m_Damage_vec;
    Param.counter_hit = m_CounterHit;
    Param.frame = m_Frame;

	//ひるみクラスを作成
	m_pDamageMotionClass = new CharacterDamageMotion(
		m_pTennis,
		new TennisEvent(m_pTennis),
		new TennisHitEvent(m_pTennis),
		Param
        );

    //エフェクト
    EffectFactory::HitEffect(
        m_pTennis,
        m_Damage_vec
        );

    //コントローラを振動
    chr_func::SetControllerShock(
        m_pTennis,
        0.5f,
        0.15f
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
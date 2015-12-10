#include "BaseballPlayerState_DamageMotion_Vanish.h"
#include "BaseballPlayerState.h"
#include "Baseball_HitEvent.h"
#include "../CharacterFunction.h"
#include "../../Effect/EffectFactory.h"
#include "../../GameSystem/GameController.h"

BaseballState_DamageVanish::BaseballState_DamageVanish(
	BaseballPlayer* pBaseball,
	const Vector3& Damage_vec  //ダメージを受けた方向
	) :
	m_pBaseball(pBaseball),
	m_Damage_vec(Damage_vec)
{

}

void BaseballState_DamageVanish::Enter(BaseballPlayer* b)
{
	
	class BaseballEvent :public CharacterDamageVanish::Event
	{
	public:
		BaseballEvent(BaseballPlayer* pBaseball) :m_pBaseball(pBaseball){}

		void FlyStart()
		{
			//吹き飛びモーションをセット
			m_pBaseball->m_Renderer.SetMotion(baseball_player::_mb_Damage_Vanish_Fly);
		}

        void Flying(const Matrix& Rotate, RATIO t)
		{
			//モデルのアニメーション更新
			m_pBaseball->m_Renderer.Update(1);

			//位置にもとずき、ワールド変換行列を計算
			chr_func::CreateTransMatrix(m_pBaseball, m_pBaseball->m_ModelSize, &m_pBaseball->m_Renderer.m_TransMatrix);

			//吹き飛びの回転を入れる
			m_pBaseball->m_Renderer.m_TransMatrix = Rotate*m_pBaseball->m_Renderer.m_TransMatrix;
		}

		void DownStart()
		{
			//ダウンモーションをセット
			m_pBaseball->m_Renderer.SetMotion(baseball_player::_mb_Damage_Vanish_Down);
		}

		void Downing()
		{
			m_pBaseball->m_Renderer.Update(1);

			//ワールド変換行列を計算
			chr_func::CreateTransMatrix(m_pBaseball, m_pBaseball->m_ModelSize, &m_pBaseball->m_Renderer.m_TransMatrix);
		}

		void StandUpStart()
		{
			//起き上がりモーションをセット
			m_pBaseball->m_Renderer.SetMotion(baseball_player::_mb_Damage_Vanish_StandUp);
		}

		void StandUping()
		{
			//モデルのアニメーション更新
			m_pBaseball->m_Renderer.Update(1);

			//ワールド変換行列を計算
			chr_func::CreateTransMatrix(m_pBaseball, m_pBaseball->m_ModelSize, &m_pBaseball->m_Renderer.m_TransMatrix);
		}

		void End()
		{
			//通常ステートをセット
			m_pBaseball->SetState(
				BaseballState_PlayerControll_Move::GetPlayerControllMove(m_pBaseball)
				);
		}


	private:
		BaseballPlayer*  m_pBaseball;
	};

	//ダメージモーションパラメーターを作成する
	CharacterDamageVanish::Param Param;

	Param.rotate_speed = Vector3(0.0f, 0.0f, 0.0);
	Param.move = m_Damage_vec;

	Param.down_frame = 15;
	Param.standup_frame = 50;


	//ひるみクラスを作成
	m_pDamageVanishClass = new CharacterDamageVanish(
		m_pBaseball,
		Param,
		new BaseballEvent(b),
		new BaseballHitEvent(b)
		);

    //エフェクト
    EffectFactory::VanishEffect(
        m_pBaseball,
        m_Damage_vec
        );

	//コントローラを振動
    chr_func::SetControllerShock(
        m_pBaseball,
        0.5f,
        0.15f
        );
}

void BaseballState_DamageVanish::Execute(BaseballPlayer* b)
{
	m_pDamageVanishClass->Update();
}

void BaseballState_DamageVanish::Exit(BaseballPlayer* b)
{
	delete m_pDamageVanishClass;
}
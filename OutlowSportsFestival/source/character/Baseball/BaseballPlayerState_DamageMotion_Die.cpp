#include "BaseballPlayerState_DamageMotion_Die.h"
#include "Baseball_HitEvent.h"
#include "../CharacterFunction.h"
#include "../../Effect/BlurImpact.h"

#include "../../Effect/HitEffectObject.h"
#include "../../Ball/Ball.h"

#include "../../Camera/Camera.h"

BaseballState_DamageMotion_Die::BaseballState_DamageMotion_Die(
	BaseballPlayer* pBaseball,
	const Vector3& Damage_vec  //ダメージを受けた方向
	) :
	m_pBaseball(pBaseball),
	m_Damage_vec(Damage_vec)
{

}

void BaseballState_DamageMotion_Die::Enter(BaseballPlayer* b)
{
	//キャラクタ共通ひるみクラスのテニス固有イベントクラス
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
			m_pBaseball->m_Renderer.Update(t);

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
			//何もしない
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
			//カメラ写すフラグをfalseに
			m_pBaseball->m_Params.camera_draw = false;
		}

	private:
		BaseballPlayer*  m_pBaseball;
	};

	//ダメージモーションパラメーターを作成する
	CharacterDamageVanish::Param Param;

	Param.rotate_speed = Vector3(0.0f, 0.0f, 0.0);
	Param.move = Vector3Normalize(m_Damage_vec) * 0.7f;
	Param.move.y = 0.2f;

	Param.down_frame = 15;
	Param.down_muteki_frame = 15;
	Param.standup_frame = 50;
	Param.standup_muteki_frame = 10;


	//ひるみクラスを作成
	m_pDamageVanishClass = new CharacterDamageVanish(
		m_pBaseball,
		Param,
		new BaseballEvent(b),
		new DamageManager::HitEventBase()
		);

	COLORf EffectColor(CharacterBase::GetPlayerColor(b->m_PlayerInfo.number));


	//ヒットエフェクト作成
	new HitEffectObject(
		m_pBaseball->m_Params.pos + Vector3(0, 3, 0) + Vector3Normalize(m_Damage_vec)*3.5f,
		m_Damage_vec,
		0.05f,
		0.15f,
		Vector3(EffectColor.r, EffectColor.g, EffectColor.b),
		5,
		50
		);

	//ブラーエフェクト
	new BlurImpactSphere(
		m_pBaseball->m_Params.pos + Vector3(0, BallBase::UsualBallShotY, 0),
		20,
		50,
		15
		);

	//カメラショック
	DefCamera.SetShock(
		Vector2(1, 1)*0.22f,
		20
		);
}

void BaseballState_DamageMotion_Die::Execute(BaseballPlayer* b)
{
	m_pDamageVanishClass->Update();
}

void BaseballState_DamageMotion_Die::Exit(BaseballPlayer* b)
{
	delete m_pDamageVanishClass;
}
#include "SoccerCommonState.h"
#include "SoccerPlayerState.h"
#include "SoccerHitEvent.h"
#include "../../Sound/Sound.h"

#include "../CharacterFunction.h"

#include "../../Effect/BlurImpact.h"

#include "../../Effect/HitEffectObject.h"

#include "../../Camera/Camera.h"

SoccerState_SmallDamage::SoccerState_SmallDamage(
	SoccerPlayer* pSoccer,
	const Vector3& Damage_vec  //ダメージを受けた方向
	) :
	m_pSoccer(pSoccer),
	m_Damage_vec(Damage_vec)
{

}
void SoccerState_SmallDamage::Enter(SoccerPlayer* s)
{
	//キャラクタ共通ひるみクラスのサッカー固有イベントクラス
	class SoccerEvent :public CharacterDamageMotion::Event
	{
	public:
		SoccerEvent(SoccerPlayer* pSoccer) :m_pSoccer(pSoccer){}

		void Update(float speed)
		{
			//モデルの更新のみ
			m_pSoccer->m_Renderer.Update(speed);
			chr_func::CreateTransMatrix(
				m_pSoccer, 
				m_pSoccer->m_ModelSize,
				&m_pSoccer->m_Renderer.m_TransMatrix);
		}
		void Start()
		{
			//ひるみモーションをセット
			m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_Damage);
		}
		void End()
		{
			//通常ステートをセット
			m_pSoccer->SetState(
				SoccerState_PlayerControll_Move::GetPlayerControllMove(m_pSoccer)
				);
		}
	private:
		SoccerPlayer*  m_pSoccer;
	};

	//ダメージモーションパラメーターを作成する
	CharacterDamageMotion::Params Param;

	Param.AllFrame = 40;
	Param.damage_vec = m_Damage_vec;
	Param.hitBack = 0.5f;
	Param.hitStopFrame = 5;
	
	Param.NoDamageFrame = 10;

	//ひるみクラスを作成
	m_pDamageMotionClass = new CharacterDamageMotion(
		m_pSoccer,
		new SoccerEvent(m_pSoccer),
		new SoccerHitEvent(m_pSoccer),
		Param
		);
	//ヒットエフェクト作成
	new HitEffectObject(
		m_pSoccer->m_Params.pos + Vector3(0, 3, 0),
		m_Damage_vec,
		0.05f,
		0.15f,
		Vector3(1.0f, 1.0f, 1.0f)
		);

	//ブラーエフェクト
	new BlurImpactSphere(
		m_pSoccer->m_Params.pos + Vector3(0, 3, 0),
		10,
		15,
		30
		);

}
void SoccerState_SmallDamage::Execute(SoccerPlayer* t)
{
	m_pDamageMotionClass->Update();
}

void SoccerState_SmallDamage::Exit(SoccerPlayer* t)
{

}
SoccerState_DamageVanish::SoccerState_DamageVanish(
	SoccerPlayer* pSoccer,
	const Vector3& Damage_vec  //ダメージを受けた方向
	) :
	m_pSoccer(pSoccer),
	m_Damage_vec(Damage_vec)
{

}
void SoccerState_DamageVanish::Enter(SoccerPlayer* s)
{
	//キャラクタ共通ひるみクラスのサッカー固有イベントクラス
	class SoccerEvent :public CharacterDamageVanish::Event
	{
	public:
		SoccerEvent(SoccerPlayer* pSoccer) :m_pSoccer(pSoccer), m_Timer(0){}

		void FlyStart()
		{
			//吹き飛びモーションをセット
			m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_Blowing);
		}
		void Flying(const Matrix& Rotate)
		{
			//モデルのアニメーション更新
			m_pSoccer->m_Renderer.Update(1);

			//位置にもとづき、ワールド変換行列を計算
			chr_func::CreateTransMatrix(m_pSoccer, m_pSoccer->m_ModelSize, &m_pSoccer->m_Renderer.m_TransMatrix);

			//吹き飛びの回転を入れる
			m_pSoccer->m_Renderer.m_TransMatrix = Rotate*m_pSoccer->m_Renderer.m_TransMatrix;
		}
		void DownStart()
		{
			//ダウンモーションをセット
			m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_Scrub);
		}

		void Downing()
		{
			m_pSoccer->m_Renderer.Update(1);

			//ワールド変換行列を計算
			chr_func::CreateTransMatrix(m_pSoccer, m_pSoccer->m_ModelSize, &m_pSoccer->m_Renderer.m_TransMatrix);
		}

		void StandUpStart()
		{
			//吹き飛びモーションをセット
			m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_GetUp);
		}

		void StandUping()
		{
			//モデルのアニメーション更新
			m_pSoccer->m_Renderer.Update(1);

			//ワールド変換行列を計算
			chr_func::CreateTransMatrix(m_pSoccer, m_pSoccer->m_ModelSize, &m_pSoccer->m_Renderer.m_TransMatrix);
		}

		void End()
		{
			//通常ステートをセット
			m_pSoccer->SetState(
				SoccerState_PlayerControll_Move::GetPlayerControllMove(m_pSoccer)
				);
		}
	private:
		SoccerPlayer*  m_pSoccer;
		int            m_Timer;
	};

	//ダメージモーションパラメーターを作成する
	CharacterDamageVanish::Param Param;

	Param.rotate_speed = Vector3(0.0f, 0.0f, 0.0);
	Param.move = m_Damage_vec * 0.95f;
	Param.down_frame = 15;
	Param.down_muteki_frame = 15;
	Param.move.y = 0.2f;
	Param.standup_frame = 50;

	//ひるみクラスを作成
	m_pDamageVanishClass = new CharacterDamageVanish(
		m_pSoccer,
		Param,
		new SoccerEvent(s),
		new SoccerHitEvent(s)
		);
	//ヒットエフェクト作成
	new HitEffectObject(
		m_pSoccer->m_Params.pos + Vector3(0, 3, 0),
		m_Damage_vec,
		0.05f,
		0.15f,
		Vector3(1.0f, 1.0f, 1.0f)
		);
	//ブラーエフェクト
	new BlurImpactSphere(
		m_pSoccer->m_Params.pos + Vector3(0, 3, 0),
		25,
		10,
		30
		);

}
void SoccerState_DamageVanish::Execute(SoccerPlayer* s)
{
	m_pDamageVanishClass->Update();
}

void SoccerState_DamageVanish::Exit(SoccerPlayer* s)
{

}

SoccerState_DamageMotion_Die::SoccerState_DamageMotion_Die(
	SoccerPlayer* pSoccer,
	const Vector3& Damage_vec  //ダメージを受けた方向
	) :
	m_pSoccer(pSoccer),
	m_Damage_vec(Damage_vec)
{

}

void SoccerState_DamageMotion_Die::Enter(SoccerPlayer* t)
{
	//キャラクタ共通ひるみクラスのテニス固有イベントクラス
	class TennisEvent :public CharacterDamageVanish::Event
	{
	public:
		TennisEvent(SoccerPlayer* pSoccer) :m_pSoccer(pSoccer){}

		void FlyStart()
		{
			//吹き飛びモーションをセット
			m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_Blowing);
			m_pSoccer->m_Params.camera_draw = false;
		}

		void Flying(const Matrix& Rotate)
		{
			//モデルのアニメーション更新
			m_pSoccer->m_Renderer.Update(1);

			//位置にもとずき、ワールド変換行列を計算
			chr_func::CreateTransMatrix(m_pSoccer, m_pSoccer->m_ModelSize, &m_pSoccer->m_Renderer.m_TransMatrix);

			//吹き飛びの回転を入れる
			m_pSoccer->m_Renderer.m_TransMatrix = Rotate*m_pSoccer->m_Renderer.m_TransMatrix;
		}

		void DownStart()
		{
			//ダウンモーションをセット
			m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_Scrub);
		}

		void Downing()
		{
			m_pSoccer->m_Renderer.Update(1);

			//ワールド変換行列を計算
			chr_func::CreateTransMatrix(m_pSoccer, m_pSoccer->m_ModelSize, &m_pSoccer->m_Renderer.m_TransMatrix);
		}

		void StandUpStart()
		{
			//何もしない
		}

		void StandUping()
		{
			//モデルのアニメーション更新
			m_pSoccer->m_Renderer.Update(1);

			//ワールド変換行列を計算
			chr_func::CreateTransMatrix(m_pSoccer, m_pSoccer->m_ModelSize, &m_pSoccer->m_Renderer.m_TransMatrix);
		}

		void End()
		{
			//カメラ写すフラグをfalseに
			m_pSoccer->m_Params.camera_draw = false;
		}

	private:
		SoccerPlayer*  m_pSoccer;
	};

	//ダメージモーションパラメーターを作成する
	CharacterDamageVanish::Param  Param;

	Param.rotate_speed = Vector3(0.0f, 0.0f, 0.0f);
	Param.move = Vector3Normalize(m_Damage_vec) * 0.7f;
	Param.move.y = 0.2f;

	Param.down_frame = 15;
	Param.down_muteki_frame = 15;
	Param.standup_frame = 50;
	Param.standup_muteki_frame = 10;


	//ひるみクラスを作成
	m_pDamageVanishClass = new CharacterDamageVanish(
		m_pSoccer,
		Param,
		new TennisEvent(t),
		new DamageManager::HitEventBase()
		);

	{
		COLORf EffectColor(CharacterBase::GetPlayerColor(t->m_PlayerInfo.number));

		//ヒットエフェクト作成
		new HitEffectObject(
			m_pSoccer->m_Params.pos + Vector3(0, 3, 0) + Vector3Normalize(m_Damage_vec)*3.5f,
			m_Damage_vec,
			0.05f,
			0.15f,
			Vector3(EffectColor.r, EffectColor.g, EffectColor.b),
			5,
			50
			);
	}

	//ブラーエフェクト
	new BlurImpactSphere(
		m_pSoccer->m_Params.pos + Vector3(0, BallBase::UsualBallShotY, 0),
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

void SoccerState_DamageMotion_Die::Execute(SoccerPlayer* s)
{
	m_pDamageVanishClass->Update();
}

void SoccerState_DamageMotion_Die::Exit(SoccerPlayer* s)
{
	delete m_pDamageVanishClass;
}
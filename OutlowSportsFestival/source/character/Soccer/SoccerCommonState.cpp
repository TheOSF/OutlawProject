#include "SoccerCommonState.h"
#include "SoccerPlayerState.h"
#include "SoccerHitEvent.h"
#include "../../Effect/EffectFactory.h"
#include "../../Sound/Sound.h"

#include "../CharacterFunction.h"
#include "../../Effect/EffectFactory.h"
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

	Param.damage_vec = m_Damage_vec;
	//ひるみクラスを作成
	m_pDamageMotionClass = new CharacterDamageMotion(
		m_pSoccer,
		new SoccerEvent(m_pSoccer),
		new SoccerHitEvent(m_pSoccer),
		Param
		);

    //エフェクト
    EffectFactory::HitEffect(
        m_pSoccer,
        m_Damage_vec
        );

}
void SoccerState_SmallDamage::Execute(SoccerPlayer* t)
{
	m_pDamageMotionClass->Update();
}

void SoccerState_SmallDamage::Exit(SoccerPlayer* t)
{
	delete m_pDamageMotionClass;

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
        void Flying(const Matrix& Rotate, RATIO t)
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
	Param.move = m_Damage_vec;
	Param.down_frame = 15;
	Param.down_muteki_frame = 15;
	Param.standup_frame = 50;

	//ひるみクラスを作成
	m_pDamageVanishClass = new CharacterDamageVanish(
		m_pSoccer,
		Param,
		new SoccerEvent(s),
		new SoccerHitEvent(s)
		);

    //エフェクト
    EffectFactory::VanishEffect(
        m_pSoccer,
        m_Damage_vec
        );

}
void SoccerState_DamageVanish::Execute(SoccerPlayer* s)
{
	m_pDamageVanishClass->Update();
}

void SoccerState_DamageVanish::Exit(SoccerPlayer* s)
{
	delete m_pDamageVanishClass;
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
	//キャラクタ共通ひるみクラスのサッカー固有イベントクラス
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

        void Flying(const Matrix& Rotate, RATIO t)
		{
			//モデルのアニメーション更新
			m_pSoccer->m_Renderer.Update(t);

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

    //エフェクト
    EffectFactory::DieEffect(
        m_pSoccer,
        m_Damage_vec
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

SoccerState_brake::SoccerState_brake(
	SoccerPlayer* pSoccer
	) :
	m_pSoccer(pSoccer)
{

}
void SoccerState_brake::Enter(SoccerPlayer* s)
{
	class SoccerMoveEvent :public CharacterUsualMove::MoveEvent
	{
		SoccerPlayer* m_pSoccer;
	public:
		SoccerMoveEvent(SoccerPlayer* pSoccer) :
			m_pSoccer(pSoccer){}

		//アニメーションの更新
		void Update(bool isRun, RATIO speed_ratio)
		{
			m_pSoccer->m_Renderer.Update(1);
		}
		//走り始めにモーションをセット
		void RunStart()
		{
			m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_Guard);
		}
		//立ちはじめにモーションをセット
		void StandStart()
		{
			m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_Guard);
		}
	};
	CharacterUsualMove::Params p;

	p.Acceleration = 0.0f;
	p.MaxSpeed = 0.2f;
	p.TurnSpeed = 0.0f;
	p.DownSpeed = 0.2f;

	m_pMoveClass = new CharacterUsualMove(
		s,
		p,
		new SoccerMoveEvent(s),
		new SoccerHitEvent(s)
		);
	//初期のたちモーションセット
	s->m_Renderer.SetMotion(SoccerPlayer::_ms_Guard);
	Sound::Play(Sound::Sand2);
	Sound::Play(Sound::Soccer_Brake);

	for (int i = 0; i < 3; ++i)
	{
		EffectFactory::Smoke(
			s->m_Params.pos + Vector3(frand() - 0.5f, frand(), frand() - 0.5f)*2.0f,
			Vector3Zero,
			1.8f,
			0.2f,
			false
			);
	}

}
void SoccerState_brake::Execute(SoccerPlayer* s)
{
	if (s->m_Params.move.Length() <= 0.001f)
	{
		s->SetState(SoccerState_PlayerControll_Move::GetPlayerControllMove(s));
	}
	
		
	m_pMoveClass->Update();

	chr_func::CreateTransMatrix(s, 0.05f, &s->m_Renderer.m_TransMatrix);
}
void SoccerState_brake::Exit(SoccerPlayer* s)
{
	delete m_pMoveClass;
}
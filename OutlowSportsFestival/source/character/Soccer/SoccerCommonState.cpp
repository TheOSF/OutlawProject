#include "SoccerCommonState.h"
#include "SoccerPlayerState.h"
#include "SoccerHitEvent.h"
#include "../../Effect/EffectFactory.h"
#include "../../Sound/Sound.h"

#include "../CharacterFunction.h"
#include "../../Effect/EffectFactory.h"
#include "../../Camera/Camera.h"
#include "../../GameSystem/GameController.h"
#include "SoccerRolling.h"
#include "Computer\SoccerComputerUtilityClass.h"

SoccerState_SmallDamage::SoccerState_SmallDamage(
	SoccerPlayer* pSoccer,
	const Vector3& Damage_vec,  //ダメージを受けた方向
    bool           Counter,
    int            Frame
	) :
	m_pSoccer(pSoccer),
	m_Damage_vec(Damage_vec),
    m_Counter(Counter),
    m_Frame(Frame)
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
    Param.counter_hit = m_Counter;
    Param.frame = m_Frame;

	//ひるみクラスを作成
    m_pDamageMotionClass = new CharacterDamageMotion(
        m_pSoccer,
        new SoccerEvent(m_pSoccer),
        new SoccerHitEvent(m_pSoccer, false), 
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
			m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_Vanish_Fly);
		}
        void Flying(const Matrix& Rotate, RATIO t)
		{
			//モデルのアニメーション更新
			m_pSoccer->m_Renderer.Update(1);

			//位置にもとづき、ワールド変換行列を計算
			chr_func::CreateTransMatrix(m_pSoccer, &m_pSoccer->m_Renderer.m_TransMatrix);

			//吹き飛びの回転を入れる
			m_pSoccer->m_Renderer.m_TransMatrix = Rotate*m_pSoccer->m_Renderer.m_TransMatrix;
		}
		void DownStart()
		{
			//ダウンモーションをセット
			m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_Vanish_Down);
		}

		void Downing()
		{
			m_pSoccer->m_Renderer.Update(1);

			//ワールド変換行列を計算
			chr_func::CreateTransMatrix(m_pSoccer, &m_pSoccer->m_Renderer.m_TransMatrix);
		}

		void StandUpStart()
		{
			//吹き飛びモーションをセット
			m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_StandUp);
		}

		void StandUping()
		{
			//モデルのアニメーション更新
			m_pSoccer->m_Renderer.Update(1);

			//ワールド変換行列を計算
			chr_func::CreateTransMatrix(m_pSoccer, &m_pSoccer->m_Renderer.m_TransMatrix);
		}

		void End()
		{
			//通常ステートをセット
			m_pSoccer->SetState(
				SoccerState_PlayerControll_Move::GetPlayerControllMove(m_pSoccer)
				);
		}

        void HitWall()
        {
            //壁に当たったモーションをセット
            m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_HitWallAndFall);
        }

        void HitFloor()
        {
            //床に当たったモーションをセット
            m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_FallAndDown);
        }

        void HitFloorAndStandUp()
        {
            //立ち上がりモーションをセット
            m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_StandUp);
        }

        void HitWallUpdate()
        {
            //モデルのアニメーション更新
            m_pSoccer->m_Renderer.Update(1);

            //ワールド変換行列を計算
            chr_func::CreateTransMatrix(m_pSoccer, &m_pSoccer->m_Renderer.m_TransMatrix);
        }

        void CanActionUpdate()
        {
            //行動分岐が可能なときに呼ばれる
            if (m_pSoccer->m_PlayerInfo.player_type == PlayerType::_Player)
            {
                if (controller::GetPush(controller::button::batu, m_pSoccer->m_PlayerInfo.number))
                {
                    m_pSoccer->SetState(new SoccerState_Rolling(new SoccerUtillityClass::PlayerRollingControll(m_pSoccer), false));
                }
            }
            else
            {
                if (frand() < 0.02f)
                {
                    m_pSoccer->SetState(new SoccerState_Rolling(new SocceComputerrUtillityClass::ComputerRollingControll(m_pSoccer, -Vector3Normalize(m_pSoccer->m_Params.pos)), false));
                }
            }
        }


	private:
		SoccerPlayer*  m_pSoccer;
		int            m_Timer;
	};

	//ダメージモーションパラメーターを作成する
	CharacterDamageVanish::Param Param;

	Param.rotate_speed = Vector3(0.0f, 0.0f, 0.0);
	Param.move = m_Damage_vec;
	Param.standup_frame = 50;
    Param.down_frame = 10;

	//ひるみクラスを作成
    m_pDamageVanishClass = new CharacterDamageVanish(
        m_pSoccer,
        Param,
        new SoccerEvent(s),
        new SoccerHitEvent(s, false)
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
			m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_Vanish_Fly);
            m_pSoccer->m_DrawObject.m_isDraw = false;
		}

        void Flying(const Matrix& Rotate, RATIO t)
		{
			//モデルのアニメーション更新
			m_pSoccer->m_Renderer.Update(t);

			//位置にもとずき、ワールド変換行列を計算
			chr_func::CreateTransMatrix(m_pSoccer, &m_pSoccer->m_Renderer.m_TransMatrix);

			//吹き飛びの回転を入れる
			m_pSoccer->m_Renderer.m_TransMatrix = Rotate*m_pSoccer->m_Renderer.m_TransMatrix;
		}

		void DownStart()
		{
			//ダウンモーションをセット
			m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_Vanish_Down);
		}

		void Downing()
		{
			m_pSoccer->m_Renderer.Update(1);

			//ワールド変換行列を計算
			chr_func::CreateTransMatrix(m_pSoccer, &m_pSoccer->m_Renderer.m_TransMatrix);
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
			chr_func::CreateTransMatrix(m_pSoccer, &m_pSoccer->m_Renderer.m_TransMatrix);
		}

		void End()
		{
			//カメラ写すフラグをfalseに
            m_pSoccer->m_DrawObject.m_isDraw = false;
		}

        void HitWall()
        {
            //壁に当たったモーションをセット
            m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_HitWallAndFall);
        }

        void HitFloor()
        {
            //床に当たったモーションをセット
            m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_FallAndDown);
        }

        void HitFloorAndStandUp()
        {
            //立ち上がりモーションをセット…しない！
            //m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_StandUp);
        }

        void HitWallUpdate()
        {
            //モデルのアニメーション更新
            m_pSoccer->m_Renderer.Update(1);

            //ワールド変換行列を計算
            chr_func::CreateTransMatrix(m_pSoccer, &m_pSoccer->m_Renderer.m_TransMatrix);
        }

        void CanActionUpdate()
        {
            //行動分岐が可能なときに呼ばれる
            // m_pDoCancelAction->DoAction();
        }

	private:
		SoccerPlayer*  m_pSoccer;
	};

	//ダメージモーションパラメーターを作成する
	CharacterDamageVanish::Param  Param;

	Param.rotate_speed = Vector3(0.0f, 0.0f, 0.0f);
    Param.move = m_Damage_vec;
    Param.down_frame = 10;
	Param.standup_frame = 50;


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
	//初期のたちモーションセット
	s->m_Renderer.SetMotion(SoccerPlayer::_ms_DushStop);

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

	m_Timer = 0;

}
void SoccerState_brake::Execute(SoccerPlayer* s)
{
	++m_Timer;

    if (++m_Timer > 30)
	{
		s->SetState(SoccerState_PlayerControll_Move::GetPlayerControllMove(s));
	}
		
    chr_func::XZMoveDown(s, 0.08f);

    s->m_Renderer.Update(1);

    chr_func::UpdateAll(s, &SoccerHitEvent(s));
    chr_func::CreateTransMatrix(s, &s->m_Renderer.m_TransMatrix);
}
void SoccerState_brake::Exit(SoccerPlayer* s)
{

}

SoccerState_clash::SoccerState_clash(
	SoccerPlayer* pSoccer
	) :
	m_pSoccer(pSoccer)
{

}
void SoccerState_clash::Enter(SoccerPlayer* s)
{
	class SoccerMoveEvent :public CharacterUsualMove::MoveEvent
	{
		SoccerPlayer* m_pSoccer;
	public:
		SoccerMoveEvent(SoccerPlayer* pSoccer) :
			m_pSoccer(pSoccer) {}

		//アニメーションの更新
		void Update(bool isRun, RATIO speed_ratio)
		{
			m_pSoccer->m_Renderer.Update(1);
		}
		//走り始めにモーションをセット
		void RunStart()
		{
			m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_Damage);
		}
		//立ちはじめにモーションをセット
		void StandStart()
		{
			m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_Damage);
		}
        //走り終わり
        void RunEnd()
        {
            m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_RunStop);
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
	s->m_Renderer.SetMotion(SoccerPlayer::_ms_DushStop);
	Sound::Play(Sound::AtkHit1);
	m_count = 0;

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
void SoccerState_clash::Execute(SoccerPlayer* s)
{
	++m_count;
	if (m_count>40)
	{
		s->SetState(SoccerState_PlayerControll_Move::GetPlayerControllMove(s));
	}


	m_pMoveClass->Update();

	chr_func::CreateTransMatrix(s, &s->m_Renderer.m_TransMatrix);
}
void SoccerState_clash::Exit(SoccerPlayer* s)
{
	delete m_pMoveClass;
}
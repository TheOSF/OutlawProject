#include "SoccerPlayerState.h"
#include "SoccerCommonState.h"
#include "SoccerHitEvent.h"
#include "SoccerPlayerState_PoseMotion.h"
#include "SoccerRolling.h"
#include "Snakeshot.h"
#include "../../Effect/EffectFactory.h"
#include "../Soccer/Computer/SoccerComputerMove.h"

#include "../../Effect/SpecialAttackEffect.h"
#include "../CharacterDefaultCounterClass.h"
#include "../../GameSystem/GameController.h"
#include "../CharacterManager.h"
#include "../CharacterFunction.h"
#include "../CharacterBase.h"
#include "../../Sound/Sound.h"
#include "../../Camera/Camera.h"
#include "SoccerAttackState.h"
#include "../CharacterHitEventFunc.h"
#include "SoccerCommonState.h"
#include "SoccerState_ControllVanish.h"

class SoccerUtillityClass
{
public:
	//ローリングの方向制御クラス
	class PlayerRollingControll :public SoccerState_Rolling::CallBackClass
	{
	public:
		SoccerPlayer*const ps;

		PlayerRollingControll(SoccerPlayer* ps) :ps(ps){}


		Vector3 GetVec()override
		{
			Vector2 stick = controller::GetStickValue(controller::stick::left, ps->m_PlayerInfo.number);
			Vector3 vec(stick.x, 0, stick.y);

			if (vec.Length() < 0.25f)
			{
				return Vector3Zero;
			}

			vec = DefCamera.GetRight()*vec.x + DefCamera.GetForward()*vec.z;
			vec.y = 0;
			vec.Normalize();

			return vec;
		}
	};

};
SoccerState* SoccerState_PlayerControll_Move::GetPlayerControllMove(
	SoccerPlayer* ps)
{
	switch (ps->m_PlayerInfo.player_type)
	{
	case PlayerType::_Player:
		return new SoccerState_PlayerControll_Move();

	case PlayerType::_Computer:

		return new SoccerState_ComputerControll_Move();

		switch (ps->m_PlayerInfo.strong_type)
		{
		case StrongType::_Weak:
			//return new 弱い通常移動
		case StrongType::_Usual:
			//return new 弱い通常移動
		case StrongType::_Strong:
			//return new 弱い通常移動
		default:break;
		}
	default:break;
	}

	assert("通常ステートが作成できないキャラクタタイプです SoccerState_PlayerControll_Move::GetPlayerControllMove" && 0);
	return nullptr;
}
bool SoccerState_PlayerControll_Move::SwitchGameState(SoccerPlayer* ps)
{
	Vector3 v;

	switch (ps->GetStateType())
	{
	case CharacterBase::State::Usual:

		return false;

	case CharacterBase::State::Freeze:

		return true;

	case CharacterBase::State::LosePose:
		ps->SetState(new SoccerState_PoseMotion(SoccerPlayer::_ms_Lose, 0.2f, 1000));
		return true;

	case CharacterBase::State::WinPose:
		ps->SetState(new SoccerState_PoseMotion(SoccerPlayer::_ms_Win, 0.2f, 1000));

		return true;
	default:
		break;
	}

	return false;

}

void SoccerState_PlayerControll_Move::ActionStateSwitch(SoccerPlayer*s)
{
	float movelen = s->m_Params.move.Length();
	// [×] で ローリング
	if (controller::GetTRG(controller::button::batu, s->m_PlayerInfo.number))
	{
		s->SetState(new SoccerState_Rolling(new SoccerUtillityClass::PlayerRollingControll(s), false));
	}
	// [□] で 格闘
	if (controller::GetTRG(controller::button::shikaku, s->m_PlayerInfo.number))
	{
        s->SetState(new SoccerAttackState(s));
	}
	// [△] で ショット
	if (controller::GetTRG(controller::button::sankaku, s->m_PlayerInfo.number))
	{
		s->SetState(new SoccerState_PlayerControll_Shot);
	}
	// [〇] で 必殺技
	if (controller::GetTRG(controller::button::maru, s->m_PlayerInfo.number))
	{
		//if (chr_func::isCanSpecialAttack(s))
		{
			s->SetState(new SoccerState_PlayerControll_Finisher);
		}
	}
	// [R1] で カウンター
	if (controller::GetTRG(controller::button::_R1, s->m_PlayerInfo.number))
	{
		s->SetState(new SoccerState_PlayerControll_Counter());
	}
	// [L1] で 固有技(ダッシュ)
	if (controller::GetPush(controller::button::_L1, s->m_PlayerInfo.number) && movelen>0.18f)
	{
		s->SetState(new SoccerState_PlayerControll_Dash());
	}
}
//-------------移動ステートクラス-------------
void SoccerState_PlayerControll_Move::Enter(SoccerPlayer* s)
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
            m_pSoccer->m_Renderer.Update(0.5f);
		}
		//走り始めにモーションをセット
		void RunStart()
		{
			m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_Run);
		}
		//立ちはじめにモーションをセット
		void StandStart()
		{
			m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_Stand);
		}
        
        //走り終わり
        void RunEnd()
        {
            m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_RunStop);
        }
	};

	CharacterUsualMove::Params p;

	p.Acceleration = 0.25f;
	p.MaxSpeed = 0.35f;
	p.TurnSpeed = 0.3f;
	p.DownSpeed = 0.08f;
	p.RunEndFrame = 35;

	m_pMoveClass = new CharacterUsualMove(
		s, 
		p, 
		new SoccerMoveEvent(s),
		new SoccerHitEvent(s)
		);
	//初期のたちモーションセット
	s->m_Renderer.SetMotion(SoccerPlayer::_ms_Stand);
}
void SoccerState_PlayerControll_Move::Execute(SoccerPlayer* s)
{
	if (SwitchGameState(s) == false)
	{
		//各アクションへ移行可能
		ActionStateSwitch(s);
		Vector2 st = controller::GetStickValue(controller::stick::left, s->m_PlayerInfo.number);
		Vector3 st_vec3;

		//ビュー空間に変換
		st_vec3 = DefCamera.GetRight()*st.x + DefCamera.GetForward()*st.y;
		st.x = st_vec3.x;
		st.y = st_vec3.z;

		//スティックの値セット
		m_pMoveClass->SetStickValue(st);

	}
	else
	{
		//スティックの値セット
		m_pMoveClass->SetStickValue(Vector2(0, 0));
	}
	
	//更新
	m_pMoveClass->Update();


	//モデルのワールド変換行列を更新
	chr_func::CreateTransMatrix(s, &s->m_Renderer.m_TransMatrix);
}
void SoccerState_PlayerControll_Move::Exit(SoccerPlayer* s)
{
	delete m_pMoveClass;
}

//-------------スライディングステートクラス-------------

SoccerState_Sliding::SoccerState_Sliding(SoccerPlayer* s) :
m_pChr(s),
m_Timer(0)
{
    //初期化
    m_Damage.m_Enable = false;
    m_Damage.m_VecPower = Vector2(0.45f, 0.0f);
    m_Damage.m_VecType = DamageShpere::DamageVecType::MemberParam;
    m_Damage.pParent = s;
    m_Damage.type = DamageBase::Type::_WeekDamage;
    m_Damage.m_Param.size = 1.5f;

    m_pStateFunc = &SoccerState_Sliding::State_Start;
}

void SoccerState_Sliding::Enter(SoccerPlayer* t)
{
    
}

void SoccerState_Sliding::Execute(SoccerPlayer* t)
{

    (this->*m_pStateFunc)();
    
    SetMoveByAngle();

    //ダメージ位置を更新
    UpdateDamage();

    m_pChr->m_Renderer.Update(1);
    chr_func::CreateTransMatrix(m_pChr, &m_pChr->m_Renderer.m_TransMatrix);
}

void SoccerState_Sliding::Exit(SoccerPlayer* t)
{

}

void SoccerState_Sliding::SetState(StateFunc State)
{
    m_pStateFunc = State;
    m_Timer = 0;
}

void SoccerState_Sliding::UpdateDamage()
{
    {
        //少し前方
        chr_func::GetFront(m_pChr, &m_Damage.m_Param.pos);
        m_Damage.m_Param.pos *= 1.5f;

        //少し上に
        m_Damage.m_Param.pos += Vector3(0, 1.5f, 0);

        //座標をセット
        m_Damage.m_Param.pos += m_pChr->m_Params.pos;
    }

    {
        m_Damage.m_Vec =  chr_func::GetFront(m_pChr);
        m_Damage.m_Vec.Normalize();
    }

}

void SoccerState_Sliding::NoBallDamageUpdate()
{

    class SoccerNoWeakDamageHitEvent :public DamageManager::HitEventBase
    {
        SoccerPlayer* m_pSoccer;
    public:
        SoccerNoWeakDamageHitEvent(SoccerPlayer* ps) :m_pSoccer(ps){}


        bool Hit(DamageBase* pDmg)
        {
            Vector3 DamageVec = Vector3Zero;

            if (pDmg->pBall != nullptr)
            {
                return false;
            }
           

            //ひるみ分岐
            switch (CharacterHitEventFunc::CheckDamage(pDmg, m_pSoccer, &DamageVec))
            {
            case CharacterHitEventFunc::SetType::Die:
                //死亡ステートへ
                m_pSoccer->SetState(new SoccerState_DamageMotion_Die(m_pSoccer, DamageVec), 2);
                break;


            case CharacterHitEventFunc::SetType::Weak_Hit:
                //弱ひるみステートへ
                m_pSoccer->SetState(new SoccerState_SmallDamage(m_pSoccer, DamageVec, false, pDmg->HitMotionFrame), 1);
                break;


            case CharacterHitEventFunc::SetType::Vanish_Hit:
                //吹き飛びステートへ
                m_pSoccer->SetState(new SoccerState_DamageVanish(m_pSoccer, DamageVec), 1);
                break;

            case CharacterHitEventFunc::SetType::Controll_Hit:
            {
                //コントロール吹き飛びステートへ
                SoccerState_ControllVanish* p = new SoccerState_ControllVanish(m_pSoccer);
                m_pSoccer->SetState(p, 2);
                ((DamageControllVanish*)pDmg)->GetDamageControll_Transform()->AddControllClass(p->GetControllClass());
            }
                break;

            case CharacterHitEventFunc::SetType::_None:
                //何もしない(自身のダメージだった場合
                return false;

            default:
                MyAssert(false, "ひるみ分岐ができていません");
                return false;
            }


            return true;
        }
    private:
    };

    chr_func::UpdateAll(m_pChr, &SoccerNoWeakDamageHitEvent(m_pChr));
}

void SoccerState_Sliding::StartAngleControll()
{
   
    if (m_pChr->m_PlayerInfo.player_type == PlayerType::_Player)
    {
        const RADIAN ControllAngle = D3DXToRadian(40);

        Vector2 Stick = controller::GetStickValue(controller::stick::left, m_pChr->m_PlayerInfo.number);
        Vector3 ViewVec(Stick.x, 0, Stick.y);

        chr_func::AngleControll(m_pChr, m_pChr->m_Params.pos + ViewVec, ControllAngle);
    }

    {
        CharacterBase* pTarget = nullptr;

        if (chr_func::CalcAtkTarget(m_pChr, D3DXToRadian(25), 15.0f, &pTarget))
        {
            chr_func::AngleControll(m_pChr, pTarget->m_Params.pos);
        }
    }
}

void SoccerState_Sliding::AutoAngleControll()
{
    const RADIAN AngleSpeed = D3DXToRadian(5);
    CharacterBase* pTarget = nullptr;

    if (chr_func::CalcAtkTarget(m_pChr, D3DXToRadian(20), 8.0f, &pTarget))
    {
        chr_func::AngleControll(m_pChr, pTarget->m_Params.pos, AngleSpeed);
    }
    else 
    {
        if (m_pChr->m_PlayerInfo.player_type == PlayerType::_Player)
        {
            Vector2 Stick = controller::GetStickValue(controller::stick::left, m_pChr->m_PlayerInfo.number);
            Vector3 ViewVec(Stick.x, 0, Stick.y);

            chr_func::AngleControll(m_pChr, m_pChr->m_Params.pos + ViewVec, AngleSpeed);
        }
        else
        {
            if (chr_func::CalcAtkTarget(m_pChr, D3DXToRadian(45), 8.0f, &pTarget))
            {
                chr_func::AngleControll(m_pChr, pTarget->m_Params.pos, AngleSpeed);
            }
        }
    }
}

void SoccerState_Sliding::SetMoveByAngle()
{
    const float L = Vector3XZLength(m_pChr->m_Params.move);

    Vector3 m = chr_func::GetFront(m_pChr);

    m_pChr->m_Params.move.x = m.x*L;
    m_pChr->m_Params.move.z = m.z*L;
}

void SoccerState_Sliding::SetEffect(RATIO alpha)
{
    EffectFactory::SmokeParticle(
        m_pChr->m_Params.pos,
        Vector3(0, 0.025f + frand()*0.025f, 0),
        25,
        1.5f*(frand()*0.5f + 0.5f) ,
        D3DCOLOR_COLORVALUE(1, 1, 1, fClamp(alpha*0.25f , 1, 0))
        );
}

void SoccerState_Sliding::State_Start()
{
    const int AllFrame = 10;
    const int DamageFrame = 5;

    ++m_Timer;

    m_Damage.m_Enable = m_Timer > DamageFrame;

    if (m_Timer == 1)
    {
        m_pChr->m_Renderer.SetMotion(SoccerPlayer::_ms_Sliding_Start);

        StartAngleControll();

        //SE
        Sound::Play(Sound::Sand2);
    }

    if (m_Timer > AllFrame)
    {
        SetState(&SoccerState_Sliding::State_Sliding);
    }

    SetEffect(1);

    NoBallDamageUpdate();
}

void SoccerState_Sliding::State_Sliding()
{
    const int SlideingFrame = 30;
    ++m_Timer;

    AutoAngleControll();

    chr_func::XZMoveDown(m_pChr, 0.025f);

    if (m_Damage.HitCount > 0)
    {
        m_pChr->SetState(new SoccerAttackState(m_pChr, true));
    }

    if (m_Timer > SlideingFrame)
    {
        SetState(&SoccerState_Sliding::State_End);
    }

    SetEffect(1 - ((float)m_Timer / (float)SlideingFrame));

    NoBallDamageUpdate();
}

void SoccerState_Sliding::State_End()
{
    const int AllFrame = 10;


    ++m_Timer;


    m_Damage.m_Enable = false;

    if (m_Timer == 1)
    {
        m_pChr->m_Renderer.SetMotion(SoccerPlayer::_ms_Sliding_End);

        chr_func::XZMoveDown(m_pChr, 0.2f);
    }

    if (m_Timer > AllFrame)
    {
        m_pChr->SetState(SoccerState_PlayerControll_Move::GetPlayerControllMove(m_pChr));
    }

    chr_func::UpdateAll(m_pChr, &SoccerHitEvent(m_pChr));
}

//------------プレイヤー操作の攻撃操作クラス--------------

SoccerState_PlayerControll_Attack::PlayerControllEvent::PlayerControllEvent(SoccerPlayer*const pSoccer) :
m_pSoccer(pSoccer)
{

}

bool SoccerState_PlayerControll_Attack::PlayerControllEvent::isDoCombo()
{
	return controller::GetTRG(controller::button::shikaku, m_pSoccer->m_PlayerInfo.number);
}

void SoccerState_PlayerControll_Attack::PlayerControllEvent::AngleControll(RADIAN angle)
{
	const CharacterBase* const pTargetCharacter = GetFrontTargetEnemy();

	if (pTargetCharacter != nullptr)
	{
		//自動回転
		chr_func::AngleControll(m_pSoccer, pTargetCharacter->m_Params.pos, angle);
	}
	else
	{
		const Vector2 Stick = controller::GetStickValue(controller::stick::left, m_pSoccer->m_PlayerInfo.number);

		//スティックが一定以上倒されているかどうか
		if (Vector2Length(Stick) > 0.25f)
		{
			Vector3 Vec(Stick.x, 0, Stick.y);

			//スティック値をカメラ空間に
			Vec = Vector3MulMatrix3x3(Vec, matView);

			//キャラクタ回転
			chr_func::AngleControll(m_pSoccer, m_pSoccer->m_Params.pos + Vec, angle);
		}
	}
}

const CharacterBase* SoccerState_PlayerControll_Attack::PlayerControllEvent::GetFrontTargetEnemy()
{
	CharacterManager::CharacterMap ChrMap = DefCharacterMgr.GetCharacterMap();

	const float  AutoDistance = 10.0f;               //自動ができる最大距離
	const RADIAN AutoMaxAngle = D3DXToRadian(90);   //自動ができる最大角度

	const CharacterBase* pTargetEnemy = nullptr;    //ターゲット保持のポインタ
	RADIAN MostMinAngle = PI;                       //もっとも狭い角度
	RADIAN TempAngle;

	Vector3 MyFront;      //自身の前方ベクトル
	chr_func::GetFront(m_pSoccer, &MyFront);

	auto it = ChrMap.begin();

	while (it != ChrMap.end())
	{
		//自身を除外
		if (m_pSoccer->m_PlayerInfo.number == it->first->m_PlayerInfo.number ||
			chr_func::isDie(it->first)
			)
		{
			++it;
			continue;
		}

		//距離が一定以上のキャラクタを除外する
		if (Vector3Distance(it->first->m_Params.pos, m_pSoccer->m_Params.pos) > AutoDistance)
		{
			it = ChrMap.erase(it);
			continue;
		}

		//前ベクトルと敵へのベクトルの角度を計算する
		TempAngle = Vector3Radian(MyFront, (it->first->m_Params.pos - m_pSoccer->m_Params.pos));

		//角度が一番狭かったら更新
		if (TempAngle < MostMinAngle)
		{
			pTargetEnemy = it->first;
			MostMinAngle = TempAngle;
		}

		++it;
	}

	return pTargetEnemy;

}

//-------------近距離攻撃ステートクラス-------------
SoccerState_PlayerControll_Attack::SoccerState_PlayerControll_Attack(SoccerPlayer* s) :
m_Attack(s, new PlayerControllEvent(s))
{

}


SoccerState_PlayerControll_Attack::~SoccerState_PlayerControll_Attack()
{

}
void SoccerState_PlayerControll_Attack::Enter(SoccerPlayer* s)
{
	//攻撃クラス作成
	SoccerAttackInfo_UsualAtk* pAtk;

	SoccerAttackInfo_UsualAtk::Param AtkParam[] =
	{
		{ 6, 2.0f, 2.5f, DamageBase::Type::_WeekDamage, 3, 20, 0.06f, 3, 9, SoccerPlayer::_ms_Atk1, 50, 10, 20, 35, 0, 5, D3DXToRadian(5), 21,SoccerPlayer::skillparams.Attack1 },
		{ 6, 2.0f, 2.5f, DamageBase::Type::_WeekDamage, 5, 18, 0.04f, 1, 6, SoccerPlayer::_ms_Atk2, 35, 5, 15, 20, 0, 5, D3DXToRadian(5), 3,SoccerPlayer::skillparams.Attack2 },
		{ 8, 2.0f, 2.5f, DamageBase::Type::_VanishDamage, 8, 16, 0.06f, 1, 7, SoccerPlayer::_ms_Atk3, 40, -1, -1, -1, 0, 8, D3DXToRadian(5), 21 ,SoccerPlayer::skillparams.Attack3 },
	};

	for (int i = 0; i < (int)ARRAYSIZE(AtkParam); ++i)
	{
		pAtk = new SoccerAttackInfo_UsualAtk(s);

		pAtk->m_Param = AtkParam[i];

		m_Attack.m_AttackInfoArray.push_back(pAtk);
	}
}
void SoccerState_PlayerControll_Attack::Execute(SoccerPlayer* s)
{
	m_Attack.Update();

	if (m_Attack.isEnd())
	{
		s->SetState(SoccerState_PlayerControll_Move::GetPlayerControllMove(s));
	}
}
void SoccerState_PlayerControll_Attack::Exit(SoccerPlayer* s)
{
	
}

//-------------射撃攻撃ステートクラス-------------
void SoccerState_PlayerControll_Shot::Enter(SoccerPlayer* s)
{
	class SoccerShotEvent :public CharacterShotAttack::Event
	{
		SoccerPlayer* m_pSoccer;
	public:
		SoccerShotEvent(SoccerPlayer* pSoccer) :
			m_pSoccer(pSoccer){}

		void Update()
		{
		
		}

		void AttackStart()
		{
			m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_Shot);
		}

		void AttackEnd()
		{
			m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_Stand);
		}
		void Shot()
		{
			BallBase::Params param;

			chr_func::GetFront(m_pSoccer, &param.move);
			param.move *= 0.5f;
			param.pos = m_pSoccer->m_Params.pos;
			param.pos.y = 0.4f;
			param.pParent = m_pSoccer;
			param.type = BallBase::Type::_Usual;
			Sound::Play(Sound::Impact2);

            new UsualBall(param, DamageBase::Type::_WeekDamage, 10, UsualBall::GetUsual_ControllHeightMoveControll());
		}
	};

	
	CharacterShotAttack::AttackParams p;

	p.ShotFrame = 20;
	p.AllFrame = 30;
	p.MoveDownSpeed = 0.075f;

	m_pShotClass = new CharacterShotAttack(s, new SoccerShotEvent(s), p, new SoccerHitEvent(s));
}
void SoccerState_PlayerControll_Shot::Execute(SoccerPlayer* s)
{

	
	if (!m_pShotClass->Update())
	{
		s->SetState(SoccerState_PlayerControll_Move::GetPlayerControllMove(s));
	}

	//モデル関連の更新
	s->m_Renderer.Update(1);
	chr_func::CreateTransMatrix(s, &s->m_Renderer.m_TransMatrix);
}
void SoccerState_PlayerControll_Shot::Exit(SoccerPlayer* s)
{
	delete m_pShotClass;
}
//-------------カウンターステートクラス-------------
void SoccerState_PlayerControll_Counter::Enter(SoccerPlayer* s)
{
	//カウンターイベントクラス
	class CounterEvent :public CharacterDefaultCounter::Event
	{
		SoccerPlayer* const m_pSoccer;
	public:
		CounterEvent(SoccerPlayer* pTennis) :
			m_pSoccer(pTennis)
		{

		}

		//構え開始
		void Pose()
		{
			m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_CounterPose);
		}

		//ボールへ移動開始
		void Move(BallBase* pCounterBall)
		{
			//ボールの位置によってモーション分岐
			if (chr_func::isRight(m_pSoccer, pCounterBall->m_Params.pos))
			{
				m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_Counter_Right);
			}
			else
			{
				m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_Counter_Left);
			}
		}

		//打つ
		void Shot(BallBase* pCounterBall)
		{
			chr_func::ResetMove(m_pSoccer);
			chr_func::AddMoveFront(m_pSoccer, -0.15f, 1000);
			
		}

		//打ち失敗
		void ShotFaild()
		{
            m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_Counter_Failed);
		}

		//終了
		void End()
		{
			//通常移動クラスへ
			m_pSoccer->SetState(SoccerState_PlayerControll_Move::GetPlayerControllMove(m_pSoccer));
		}
	};

	//カウンターパラメータ設定
	CharacterDefaultCounter::Param param;

	param.AfterShotFrame = 15;
	param.CanCounterFrame = 30;
	param.CatchAriaSize = 8;
	param.ControllRadian = D3DXToRadian(33);
	param.FailedFrame = 20;
	param.PoseFrame = 60;
	param.ShotFrame = 6;
	param.BallSpeed = 0.85f;

	// カウンタークラス作成
	m_pCounter = new CharacterDefaultCounter(
		s,
		param,
		new CounterEvent(s),
		new SoccerHitEvent(s)
		);
}
void SoccerState_PlayerControll_Counter::Execute(SoccerPlayer* s)
{
	m_pCounter->SetStickValue(controller::GetStickValue(controller::stick::left, s->m_PlayerInfo.number));
	//更新
	m_pCounter->Update();

	//モデルのワールド変換行列を更新
	chr_func::CreateTransMatrix(s, &s->m_Renderer.m_TransMatrix);

	//モデル更新
	s->m_Renderer.Update(1);
}
void SoccerState_PlayerControll_Counter::Exit(SoccerPlayer* s)
{
	delete m_pCounter;
}
//-------------ダッシュステートクラス-------------

void SoccerState_PlayerControll_Dash::Enter(SoccerPlayer* s)
{
	class SoccerDashEvent 
	{
		SoccerPlayer* m_pSoccer;
	public:
		SoccerDashEvent(SoccerPlayer* pSoccer) :
			m_pSoccer(pSoccer){}

		void Update(bool isRun, RATIO speed_ratio)
		{
			m_pSoccer->m_Renderer.Update(1.5f);
		}

		void RunStart()
		{
			m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_Run);
		}

		void StandStart()
		{
			m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_Stand);
		}
	};
	m_timer = 0;
	Sound::Play(Sound::Soccer_Speed_Up1);
	m_pMoveClass = new SoccerDash(
		s
		);
}
void SoccerState_PlayerControll_Dash::Execute(SoccerPlayer* s)
{

	
	++m_timer;
	if (SoccerState_PlayerControll_Move::SwitchGameState(s) == false)
	{
		Vector2 st = controller::GetStickValue(controller::stick::left, s->m_PlayerInfo.number);

		// [L1]離す / カーソル倒さないと戻る
		if (!controller::GetPush(controller::button::_L1, s->m_PlayerInfo.number) || (st.x == 0 && st.y == 0))
		{
			s->SetState(new SoccerState_brake(s));
		}
		else
		{
			m_pMoveClass->SetStickValue(st);
		}
		// [△] で ショット
		if (controller::GetTRG(controller::button::sankaku, s->m_PlayerInfo.number))
		{
			s->SetState(new SoccerState_PlayerControll_Shot);
		}
		// [×] で ローリング
		if (controller::GetTRG(controller::button::batu, s->m_PlayerInfo.number))
		{
			s->SetState(new SoccerState_Rolling(new SoccerUtillityClass::PlayerRollingControll(s), true));
		}
		// [□] で スライディング
		if (controller::GetTRG(controller::button::shikaku, s->m_PlayerInfo.number))
		{
			s->SetState(new SoccerState_Sliding(s));
		}
		// [〇] で 必殺技
		if (controller::GetTRG(controller::button::maru, s->m_PlayerInfo.number))
		{
			if (chr_func::isCanSpecialAttack(s))
			{
				s->SetState(new SoccerState_PlayerControll_Finisher);
			}
		}
		// [R1] で カウンター
		if (controller::GetTRG(controller::button::_R1, s->m_PlayerInfo.number))
		{
			s->SetState(new SoccerState_PlayerControll_Counter());
		}

		if (m_timer % 19 == 5)
		{
			Sound::Play(Sound::Sand1);

		}
		if (m_timer % 3 == 2)
		{
			EffectFactory::Smoke(
				s->m_Params.pos + Vector3(frand() - 0.5f, frand()+0.5f, frand() - 0.5f)*2.0f,
				Vector3Zero,
				1.8f,
				1.0f,
				true
				);
		}

		

	}
	else
	{
		//スティックの値セット
		m_pMoveClass->SetStickValue(Vector2(0, 0));
	}


	
	
	

	
	m_pMoveClass->Update();

	chr_func::CreateTransMatrix(s, &s->m_Renderer.m_TransMatrix);
}
void SoccerState_PlayerControll_Dash::Exit(SoccerPlayer* s)
{
	delete m_pMoveClass;
}

void SoccerState_SmallDamage(SoccerPlayer* s)
{

}
SoccerState_PlayerControll_Finisher::SoccerState_PlayerControll_Finisher() :
m_pSnakeShotClass(nullptr)
{

}
void SoccerState_PlayerControll_Finisher::Enter(SoccerPlayer* s)
{
	m_pSnakeShotClass = this->SnakeShotClass(s);
	
	timeflg = false;
	m_Timer = 0;
	chr_func::ResetSkillGauge(s);
}
void SoccerState_PlayerControll_Finisher::Execute(SoccerPlayer* s)
{
	m_Timer++;

	if (!timeflg)
	{
		//　発動音再生&The World
		if (m_Timer == 1)
		{
			Sound::Play(Sound::Skill);
			
            std::list<LpGameObjectBase> UpdateObjList;

            UpdateObjList.push_back(s);

            DefGameObjMgr.FreezeOtherObjectUpdate(UpdateObjList, 55, true);

			new SpecialAttackEffect(s, 55);
		}

		if (m_Timer >= 55)
		{
			timeflg = true;
			m_Timer = 0;
		}
	}
	else
	{
		m_pSnakeShotClass->SetStickValue(
			controller::GetStickValue(controller::stick::left, s->m_PlayerInfo.number));
	}
	//無敵
	{
		DamageManager::HitEventBase NoDmgHitEvent;   //ノーダメージ

		chr_func::UpdateAll(s, &NoDmgHitEvent);
	}
	
	// 更新
	if (m_pSnakeShotClass->Update() == false)
	{
		return;
	}

}
void SoccerState_PlayerControll_Finisher::Exit(SoccerPlayer* s)
{
	delete m_pSnakeShotClass;
}
//　遠距離クラス
CharacterShotAttack* SoccerState_PlayerControll_Finisher::SnakeShotClass(SoccerPlayer* s){
	class ShotAttackEvent :public CharacterShotAttack::Event{
		SoccerPlayer* m_pSoccer;
		Snakeshot* snake;
	public:
		//　ボール
		BallBase::Params param;
		//　ターゲット
		Vector3 target;
	public:
		//　コンストラクタ
		ShotAttackEvent(SoccerPlayer* pSoccer) :target(0, 0, 0),
			m_pSoccer(pSoccer){}
		//　更新
		void Update()override{
			//　モデル更新
			m_pSoccer->m_Renderer.Update(1.0f);

			// 転送行列更新
			chr_func::CreateTransMatrix(
				m_pSoccer,
				&m_pSoccer->m_Renderer.m_TransMatrix);
		}
	public:
		// ダメージ判定開始 & ボール発射
		void Shot()
		{
			//　遠距離攻撃(param計算)
            Vector3 pos, vec;

            chr_func::GetFront(m_pSoccer, &vec);
			
            pos = m_pSoccer->m_Params.pos;
            pos.y = UsualBall::UsualBallShotY;

			//生成
			Sound::Play(Sound::Beam2);
            new Snakeshot(pos, vec, m_pSoccer, 1);
		}

		//　遠距離攻撃開始
		void AttackStart()override{
			//　☆モーション
            m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_SpecialAtk);
		}

		void AttackEnd()
		{
			//攻撃終了時に通常移動モードに戻る
			m_pSoccer->SetState(SoccerState_PlayerControll_Move::GetPlayerControllMove(m_pSoccer));
		}
	};

	CharacterShotAttack::AttackParams atk;

	atk.AllFrame = 55;
	atk.MaxTurnRadian = PI / 4;
	atk.MoveDownSpeed = 0.3f;
	atk.ShotFrame = 55;

	return m_pSnakeShotClass = new CharacterShotAttack(
		s,
		new ShotAttackEvent(s),
		atk,
		new  DamageManager::HitEventBase()
		);
}



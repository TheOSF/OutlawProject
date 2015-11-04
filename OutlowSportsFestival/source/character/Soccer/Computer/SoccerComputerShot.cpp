#include "../SoccerPlayer.h"
#include "SoccerComputerShot.h"
#include "SoccerComputerMove.h"
#include "../../CharacterManager.h"
#include "../../CharacterFunction.h"
#include "../../../Sound/Sound.h"


void SoccerState_ComputerControll_Shot::Enter(SoccerPlayer* s)
{
	class SoccerShotEvent :public CharacterShotAttack::Event
	{
		SoccerPlayer* m_pSoccer;
	public:
		SoccerShotEvent(SoccerPlayer* pSoccer) :
			m_pSoccer(pSoccer) {}

		void Update()
		{
			m_pSoccer->m_Renderer.Update(1);

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
			param.pos.y = BallBase::UsualBallShotY;
			param.pParent = m_pSoccer;
			param.type = BallBase::Type::_Usual;
			Sound::Play(Sound::Impact2);
			new UsualBall(param, DamageBase::Type::_WeekDamage, 10);
		}
	};

	class SoccerHitEvent :public DamageManager::HitEventBase
	{
	public:
		SoccerHitEvent(SoccerPlayer* ps) :m_pSoccer(ps) {}

		bool Hit(DamageBase* pDmg)
		{
			if (pDmg->pParent == nullptr)
			{
				return false;
			}
			//当たった時に呼ばれる関数(戻り値：当たったかどうか)
			//自分の作っているダメージだった場合は何もしない
			if (pDmg->pParent->m_PlayerInfo.number == m_pSoccer->m_PlayerInfo.number)
			{
				return false;
			}

			//当たった時にそのダメージの種類から、それぞれのステートに派生させる
			switch (pDmg->type)
			{
			case DamageBase::Type::_WeekDamage:
				//弱攻撃
				//m_pSoccer->SetState();
				return true;

				/*
				//未作成
				case DamageBase::Type::_VanishDamage:
				//吹き飛びダメージ
				m_pSoccer->SetState();
				return true;
				case DamageBase::Type::_UpDamage:
				//上に吹き飛び
				m_pSoccer->SetState();
				return true;
				*/

			default:break;
			}

			return false;

		}
	private:
		SoccerPlayer* m_pSoccer;
	};
	CharacterShotAttack::AttackParams p;

	p.ShotFrame = 10;
	p.AllFrame = 35;
	p.MoveDownSpeed = 0.2f;

	m_pShotClass = new CharacterShotAttack(s, new SoccerShotEvent(s), p, new SoccerHitEvent(s));
}
void SoccerState_ComputerControll_Shot::Execute(SoccerPlayer* s)
{
	//ターゲット選定＆向き補正
	
	CharacterManager::CharacterMap ChrMap = DefCharacterMgr.GetCharacterMap();

	const float  AutoDistance = 400.0f;               //自動ができる最大距離
	const RADIAN AutoMaxAngle = D3DXToRadian(90);   //自動ができる最大角度

	const CharacterBase* pTargetEnemy = nullptr;    //ターゲット保持のポインタ
	RADIAN MostMinAngle = PI;                       //もっとも狭い角度
	RADIAN TempAngle;

	Vector3 MyFront;      //自身の前方ベクトル
	chr_func::GetFront(s, &MyFront);

	auto it = ChrMap.begin();

	while (it != ChrMap.end())
	{
		//自身を除外
		if (s->m_PlayerInfo.number == it->first->m_PlayerInfo.number ||
			chr_func::isDie(it->first)
			)
		{
			++it;
			continue;
		}

		//距離が一定以上のキャラクタを除外する
		if (Vector3Distance(it->first->m_Params.pos, s->m_Params.pos) > AutoDistance)
		{
			it = ChrMap.erase(it);
			continue;
		}

		//前ベクトルと敵へのベクトルの角度を計算する
		TempAngle = Vector3Radian(MyFront, (it->first->m_Params.pos - s->m_Params.pos));

		//角度が一番狭かったら更新
		if (TempAngle < MostMinAngle)
		{
			pTargetEnemy = it->first;
			MostMinAngle = TempAngle;
		}

		++it;
	}
	if (pTargetEnemy != nullptr)
	{
		chr_func::AngleControll(s, pTargetEnemy->m_Params.pos, 0.3f);
	}
	if (!m_pShotClass->Update())
	{
		s->SetState(new SoccerState_ComputerControll_Move);
    }

	//基本的な更新
	SoccerHitEvent HitEvent(s);
	chr_func::UpdateAll(s, &HitEvent);
	
	//モデル関連の更新
	s->m_Renderer.Update(1);
	chr_func::CreateTransMatrix(s, 0.05f, &s->m_Renderer.m_TransMatrix);
}
void SoccerState_ComputerControll_Shot::Exit(SoccerPlayer* s)
{
	delete m_pShotClass;
}
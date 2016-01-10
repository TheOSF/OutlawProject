#include "../Computer/SoccerComputerMove.h"
#include "../../Computer/CharacterComputerReactionHitEvent.h"
#include "../../CharacterFunction.h"
#include "../../CharacterMoveClass.h"
#include "SoccerComputerAttack.h"
#include "SoccerComputerDash.h"
#include "../SoccerPlayerState.h"
#include "../SoccerRolling.h"
#include "SoccerComputerShot.h"
#include "SoccerComputerFinisher.h"
#include "SoccerComputerCounter.h"
#include "../SoccerPlayerState_PoseMotion.h"
#include "../../CharacterManager.h"
#include "../../../Camera/Camera.h"
#include "../SoccerHitEvent.h"
#include "../../CharacterManager.h"

#include "../Computer/SoccerComputerUtilityClass.h"



bool SoccerState_ComputerControll_Move::SwitchGameState(SoccerPlayer* ps)
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

//ステート開始
void SoccerState_ComputerControll_Move::Enter(SoccerPlayer* s)
{
    CharacterComputerMove::GetParams(cParam, s->m_PlayerInfo.strong_type);
	Dashpro = 250 - (int)(cParam.ActionFrequence * 100);
	//移動イベントクラス
	class SoccerMoveEvent :public CharacterUsualMove::MoveEvent
	{
		SoccerPlayer* m_cSoccer;
		
	public:
		SoccerMoveEvent(SoccerPlayer* cSoccer) :
			m_cSoccer(cSoccer){}

		//アニメーションの更新
		void Update(bool isRun, RATIO speed_ratio)
		{
			m_cSoccer->m_Renderer.Update(1);
		}
		//走り始めにモーションをセット
		void RunStart()
		{

			m_cSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_Run);
		}
		//立ちはじめにモーションをセット
		void StandStart()
		{
			m_cSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_Stand);
		}
        //走り終わり
        void RunEnd()
        {
            m_cSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_RunStop);
        }
	};

	//移動パラメータを代入
	CharacterUsualMove::Params Movep;

	Movep.Acceleration = 0.25f;
	Movep.MaxSpeed = 0.35f;
	Movep.TurnSpeed = 0.3f;
	Movep.DownSpeed = 0.08f;
	Movep.RunEndFrame = 35;


	//移動コントロールクラスの作成
	m_pMoveControllClass = new CharacterComputerMove(s);

	//移動クラスの作成
	m_pMoveClass = new CharacterUsualMove(
		s,
		Movep,
		new SoccerMoveEvent(s),
		new SoccerHitEvent(s)
		);


	//攻撃イベントクラス
	class SoccerDoEvent :public CharacterComputerDoAction::ActionEvent
	{
		SoccerPlayer* m_cSoccer;
		CharacterComputerMove::Param cParam;
		CharacterComputerMove*  m_pMoveControllClass;
		int AttackPoint;
	public:
		SoccerDoEvent(SoccerPlayer* cSoccer) :
			m_cSoccer(cSoccer) 
		{
			AttackPoint = rand() % 500;
			m_pMoveControllClass->GetParams(cParam, m_cSoccer->m_PlayerInfo.strong_type);
		}

		//アニメーションの更新
		void Attack(float len)override
		{
			if (chr_func::isCanSpecialAttack(m_cSoccer))
			{
				if (SoccerState_ComputerControll_Move::calcTarget(m_cSoccer) == true)
				{
					m_cSoccer->SetState(new SoccerState_ComputerControll_Finisher());
				}
			}
			if (len < 5.0f)
			{
				if (AttackPoint>450)
				{
					const Vector3 avoidvec(0, 0, 0);
					m_cSoccer->SetState(
						new SoccerState_Rolling
						(new SocceComputerrUtillityClass::ComputerRollingControll(m_cSoccer, avoidvec),
							false));
				}
				else if ((cParam.ActionFrequence * 450) > AttackPoint)
				{
					m_cSoccer->SetState(new SoccerState_ComputerControll_Attack(m_cSoccer));
				}
			}
			else if (len < 20.0f)
			{
				if ((cParam.ActionFrequence * 350) > AttackPoint)
				{
					m_cSoccer->SetState(new SoccerState_ComputerControll_Shot);
				}
			}
		}

	};

	//攻撃クラスの作成
	m_pDoActionClass = new CharacterComputerDoAction(
		s, 
		cParam,
		new SoccerDoEvent(s), 
		new SoccerHitEvent(s)
		);

	//反応イベントクラス
	class SoccerReactionEvent :public CharacterComputerReaction::ActionEvent
	{
		Vector3 Vec;
		SoccerPlayer* m_cSoccer;
		CharacterComputerMove::Param cParam;
		CharacterComputerMove*  m_pMoveControllClass;
		int ReactionPoint;
	public:
		SoccerReactionEvent(SoccerPlayer* cSoccer) :
			m_cSoccer(cSoccer)
		{
			m_pMoveControllClass->GetParams(cParam, m_cSoccer->m_PlayerInfo.strong_type);
			ReactionPoint = rand() % 100;
		}

		//アニメーションの更新
		void Reaction(CharacterComputerReactionHitEvent::HitType hittype, Vector3 vec)override
		{
			if (hittype == CharacterComputerReactionHitEvent::HitType::CanCounter)
			{
				if ((cParam.BallCounter * 100) > ReactionPoint)
				{
					m_cSoccer->SetState(new SoccerState_PlayerControll_Counter);
				}
			}
			else
			{
				if ((cParam.BallCounter * 100) > ReactionPoint)
				{
					m_cSoccer->SetState(
						new SoccerState_Rolling
						(new SocceComputerrUtillityClass::ComputerRollingControll(m_cSoccer, vec),
							false));
				}
			}
		}

	};
	//反応クラスの作成
	m_pReactionClass = new CharacterComputerReaction(
		s,
		cParam,
		new SoccerReactionEvent(s)
		);
}


void SoccerState_ComputerControll_Move::Execute(SoccerPlayer* s)
{
	if (SwitchGameState(s) == false)
	{
		m_pMoveClass->SetStickValue(m_pMoveControllClass->SwitchAction(s));
		m_pDoActionClass->Update();
		m_pReactionClass->Update();

		if (rand() %  Dashpro == 0)
		{
			s->SetState(new SoccerState_ComputerControll_Dash(s));
		}
	}
	else
	{
		m_pMoveClass->SetStickValue(Vector2(0,0));
	}
	m_pMoveClass->Update();
	
	
	//モデルのワールド変換行列を更新

	chr_func::CreateTransMatrix(s, s->m_ModelSize, &s->m_Renderer.m_TransMatrix);

}

void SoccerState_ComputerControll_Move::Exit(SoccerPlayer* s)
{
	delete m_pMoveClass;
	delete m_pMoveControllClass; 
	delete m_pDoActionClass;
	delete m_pReactionClass;
}
bool SoccerState_ComputerControll_Move::calcTarget(SoccerPlayer* s)
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
	if (MostMinAngle < .1f)
	{
		//return pTargetEnemy->m_Params.pos;
		return true;
	}
	else
	{
		//return Vector3(0, 0, 0);
		return false;
	}
}

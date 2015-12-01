#include "BaseballPlayerState_ComMove.h"

#include "../Baseball_HitEvent.h"
#include "../BaseballPlayerState_Attack_B.h"
#include "../BaseballPlayerState_Attack_P.h"
#include "../BaseballState_PlayerControll_ShotAttack_B.h"
#include "../BaseballState_PlayerControll_ShotAttack_P.h"
#include "../BaseballState_SPAttack_B.h"
#include "../BaseballState_SPAttack_P.h"
#include "../BaseballState_Change.h"
#include "../BaseballPlayerState_Counter.h"
#include "../BaseballPlayerState.h"
#include "../BaseballState_PlayerControll_Evasion.h"
#include "../BaseballState_PoseMotion.h"

#include "../../../GameSystem/GameController.h"
#include "../../CharacterFunction.h"
#include "../../../Ball/UsualBall.h"
#include "../../../Damage/Damage.h"
#include "../../CharacterCounterClass.h"
#include "../../CharacterMoveClass.h"
#include "../../CharacterShotAttackClass.h"
#include "../../Computer/CharacterComputerReactionHitEvent.h"
#include "../../CharacterManager.h"


class BaseballPlayerComputerrUtillityClass
{
public:
	//ローリングの方向制御クラス
	class ComputerRollingControll :public  BaseballState_Rolling::CallBackClass
	{
	public:
		BaseballPlayer*const cb;
		ComputerRollingControll(BaseballPlayer* pb, Vector3 vec) :cb(cb), stick(vec) {}
		Vector3 stick;


		Vector3 GetVec()override
		{
			Vector3 vec(stick.x, 0, stick.z);

			return vec;

		}
	};

};


bool BaseballPlayerState_ComMove::SwitchGameState(BaseballPlayer* pb)
{
	Vector3 v;

	switch (pb->GetStateType())
	{
	case CharacterBase::State::Usual:

		return false;

	case CharacterBase::State::Freeze:

		return true;

	case CharacterBase::State::LosePose:
		pb->SetState(new BaseballState_PoseMotion(baseball_player::_mb_LosePose, 0.2f, 1000));
		return true;

	case CharacterBase::State::WinPose:
		pb->SetState(new BaseballState_PoseMotion(baseball_player::_mb_WinPose, 0.2f, 1000));

		return true;
	default:
		break;
	}

	return false;


}

//ステート開始
void BaseballPlayerState_ComMove::Enter(BaseballPlayer* b)
{
	//　移動
	doMove(b);
	//　攻撃
	doAction(b);
	//　反応
	doReaction(b);
}


void BaseballPlayerState_ComMove::Execute(BaseballPlayer* b)
{

	if (SwitchGameState(b) == false)
	{
		BaseballPlayer* b2 = b;

		//スティック値をセット
		m_pMoveClass->SetStickValue(m_pMoveControllClass->SwitcAction_Baseball(b, b->getBatterFlg()));
		//　切り替え
		doChange(b);
	}
	if (!chr_func::isDie(b))
	{
		//********
		//　更新
		//********
		//　攻撃
		m_pDoActionClass->Update();
		//　動き
		m_pMoveClass->Update();
		//　反応
		m_pReactionClass->Update();

	}
	//モデルのワールド変換行列を更新
	chr_func::CreateTransMatrix(b, b->m_ModelSize, &b->m_Renderer.m_TransMatrix);


}

void BaseballPlayerState_ComMove::Exit(BaseballPlayer* b)
{
	delete m_pMoveClass;
	delete m_pMoveControllClass;
	delete m_pDoActionClass;
	delete m_pReactionClass;
}

//　移動
void BaseballPlayerState_ComMove::doMove(BaseballPlayer* b)
{
	//移動イベントクラス
	class BaseballMoveEvent :public CharacterUsualMove::MoveEvent
	{
		BaseballPlayer* m_pBaseball;
	public:
		BaseballMoveEvent(BaseballPlayer* pBaseball) :
			m_pBaseball(pBaseball){}

		//アニメーションの更新
		void Update(bool isRun, RATIO speed_ratio)
		{
			m_pBaseball->m_Renderer.Update(1);
		}
		//走り始めにモーションをセット
		void RunStart()
		{
			m_pBaseball->m_Renderer.SetMotion(baseball_player::_mb_Run);
		}
		//立ちはじめにモーションをセット
		void StandStart()
		{
			m_pBaseball->m_Renderer.SetMotion(baseball_player::_mb_Stand);
		}
	};

	//移動パラメータを代入
	CharacterUsualMove::Params p;

	p.Acceleration = 0.2f;
	p.MaxSpeed = 0.2f;
	p.TurnSpeed = 0.3f;
	p.DownSpeed = 0.2f;

	//移動コントロールクラスの作成
	m_pMoveControllClass = new CharacterComputerMove(b);

	//移動クラスの作成
	m_pMoveClass = new CharacterUsualMove(
		b,
		p,
		new BaseballMoveEvent(b),
		new BaseballHitEvent(b)
		);
}

//　攻撃
void BaseballPlayerState_ComMove::doAction(BaseballPlayer* b)
{
	//攻撃イベントクラス
	class BaseballDoEvent :public CharacterComputerDoAction::ActionEvent
	{

		BaseballPlayer* m_cBaseball;
		CharacterComputerMove::Param cParam;
		CharacterComputerMove*  m_pMoveControllClass;
		int AttackPoint;
	public:
		BaseballDoEvent(BaseballPlayer* cBaseball) :
			m_cBaseball(cBaseball) 
		{
			AttackPoint = rand() % 100;
			m_pMoveControllClass->GetParams(cParam, m_cBaseball->m_PlayerInfo.strong_type);
		}

		//アニメーションの更新
		void Attack(float len)override
		{
			if (len == 0){
				return;
			}
			//　実行パターン
			if (m_cBaseball->getBatterFlg())
			{
				//　バッター時
				if (len < 7.0f)
				{
					if ((cParam.ActionFrequence * 100) > AttackPoint)
					{
						//　必殺
						if (chr_func::isCanSpecialAttack(m_cBaseball))
						{
							m_cBaseball->SetState(new BaseballState_SPAttack_B(m_cBaseball));
						}
						else
						{
							m_cBaseball->SetState(new Baseball_PlayerControll_Attack_B(m_cBaseball));
						}
					}

				}
				else if (len >= 6.0f && len < 25.0f)
				{
					if ((cParam.ActionFrequence * 100) > AttackPoint)
					{
						m_cBaseball->SetState(new BaseballState_PlayerControll_ShotAttack_B(new PlayerShotControllClass_B(m_cBaseball)));
					}
				}
			}
			else{
				if (chr_func::isCanSpecialAttack(m_cBaseball))
				{
					m_cBaseball->SetState(new BaseballState_SPAttack_P());
				}
				//　投手時
				if (len < 6.0f)
				{
					if ((cParam.ActionFrequence * 100) > AttackPoint)
					{
						m_cBaseball->SetState(new Baseball_PlayerControll_Attack_P(m_cBaseball));
					}
				}
				else if (len >= 6.0f && len < 35.0f)
				{
					if ((cParam.ActionFrequence * 100) > AttackPoint)
					{
						m_cBaseball->SetState(new BaseballState_PlayerControll_ShotAttack_P());
					}
				}
			}

		}
	};

	CharacterComputerMove::Param cParam;
	m_pMoveControllClass->GetParams(cParam, b->m_PlayerInfo.strong_type);
	//攻撃クラスの作成
	m_pDoActionClass = new CharacterComputerDoAction(
		b,
		cParam,
		new BaseballDoEvent(b),
		new BaseballHitEvent(b)
		);

}


//　切り替え
void BaseballPlayerState_ComMove::doChange(BaseballPlayer* b)
{
	if (!chr_func::isDie(b))
	{
		nearpos = GetNearTargetPos(b) - b->m_Params.pos;

		//　ターゲットと一定距離以下・以上なら切り替え
		if (nearpos.Length() < 15.0f && !b->getBatterFlg() ||
			nearpos.Length() > 34.0f && b->getBatterFlg())
		{
			b->SetState(new BaseballState_Change());
		}
	}

}

//　反応
void  BaseballPlayerState_ComMove::doReaction(BaseballPlayer* b)
{
	//反応イベントクラス
	class  BaseballPlayerReactionEvent :public CharacterComputerReaction::ActionEvent
	{
		Vector3 Vec;
		BaseballPlayer* m_cBaseball;
		CharacterComputerMove::Param cParam;
		CharacterComputerMove*  m_pMoveControllClass;
		int ReactionPoint;
	public:
		BaseballPlayerReactionEvent(BaseballPlayer* cBaseball) :
			m_cBaseball(cBaseball)
		{
			m_pMoveControllClass->GetParams(cParam, m_cBaseball->m_PlayerInfo.strong_type);
			ReactionPoint = rand() % 100;

		}

		//アニメーションの更新
		void Reaction(CharacterComputerReactionHitEvent::HitType hittype, Vector3 vec)override
		{
			int rnd = rand() % 10;
			//　遠距離攻撃なら
			if (hittype == CharacterComputerReactionHitEvent::HitType::CanCounter)
			{
				if ((cParam.BallCounter * 100) > ReactionPoint)
				{
				//　実行パターン
					if (m_cBaseball->getBatterFlg()){
						//　7：3の割合になるように
						if (rnd < 3)
						{
							//　バッター時
							m_cBaseball->SetState(new BaseballState_PlayerControll_Counter(5));
						}
						else
						{
							m_cBaseball->SetState(new BaseballState_Rolling(new BaseballPlayerComputerrUtillityClass::ComputerRollingControll(m_cBaseball, vec)));
						}
					}
				}
				else
				{
					if ((cParam.BallCounter * 100) > ReactionPoint)
					{
						//　7：3の割合になるように
						if (rnd >= 3)
						{
							//　投手時
							m_cBaseball->SetState(new BaseballState_PlayerControll_Counter(9));
						}
						else
						{
							m_cBaseball->SetState(new BaseballState_Rolling(new BaseballPlayerComputerrUtillityClass::ComputerRollingControll(m_cBaseball, vec)));
						}
					}
				}
			}
			//　それ以外
			else
			{
				m_cBaseball->SetState(
					new BaseballState_Rolling(new BaseballPlayerComputerrUtillityClass::ComputerRollingControll(m_cBaseball, vec)));
			}
		}

	};

	//反応クラスの作成
	CharacterComputerMove::Param cParam;
	m_pMoveControllClass->GetParams(cParam, b->m_PlayerInfo.strong_type);
	m_pReactionClass = new CharacterComputerReaction(
		b,
		cParam,
		new BaseballPlayerReactionEvent(b)
		);
}

//　一番近いキャラのposをとる
Vector3 BaseballPlayerState_ComMove::GetNearTargetPos(BaseballPlayer* b)
{
	Vector3 v;

	float MostNear = 1000;
	float TempLen;
	CharacterBase* pTarget = nullptr;

	//　map代入
	const CharacterManager::CharacterMap& chr_map =
		DefCharacterMgr.GetCharacterMap();

	for (auto it = chr_map.begin(); it != chr_map.end(); ++it)
	{

		//　死んでるor自分ならcontinue
		if (chr_func::isDie(it->first) ||
			it->first->m_PlayerInfo.number == b->m_PlayerInfo.number)
		{
			continue;
		}

		v = it->first->m_Params.pos - b->m_Params.pos;
		v.y = 0;

		TempLen = v.Length();

		if (MostNear > TempLen)
		{
			MostNear = TempLen;
			pTarget = it->first;
		}
	}

	if (pTarget == nullptr)
	{
		return Vector3Zero;
	}

	return pTarget->m_Params.pos;
}
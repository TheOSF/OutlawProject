#include "BaseballPlayerState_ComMove.h"
#include "../Baseball_HitEvent.h"
#include "../BaseballPlayerState_Attack_B.h"
#include "../BaseballPlayerState_Attack_P.h"
#include "../BaseballState_PlayerControll_ShotAttack_B.h"
#include "../BaseballState_PlayerControll_ShotAttack_P.h"
#include "../BaseballState_Change.h"
#include "../BaseballPlayerState_Counter.h"
#include "../BaseballPlayerState.h"
#include "../BaseballState_PlayerControll_Evasion.h"

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

		ComputerRollingControll(BaseballPlayer* pb) :cb(cb) {}


		Vector3 GetVec()override
		{
			Vector2 stick = Vector2(1, 1);
			//Vector2 stick = chr_func::GetRight(cs,);
			Vector3 vec(stick.x, 0, stick.y);

			if (vec.Length() < 0.25f)
			{
				return Vector3Zero;
			}

			vec = Vector3MulMatrix3x3(vec, matView);
			vec.Normalize();

			return vec;
		}
	};

};




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
	//スティック値をセット
	m_pMoveClass->SetStickValue(m_pMoveControllClass->SwitcAction_Baseball(b, b->getBatterFlg()));

	//********
	//　更新
	//********
	//　動き
	m_pMoveClass->Update();
	//　攻撃
	m_pDoActionClass->Update();
	//　反応
	m_pReactionClass->Update();
	//　切り替え
	doChange(b);
	
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
	public:
		BaseballDoEvent(BaseballPlayer* cBaseball) :
			m_cBaseball(cBaseball) {}

		//アニメーションの更新
		void Attack(float len)override
		{
			//　実行パターン
			if (m_cBaseball->getBatterFlg()){
				//　バッター時
				if (len < 6.0f)
				{
					m_cBaseball->SetState(new Baseball_PlayerControll_Attack_B(m_cBaseball));
				}
				else if ( len < 20.0f)
				{
					m_cBaseball->SetState(new BaseballState_PlayerControll_ShotAttack_B(new PlayerShotControllClass_B(m_cBaseball)));
				}
			}
			else{
				//　投手時
				if (len < 5.0f)
				{
					m_cBaseball->SetState(new Baseball_PlayerControll_Attack_P(m_cBaseball));
				}
				else if (len < 30.0f)
				{
					m_cBaseball->SetState(new BaseballState_PlayerControll_ShotAttack_P());
				}
			}

		}
	};

	CharacterComputerMove::Param cParam;
	m_pMoveControllClass->GetParams(cParam, b->m_PlayerInfo.strong_type);
	//攻撃クラスの作成
	m_pDoActionClass = new CharacterComputerDoAction(b, cParam, new BaseballDoEvent(b), new BaseballHitEvent(b));

}


//　切り替え
void BaseballPlayerState_ComMove::doChange(BaseballPlayer* b)
{

	nearpos = m_pMoveControllClass->GetMoveTargetPos(b) - b->m_Params.pos;
	
	//　ターゲットと一定距離以下・以上なら切り替え
	if (nearpos.Length() < 15.0f && !b->getBatterFlg() ||
		nearpos.Length() > 36.0f && b->getBatterFlg())
	{
		b->SetState(new BaseballState_Change());
	}

}

//　反応
void  BaseballPlayerState_ComMove::doReaction(BaseballPlayer* b)
{
	//反応イベントクラス
	class  BaseballPlayerReactionEvent :public CharacterComputerReaction::ActionEvent
	{

		BaseballPlayer* m_cBaseball;
	public:
		BaseballPlayerReactionEvent(BaseballPlayer* cBaseball) :
			m_cBaseball(cBaseball) {}

		//アニメーションの更新
		void Reaction(CharacterComputerReactionHitEvent::HitType hittype, Vector3 vec)override
		{
			//　遠距離攻撃なら
			if (hittype == CharacterComputerReactionHitEvent::HitType::CanCounter)
			{
				//　実行パターン
				if (m_cBaseball->getBatterFlg()){
					//　バッター時
					m_cBaseball->SetState(new BaseballState_PlayerControll_Counter(5));
				}
				else
				{
					//　投手時
					m_cBaseball->SetState(new BaseballState_PlayerControll_Counter(9));
				}
			}
			//　それ以外
			else
			{
				m_cBaseball->SetState(new BaseballState_Rolling(new BaseballPlayerComputerrUtillityClass::ComputerRollingControll(m_cBaseball)));
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

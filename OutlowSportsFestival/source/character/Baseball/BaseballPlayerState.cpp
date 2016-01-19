#include "BaseballPlayerState.h"


#include "BaseballState_PlayerControll_Evasion.h"
#include "BaseballState_PlayerControll_ShotAttack_B.h"
#include "BaseballState_PlayerControll_ShotAttack_P.h"
#include "BaseballPlayerState_Attack_B.h"
#include "BaseballPlayerState_Attack_P.h"
#include "BaseballPlayerState_Counter.h"
#include "BaseballState_SPAttack_B.h"
#include "BaseballState_SPAttack_P.h"
#include "BaseballState_PoseMotion.h"
#include "BaseballState_Change.h"
#include "Baseball_HitEvent.h"

#include "Computer/BaseballPlayerState_ComMove.h"

#include "../../GameSystem/GameController.h"
#include "../CharacterFunction.h"
#include "../CharacterManager.h"
#include "../CharacterEvasionClass.h"

#include "../../Ball/UsualBall.h"
#include "../../Ball/MilderHoming.h"
#include "../../Damage/Damage.h"
#include "../../Camera/Camera.h"

class BallBall_Utillity
{
public:
	//ローリングの方向制御クラス
	class PlayerRollingControll :public BaseballState_Rolling::CallBackClass
	{
	public:
		BaseballPlayer*const pb;

		PlayerRollingControll(BaseballPlayer* pb) :pb(pb){}


		Vector3 GetVec()override
		{
			Vector2 stick = controller::GetStickValue(controller::stick::left, pb->m_PlayerInfo.number);
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




//ショット中のコントロールクラス

Vector3 PlayerShotControllClass_B::GetVec()
{
	Vector2 stick = controller::GetStickValue(controller::stick::left, m_pBaseball->m_PlayerInfo.number);
	Vector3 vec(stick.x, 0, stick.y);

	if (vec.Length() < 0.25f)
	{
		return Vector3Zero;
	}

	vec = DefCamera.GetRight()*vec.x + DefCamera.GetUp()*vec.z;
	vec.Normalize();

	return vec;
}

bool PlayerShotControllClass_B::DoOtherAction()
{
	BaseballPlayer * const b = m_pBaseball;

	//　近距離攻撃[□]
	if (controller::GetTRG(controller::button::shikaku, b->m_PlayerInfo.number))
	{
		b->SetState(new Baseball_PlayerControll_Attack_B(b));
		return true;
	}
	//　回避行動[×]
	if (controller::GetTRG(controller::button::batu, b->m_PlayerInfo.number))
	{
		b->SetState(new BaseballState_Rolling(new BallBall_Utillity::PlayerRollingControll(b)));
		return true;
	}

	// カウンター[R1]
	if (controller::GetTRG(controller::button::_R1, b->m_PlayerInfo.number))
	{
		b->SetState(new  BaseballState_PlayerControll_Counter(5));
		return true;
	}

	return false;
}

//　コンピューター
bool PlayerShotControllClass_B::DoOtherAction_Com()
{
	BaseballPlayer * const b = m_pBaseball;

	b->SetState(new BaseballState_Rolling(new BallBall_Utillity::PlayerRollingControll(b)));
	return true;

}

bool PlayerShotControllClass_B::DoShotAfterAction()
{
	BaseballPlayer * const b = m_pBaseball;

	if (controller::GetTRG(controller::button::shikaku, b->m_PlayerInfo.number))
	{// [□] で [近距離攻撃]
		b->SetState(new Baseball_PlayerControll_Attack_B(b));
		return true;
	}

	return false;
}




//***************************************
//　移動
//***************************************

BaseballState* BaseballState_PlayerControll_Move::GetPlayerControllMove(BaseballPlayer* pt)
{
	switch (pt->m_PlayerInfo.player_type)
	{
	case PlayerType::_Player:
		return new BaseballState_PlayerControll_Move();

	case PlayerType::_Computer:
		return new BaseballPlayerState_ComMove();
		switch (pt->m_PlayerInfo.strong_type)
		{
		case StrongType::_Weak:
			//return new 弱い通常移動
		case StrongType::_Usual:
			//return new 弱い通常移動
		case StrongType::_Strong:
			
		default:break;
		}
	default:break;
	}

	assert("通常ステートが作成できないキャラクタタイプです BaseballState_PlayerControll_Move::GetPlayerControllMove" && 0);
	return nullptr;
}

bool BaseballState_PlayerControll_Move::SwitchGameState(BaseballPlayer* pb)
{
	Vector3 v;

	switch (pb->GetStateType())
	{
	case CharacterBase::State::Usual:

		return false;

	case CharacterBase::State::Freeze:

		return true;

	case CharacterBase::State::LosePose:
		pb->SetState(new BaseballState_PoseMotion(baseball_player::_mb_Stand_B, 0.2f, 1000));
		return true;

	case CharacterBase::State::WinPose:
		pb->SetState(new BaseballState_PoseMotion(baseball_player::_mb_Stand_P, 0.2f, 1000));

		return true;
	default:
		break;
	}

	return false;


}
//　ステート開始
void BaseballState_PlayerControll_Move::Enter(BaseballPlayer* b)
{
	//　移動イベントクラス(ローカルクラス)
	class BaseballMoveEvent :public CharacterUsualMove::MoveEvent
	{
		BaseballPlayer* m_pBaseball;
	public:
		BaseballMoveEvent(BaseballPlayer* pBaseball) :
			m_pBaseball(pBaseball){}

		//　アニメーション更新
		void Update(bool isRun, RATIO speed_ratio){
			m_pBaseball->m_Renderer.Update(1);
		}
		//　走り始めにモーションセット
		void RunStart(){
			//　実行パターン
			if (m_pBaseball->getBatterFlg())
			{
				m_pBaseball->m_Renderer.SetMotion(baseball_player::_mb_Run_B);
			}
			else
			{
				m_pBaseball->m_Renderer.SetMotion(baseball_player::_mb_Run_P);
			}
		}
		//　立ち始めにモーションセット
		void StandStart(){
			if (m_pBaseball->getBatterFlg())
			{
				m_pBaseball->m_Renderer.SetMotion(baseball_player::_mb_Stand_B);
			}
			else
			{
				m_pBaseball->m_Renderer.SetMotion(baseball_player::_mb_Stand_P);
			}
		}
        //走り終わり
        void RunEnd()
        {
            if (m_pBaseball->getBatterFlg())
            {
                m_pBaseball->m_Renderer.SetMotion(baseball_player::_mb_Stop_B);
            }
            else
            {
                m_pBaseball->m_Renderer.SetMotion(baseball_player::_mb_Stop_P);
            }
        }
	};

	//　移動パラメータを代入
	CharacterUsualMove::Params p;

	p.Acceleration = 0.15f;
	p.MaxSpeed = 0.32f;
	p.TurnSpeed = 0.3f;
	p.DownSpeed = 0.08f;
	p.RunEndFrame = 35;

	//　移動クラスの作成
	m_pMoveClass = new CharacterUsualMove(
		b,
		p,
		new BaseballMoveEvent(b),
		new BaseballHitEvent(b)
		);


	
}


//　ステート実行
void BaseballState_PlayerControll_Move::Execute(BaseballPlayer* b)
{

	if (SwitchGameState(b) == false)
	{
		//　スティックの値を取得
		Vector2 st = controller::GetStickValue(controller::stick::left, b->m_PlayerInfo.number);

		//　切り替え

		SetBatterFlg(b);
		//　実行パターン
		if (batterflg){
			//　バッター時
			Batter(b);
		}
		else{
			//　投手時
			Pitcher(b);
		}
		//　スティックの値セット
		m_pMoveClass->SetStickValue(st);
	}
	else
	{
		//スティックの値セット
		m_pMoveClass->SetStickValue(Vector2(0, 0));
	}

	//　更新
	m_pMoveClass->Update();
	//　モデルのワールド変換行列を更新
	chr_func::CreateTransMatrix(b, &b->m_Renderer.m_TransMatrix);

}

//　ステートの終了
void BaseballState_PlayerControll_Move::Exit(BaseballPlayer* b)
{
	delete m_pMoveClass;
}

//　バッター時
void BaseballState_PlayerControll_Move::Batter(BaseballPlayer* b){
	//　遠距離攻撃[△]
	if (controller::GetTRG(controller::button::sankaku, b->m_PlayerInfo.number)){
		b->SetState(new BaseballState_PlayerControll_ShotAttack_B(new PlayerShotControllClass_B(b)));
		return;
	}
	//　近距離攻撃[□]
	if (controller::GetTRG(controller::button::shikaku, b->m_PlayerInfo.number)){
		b->SetState(new Baseball_PlayerControll_Attack_B(b));
		return;
	}
	//　回避行動[×]
	if (controller::GetTRG(controller::button::batu, b->m_PlayerInfo.number)){
		b->SetState(new BaseballState_Rolling(new BallBall_Utillity::PlayerRollingControll(b)));
		return;
	}



	//　必殺打てるなら
    if (chr_func::isCanSpecialAttack(b))
    {
        //　必殺技[○]
        if (controller::GetTRG(controller::button::maru, b->m_PlayerInfo.number)){
            b->SetState(new BaseballState_SPAttack_B(b));
            return;
        }
    }
	// カウンター[R1]
	if (controller::GetTRG(controller::button::_R1, b->m_PlayerInfo.number))
	{
		b->SetState(new  BaseballState_PlayerControll_Counter(5));
		return;
	}
	// 切り替え[L1]
	if (controller::GetTRG(controller::button::_L1, b->m_PlayerInfo.number))
	{
		MyDebugString("Start=%3.3f \n", b->m_Params.move.Length());
		b->SetState(new BaseballState_Change());
		return;
	}
}

//　投手時
void  BaseballState_PlayerControll_Move::Pitcher(BaseballPlayer* b){
	//　遠距離攻撃[△]
	if (controller::GetTRG(controller::button::sankaku, b->m_PlayerInfo.number)){
		b->SetState(new BaseballState_PlayerControll_ShotAttack_P());
		return;
	}
	//　近距離攻撃[□]
	if (controller::GetTRG(controller::button::shikaku, b->m_PlayerInfo.number)){
		b->SetState(new Baseball_PlayerControll_Attack_P(b));
		return;
	}
	//　回避行動[×]
	if (controller::GetTRG(controller::button::batu, b->m_PlayerInfo.number)){
		b->SetState(new BaseballState_Rolling(new BallBall_Utillity::PlayerRollingControll(b)));
		return;
	}

	//　必殺打てるなら
    if (chr_func::isCanSpecialAttack(b))
    {
        //　必殺技[○]
        if (controller::GetTRG(controller::button::maru, b->m_PlayerInfo.number)){
            b->SetState(new BaseballState_SPAttack_P());
            return;
        }
    }



	// カウンター[R1]
	if (controller::GetTRG(controller::button::_R1, b->m_PlayerInfo.number))
	{
		b->SetState(new  BaseballState_PlayerControll_Counter(9));
		return;
	}
	// 切り替え[L1]
	if (controller::GetTRG(controller::button::_L1, b->m_PlayerInfo.number))
	{
		MyDebugString("Start=%3.3f \n", b->m_Params.move.Length());
		b->SetState(new BaseballState_Change());
		return;
	}
}

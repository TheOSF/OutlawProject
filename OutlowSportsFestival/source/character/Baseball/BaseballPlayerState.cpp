#include "BaseballPlayerState.h"

#include "BaseballState_PlayerControll_Evasion.h"
#include "BaseballState_PlayerControll_ShotAttack_B.h"
#include "BaseballState_PlayerControll_ShotAttack_P.h"
#include "BaseballPlayerState_Attack_B.h"
#include "BaseballPlayerState_Attack_P.h"
#include "BaseballPlayerState_Counter.h"
#include "BaseballState_SPAttack_B.h"
#include "BaseballState_SPAttack_P.h"
#include "BaseballState_Change.h"

#include "Baseball_HitEvent.h"
#include "../../GameSystem/GameController.h"
#include "../CharacterFunction.h"
#include "../CharacterManager.h"
#include "../CharacterEvasionClass.h"

#include "../../Ball/UsualBall.h"
#include "../../Ball/MilderHoming.h"
#include "../../Damage/Damage.h"
#include "../CharacterCounterClass.h"
#include "../../Camera/Camera.h"

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

		vec = Vector3MulMatrix3x3(vec, matView);
		vec.Normalize();

		return vec;
	}
};


//ショット中のコントロールクラス
class PlayerShotControllClass_B :public BaseballState_PlayerControll_ShotAttack_B::ControllClass
{
	BaseballPlayer* const   m_pBaseball;
public:
	PlayerShotControllClass_B(BaseballPlayer* pBaseball) :
		m_pBaseball(pBaseball){}

	Vector3 GetVec()
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

	bool DoOtherAction()
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
			b->SetState(new BaseballState_Rolling(new PlayerRollingControll(b)));
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

	bool DoShotAfterAction()
	{
		BaseballPlayer * const b = m_pBaseball;

		if (controller::GetTRG(controller::button::shikaku, b->m_PlayerInfo.number))
		{// [□] で [近距離攻撃]
			b->SetState(new Baseball_PlayerControll_Attack_B(b));
			return true;
		}

		return false;
	}
};



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
		return new BaseballState_PlayerControll_Move();
		switch (pt->m_PlayerInfo.strong_type)
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

	assert("通常ステートが作成できないキャラクタタイプです BaseballState_PlayerControll_Move::GetPlayerControllMove" && 0);
	return nullptr;
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
			m_pBaseball->m_Renderer.SetMotion(baseball_player::_mb_Run);
		}
		//　立ち始めにモーションセット
		void StandStart(){
			m_pBaseball->m_Renderer.SetMotion(baseball_player::_mb_Stand);
		}
	};

	//　ダメージイベントクラスの作成
	class BaseballDamageHitEvent :public DamageManager::HitEventBase
	{
		BaseballPlayer* m_pBaseball;
	public:
		BaseballDamageHitEvent(BaseballPlayer* pBaseball) :
			m_pBaseball(pBaseball){}

		//当たった時にそのダメージの種類から、それぞれのステートに派生させる
		bool Hit(DamageBase* pDmg)
		{
			bool ret = true;

			switch (pDmg->type)
			{
			case DamageBase::Type::_WeekDamage:
				//m_pTennis->SetState();	ステートができていないため
				break;
			case DamageBase::Type::_VanishDamage:
				//m_pTennis->SetState();
				break;
			case DamageBase::Type::_UpDamage:
				//m_pTennis->SetState();
				break;
			default:
				ret = false;
				break;
			}

			return ret;
		}
	};

	//　移動パラメータを代入
	CharacterUsualMove::Params p;

	p.Acceleration = 0.2f;
	p.MaxSpeed = 0.2f;
	p.TurnSpeed = 0.3f;
	p.DownSpeed = 0.2f;

	//　移動クラスの作成
	m_pMoveClass = new CharacterUsualMove(
		b,
		p,
		new BaseballMoveEvent(b),
		new BaseballHitEvent(b)
		);
}


//　ステート実行
void BaseballState_PlayerControll_Move::Execute(BaseballPlayer* b){

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


	//　更新
	m_pMoveClass->Update();
	//　モデルのワールド変換行列を更新
	chr_func::CreateTransMatrix(b, b->m_ModelSize, &b->m_Renderer.m_TransMatrix);

}

//　ステートの終了
void BaseballState_PlayerControll_Move::Exit(BaseballPlayer* b){
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
		b->SetState(new BaseballState_Rolling(new PlayerRollingControll(b)));
		return;
	}
	//　必殺技[○]
	if (controller::GetTRG(controller::button::maru, b->m_PlayerInfo.number)){
		b->SetState(new BaseballState_SPAttack_B(b));
		return;
	}
	// カウンター[R1]
	if (controller::GetTRG(controller::button::_R1, b->m_PlayerInfo.number))
	{
		b->SetState(new  BaseballState_PlayerControll_Counter(5));
		return ;
	}
	// 切り替え[L1]
	if (controller::GetTRG(controller::button::_L1, b->m_PlayerInfo.number))
	{
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
		b->SetState(new BaseballState_Rolling(new PlayerRollingControll(b)));
		return;
	}
	//　必殺技[○]
	if (controller::GetTRG(controller::button::maru, b->m_PlayerInfo.number)){
		b->SetState(new BaseballState_SPAttack_P());
		return;
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
		b->SetState(new BaseballState_Change());
		return;
	}
}

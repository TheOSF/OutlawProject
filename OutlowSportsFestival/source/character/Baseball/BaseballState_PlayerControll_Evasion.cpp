
#include "BaseballPlayerState.h"
#include "BaseballState_PlayerControll_Evasion.h"
#include "Baseball_HitEvent.h"
#include "../../GameSystem/GameController.h"
#include "../CharacterFunction.h"
#include "../CharacterManager.h"
#include "../CharacterEvasionClass.h"

#include "../../Damage/Damage.h"
#include "../CharacterCounterClass.h"


//***************************************
//　回避
//***************************************

//　コンストラクタ
BaseballState_PlayerControll_Evasion::BaseballState_PlayerControll_Evasion(float speed) :
m_pEvasionClass(nullptr)
{
	roolspeed = speed;
}

//　ステート開始
void BaseballState_PlayerControll_Evasion::Enter(BaseballPlayer* b){
	// 回避クラス作成
	m_pEvasionClass = this->CreateEvasionClass(b);
}


//　ステート実行
void BaseballState_PlayerControll_Evasion::Execute(BaseballPlayer* b){
	// スティックの値セット
	m_pEvasionClass->SetStickValue(
		controller::GetStickValue(controller::stick::left, b->m_PlayerInfo.number));

	// 更新
	if (m_pEvasionClass->Update() == false)
	{
		return;
	}
}

//　ステート終了
void BaseballState_PlayerControll_Evasion::Exit(BaseballPlayer* b){
	delete m_pEvasionClass;
}


//　回避クラス
CharacterEvasion* BaseballState_PlayerControll_Evasion::CreateEvasionClass(BaseballPlayer* b){
	class EvasionEvent : public CharacterEvasion::Event{
		BaseballPlayer* m_pBaseball;//　野球
	public:
		//　コンストラクタ
		EvasionEvent(BaseballPlayer* pBaseball) :
			m_pBaseball(pBaseball){}

		// 更新
		void Update()override
		{
			// モデル更新
			m_pBaseball->m_Renderer.Update(1.0f);

			// 転送行列更新
			chr_func::CreateTransMatrix(
				m_pBaseball,
				0.05f,
				&m_pBaseball->m_Renderer.m_TransMatrix);
		}


		// 回避行動開始
		void EvasionStart()override
		{
			m_pBaseball->m_Renderer.SetMotion(baseball_player::_mt_Evasion);
		}


		// 回避行動終了
		void EvasionEnd()override
		{
			// 通常移動へ
			m_pBaseball->m_Renderer.SetMotion(baseball_player::_mt_Stand);
			m_pBaseball->SetState(new BaseballState_PlayerControll_Move());
		}
	};

	// 回避パラメータ設定
	CharacterEvasion::EvasionParams params;
	params.AllFrame = 35;         // 全35フレーム
	params.MaxTurnRadian = PI / 4;    // 45°
	params.MoveDownSpeed = 0.2f;      // 減速割合
	params.MoveSpeed = roolspeed;    // 移動スピード
	params.NoDamageStartFrame = 3;          // 開始3フレームで無敵開始
	params.NoDamageEndFrame = 20;       // 開始20フレームで無敵終了

	// 作成
	return new CharacterEvasion(
		b,
		new EvasionEvent(b),
		params
		);
}
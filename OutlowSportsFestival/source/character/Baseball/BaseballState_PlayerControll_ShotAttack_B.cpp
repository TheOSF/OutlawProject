#include "BaseballPlayerState.h"
#include "Baseball_HitEvent.h"
#include "BaseballState_PlayerControll_ShotAttack_B.h"
#include "../../GameSystem/GameController.h"
#include "../CharacterFunction.h"
#include "../CharacterManager.h"
#include "../CharacterEvasionClass.h"

#include "../../Ball/UsualBall.h"
#include "../../Ball/MilderHoming.h"
#include "../../Damage/Damage.h"
#include "../CharacterCounterClass.h"

//***************************************
//　遠距離(バッター)
//***************************************

//　コンストラクタ
BaseballState_PlayerControll_ShotAttack_B::BaseballState_PlayerControll_ShotAttack_B() :m_pShotAttackClass_B(nullptr){

}


//　ステート開始
void BaseballState_PlayerControll_ShotAttack_B::Enter(BaseballPlayer* b){
	// 遠距離(バッター)クラス作成
	m_pShotAttackClass_B = this->CreateShotAttackClass_B(b);
}


//　ステート実行
void BaseballState_PlayerControll_ShotAttack_B::Execute(BaseballPlayer* b){
	// スティックの値セット
	m_pShotAttackClass_B->SetStickValue(
		controller::GetStickValue(controller::stick::left, b->m_PlayerInfo.number));

	// 更新
	if (m_pShotAttackClass_B->Update() == false)
	{
		return;
	}
}

//　ステート終了
void BaseballState_PlayerControll_ShotAttack_B::Exit(BaseballPlayer* b){
	delete m_pShotAttackClass_B;
}




//　遠距離クラス
CharacterShotAttack* BaseballState_PlayerControll_ShotAttack_B::CreateShotAttackClass_B(BaseballPlayer* b){
	class ShotAttackEvent_B :public CharacterShotAttack::Event{
		BaseballPlayer* m_pBaseball;//　野球
		MilderHoming* mild;
	public:
		//　コンストラクタ
		ShotAttackEvent_B(BaseballPlayer* pBaseball) :
			m_pBaseball(pBaseball){}
		//　更新
		void Update()override{
			//　モデル更新
			m_pBaseball->m_Renderer.Update(1.0f);

			// 転送行列更新
			chr_func::CreateTransMatrix(
				m_pBaseball,
				0.05f,
				&m_pBaseball->m_Renderer.m_TransMatrix);
		}

		// ダメージ判定開始 & ボール発射
		void Shot()
		{
			BallBase::Params param;

			chr_func::GetFront(m_pBaseball, &param.move);
			param.move *= 0.7f;
			param.pos = m_pBaseball->m_Params.pos;
			param.pos.y = BallBase::UsualBallShotY;
			param.pParent = m_pBaseball ;
			param.type = BallBase::Type::_Usual;

			new UsualBall(param, DamageBase::Type::_WeekDamage, 1);
		}

		//　遠距離攻撃開始
		void AttackStart()override{
			//　☆モーション
			m_pBaseball->m_Renderer.SetMotion(baseball_player::_mb_Shot);
		}

		void AttackEnd()
		{
			//攻撃終了時に通常移動モードに戻る
			m_pBaseball->SetState(new BaseballState_PlayerControll_Move());
		}
	};

	CharacterShotAttack::AttackParams atk;

	atk.AllFrame = 40;
	atk.AttackPower = 8;
	atk.MaxTurnRadian = PI / 4;
	atk.MoveDownSpeed = 0.8f;
	atk.ShotFrame = 15;

	return m_pShotAttackClass_B = new CharacterShotAttack(
		b,
		new ShotAttackEvent_B(b),
		atk,
		new  BaseballHitEvent(b)
		);
}

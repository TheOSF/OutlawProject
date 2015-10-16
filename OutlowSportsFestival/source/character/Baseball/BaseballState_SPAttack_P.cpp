#include "BaseballState_SPAttack_P.h"
#include "BaseballPlayerState.h"
#include "Baseball_HitEvent.h"
#include "../../Ball/Baseball_SpAtk_Ball.h"

#include "../../Camera/Camera.h"
#include "../../Collision/Collision.h"
#include "../../GameSystem/GameController.h"
#include "../CharacterFunction.h"
#include "../CharacterManager.h"
#include "../CharacterEvasionClass.h"

//　コンストラクタ
BaseballState_SPAttack_P::BaseballState_SPAttack_P() :m_pSpAttack_P(nullptr)
{

}


//　ステート開始
void BaseballState_SPAttack_P::Enter(BaseballPlayer* b)
{
	// 遠距離(バッター)クラス作成
	m_pSpAttack_P = this->CreateSpAttack_P(b);
}


//　ステート実行
void BaseballState_SPAttack_P::Execute(BaseballPlayer* b){
	// スティックの値セット
	m_pSpAttack_P->SetStickValue(
		controller::GetStickValue(controller::stick::left, b->m_PlayerInfo.number));

	// 更新
	if (m_pSpAttack_P->Update() == false)
	{
		return;
	}
}

//　ステート終了
void BaseballState_SPAttack_P::Exit(BaseballPlayer* b){
	delete m_pSpAttack_P;
}


//　遠距離クラス
CharacterShotAttack* BaseballState_SPAttack_P::CreateSpAttack_P(BaseballPlayer* b){
	class SpAttackEvent :public CharacterShotAttack::Event{
		BaseballPlayer* m_pBaseball;//　野球
	public:
		//　コンストラクタ
		SpAttackEvent(BaseballPlayer* pBaseball) :
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
			param.move *= 10.0f;
			param.pos = m_pBaseball->m_Params.pos;
			param.pos.y = BallBase::UsualBallShotY;
			param.pParent = m_pBaseball;
			param.type = BallBase::Type::_BaseballSpecialAtk;

			new Baseball_SpAtk_Ball(param, DamageBase::Type::_VanishDamage, 1);
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
	atk.AttackPower = 20;
	atk.MaxTurnRadian = PI / 4;
	atk.MoveDownSpeed = 1.2f;
	atk.ShotFrame = 15;

	return m_pSpAttack_P = new CharacterShotAttack(
		b,
		new SpAttackEvent(b),
		atk,
		new  BaseballHitEvent(b)
		);
}

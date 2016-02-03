
#include "BaseballPlayerState.h"
#include "Baseball_HitEvent.h"
#include "BaseballState_PlayerControll_ShotAttack_P.h"

#include "Computer/BaseballPlayerState_ComMove.h"

#include "../../GameSystem/GameController.h"
#include "../CharacterFunction.h"
#include "../CharacterManager.h"
#include "../CharacterEvasionClass.h"

#include "../../Ball/UsualBall.h"
#include "../../Ball/MilderHoming.h"
#include "../../Damage/Damage.h"

#include "Sound/Sound.h"
#include "../../GameSystem/GameController.h"

//***************************************
//　遠距離(投手)
//***************************************
class ShotAttackEvent_P;
//　コンストラクタ
BaseballState_PlayerControll_ShotAttack_P::BaseballState_PlayerControll_ShotAttack_P() :
pTargetEnemy(nullptr), m_pShotAttackClass_P(nullptr)
{

}


//　ステート開始
void BaseballState_PlayerControll_ShotAttack_P::Enter(BaseballPlayer* b)
{
	// 遠距離(ピッチャー)クラス作成
	m_pShotAttackClass_P = this->CreateShotAttackClass_P(b);

}


//　ステート実行
void BaseballState_PlayerControll_ShotAttack_P::Execute(BaseballPlayer* b){

	//　Playerならこっち
	if (b->m_PlayerInfo.player_type == PlayerType::_Player)
	{
		// スティックの値セット
		m_pShotAttackClass_P->SetStickValue(
			controller::GetStickValue(controller::stick::left, b->m_PlayerInfo.number));
		// 更新
		if (m_pShotAttackClass_P->Update() == false)
		{
			return;
		}
	}
	else
	{

		// 更新
		if (m_pShotAttackClass_P->Update() == false)
		{
			return;
		}


	}


}

//　ステート終了
void BaseballState_PlayerControll_ShotAttack_P::Exit(BaseballPlayer* b){
	delete m_pShotAttackClass_P;
}


//　遠距離クラス
CharacterShotAttack* BaseballState_PlayerControll_ShotAttack_P::CreateShotAttackClass_P(BaseballPlayer* b){
	class ShotAttackEvent_P :public CharacterShotAttack::Event{
		BaseballPlayer* m_pBaseball;//　野球
		MilderHoming* mild;
	public:
		//　ボール
		BallBase::Params param;
		//　ターゲット
		Vector3 target;
	public:
		//　コンストラクタ
		ShotAttackEvent_P(BaseballPlayer* pBaseball) :target(0, 0, 0),
			m_pBaseball(pBaseball){}
		//　更新
		void Update()override{

			//　モデル更新
			m_pBaseball->ModelUpdate(1);

			// 転送行列更新
			chr_func::CreateTransMatrix(
				m_pBaseball,
                &m_pBaseball->getNowModeModel()->m_TransMatrix);
		}
	public:
		// ダメージ判定開始 & ボール発射
		void Shot()
		{
			//　遠距離攻撃(param計算)
			BallBase::Params param;

			chr_func::GetFront(m_pBaseball, &param.move);

            param.pos = m_pBaseball->getNowModeModel()->GetWorldBonePos(40);
			param.pos.y = UsualBall::UsualBallShotY;

			param.pParent = m_pBaseball;
			param.scale = 1.0f;
			param.type = BallBase::Type::_Milder;

			//生成
            new UsualBall(
                param,
                DamageBase::Type::_WeekDamage,
                5.0f,
                MilderHoming::GetMilderHomingMove()
                );

            //コントローラを振動
            chr_func::SetControllerShock(
                m_pBaseball,
                0.5f,
                0.15f
                );

            //スキルゲージ加算
            chr_func::AddSkillGauge(m_pBaseball, UsualBall::AddSkillValueRatio);
		}

		//　遠距離攻撃開始
		void AttackStart()override{
			//　☆モーション
			m_pBaseball->SetMotion(baseball_player::_mb_Atk2_P);
			//　効果音
			Sound::Play(Sound::Swing3);
		}

		void AttackEnd()

		{	//攻撃終了時に通常移動モードに戻る
			if (m_pBaseball->m_PlayerInfo.player_type == PlayerType::_Player)
			{
				//　プレイヤー
				m_pBaseball->SetState(new BaseballState_PlayerControll_Move());
			}
			else
			{
				//　コンピューター
				m_pBaseball->SetState(new BaseballPlayerState_ComMove());
			}
		}
	};

	CharacterShotAttack::AttackParams atk;

	atk.AllFrame = 40;
	atk.MaxTurnRadian = PI / 4;
	atk.MoveDownSpeed = 0.8f;
	atk.ShotFrame = 15;

	return m_pShotAttackClass_P = new CharacterShotAttack(
		b,
		new ShotAttackEvent_P(b),
		atk,
		new  BaseballHitEvent(b)
		);
}

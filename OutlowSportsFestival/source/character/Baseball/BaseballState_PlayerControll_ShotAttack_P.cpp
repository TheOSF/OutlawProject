
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
#include "../CharacterCounterClass.h"

#include "Sound/Sound.h"

//***************************************
//　遠距離(投手)
//***************************************
class ShotAttackEvent_P;
//　コンストラクタ
BaseballState_PlayerControll_ShotAttack_P::BaseballState_PlayerControll_ShotAttack_P() :m_pShotAttackClass_P(nullptr){

}


//　ステート開始
void BaseballState_PlayerControll_ShotAttack_P::Enter(BaseballPlayer* b){
	// 遠距離(ピッチャー)クラス作成
	m_pShotAttackClass_P = this->CreateShotAttackClass_P(b);
}


//　ステート実行
void BaseballState_PlayerControll_ShotAttack_P::Execute(BaseballPlayer* b){
	// スティックの値セット
	m_pShotAttackClass_P->SetStickValue(
		controller::GetStickValue(controller::stick::left, b->m_PlayerInfo.number));

	//　Comなら
	ComExcute(b);

	// 更新
	if (m_pShotAttackClass_P->Update() == false)
	{
		return;
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
		ShotAttackEvent_P(BaseballPlayer* pBaseball) :target(0,0,0),
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
	public:
		// ダメージ判定開始 & ボール発射
		void Shot()
		{
			//　遠距離攻撃(param計算)
			BallBase::Params param;

			chr_func::GetFront(m_pBaseball, &param.move);

			param.pos = m_pBaseball->m_Params.pos;
			param.pos.y = UsualBall::UsualBallShotY;

			param.pParent = m_pBaseball;
			param.scale = 1.0f;
			param.type = BallBase::Type::_Milder;
			//生成
			new MilderHoming(param,5,m_pBaseball);
		}

		//　遠距離攻撃開始
		void AttackStart()override{
			//　☆モーション
			m_pBaseball->m_Renderer.SetMotion(baseball_player::_mb_Shot);
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
	atk.AttackPower = 8;
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

void BaseballState_PlayerControll_ShotAttack_P::ComExcute(BaseballPlayer* b)
{
	if (b->m_PlayerInfo.player_type == PlayerType::_Computer)
	{
		//ターゲット選定＆向き補正

		CharacterManager::CharacterMap ChrMap = DefCharacterMgr.GetCharacterMap();

		const float  AutoDistance = 400.0f;               //自動ができる最大距離
		const RADIAN AutoMaxAngle = D3DXToRadian(90);   //自動ができる最大角度

		const CharacterBase* pTargetEnemy = nullptr;    //ターゲット保持のポインタ
		RADIAN MostMinAngle = PI;                       //もっとも狭い角度
		RADIAN TempAngle;

		Vector3 MyFront;      //自身の前方ベクトル
		chr_func::GetFront(b, &MyFront);

		auto it = ChrMap.begin();

		while (it != ChrMap.end())
		{
			//自身を除外
			if (b->m_PlayerInfo.number == it->first->m_PlayerInfo.number ||
				chr_func::isDie(it->first)
				)
			{
				++it;
				continue;
			}

			//距離が一定以上のキャラクタを除外する
			if (Vector3Distance(it->first->m_Params.pos, b->m_Params.pos) > AutoDistance)
			{
				it = ChrMap.erase(it);
				continue;
			}

			//前ベクトルと敵へのベクトルの角度を計算する
			TempAngle = Vector3Radian(MyFront, (it->first->m_Params.pos - b->m_Params.pos));

			//角度が一番狭かったら更新
			if (TempAngle < MostMinAngle)
			{
				pTargetEnemy = it->first;
				MostMinAngle = TempAngle;
			}

			++it;
		}
	}

}
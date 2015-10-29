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
#include "../../Sound/Sound.h"

//
//void  BaseballState_SPAttack_P::PlayerControllEvent::AngleControll(RADIAN angle)
//{
//	const CharacterBase* const pTargetCharacter = GetFrontTargetEnemy();
//
//	if (pTargetCharacter != nullptr)
//	{
//		//自動回転
//		chr_func::AngleControll(m_pBaseball, pTargetCharacter->m_Params.pos, angle);
//	}
//	else
//	{
//		const Vector2 Stick = controller::GetStickValue(controller::stick::left, m_pBaseball->m_PlayerInfo.number);
//
//		//スティックが一定以上倒されているかどうか
//		if (Vector2Length(Stick) > 0.25f)
//		{
//			Vector3 Vec(Stick.x, 0, Stick.y);
//
//			//スティック値をカメラ空間に
//			Vec = Vector3MulMatrix3x3(Vec, matView);
//
//			//キャラクタ回転
//			chr_func::AngleControll(m_pBaseball, m_pBaseball->m_Params.pos + Vec, angle);
//		}
//	}
//}
//
//const CharacterBase*  BaseballState_SPAttack_P::PlayerControllEvent::GetFrontTargetEnemy()
//{
//	CharacterManager::CharacterMap ChrMap = DefCharacterMgr.GetCharacterMap();
//
//	const float  AutoDistance = 10.0f;               //自動ができる最大距離
//	const RADIAN AutoMaxAngle = D3DXToRadian(90);   //自動ができる最大角度
//
//	const CharacterBase* pTargetEnemy = nullptr;    //ターゲット保持のポインタ
//	RADIAN MostMinAngle = PI;                       //もっとも狭い角度
//	RADIAN TempAngle;
//
//	Vector3 MyFront;      //自身の前方ベクトル
//	chr_func::GetFront(m_pBaseball, &MyFront);
//
//	auto it = ChrMap.begin();
//
//	while (it != ChrMap.end())
//	{
//		//自身を除外
//		if (m_pBaseball->m_PlayerInfo.number == it->first->m_PlayerInfo.number ||
//			chr_func::isDie(it->first)
//			)
//		{
//			++it;
//			continue;
//		}
//
//		//距離が一定以上のキャラクタを除外する
//		if (Vector3Distance(it->first->m_Params.pos, m_pBaseball->m_Params.pos) > AutoDistance)
//		{
//			it = ChrMap.erase(it);
//			continue;
//		}
//
//		//前ベクトルと敵へのベクトルの角度を計算する
//		TempAngle = Vector3Radian(MyFront, (it->first->m_Params.pos - m_pBaseball->m_Params.pos));
//
//		//角度が一番狭かったら更新
//		if (TempAngle < MostMinAngle)
//		{
//			pTargetEnemy = it->first;
//			MostMinAngle = TempAngle;
//		}
//
//		++it;
//	}
//
//	return pTargetEnemy;
//
//}


//　コンストラクタ
BaseballState_SPAttack_P::BaseballState_SPAttack_P() :
m_Timer(0),
timeflg(false),
m_pSpAttack_P(nullptr)
{

}


//　ステート開始
void BaseballState_SPAttack_P::Enter(BaseballPlayer* b)
{
	// 遠距離(バッター)クラス作成
	m_pSpAttack_P = this->CreateSpAttack_P(b);
	m_pBaseBall = b;
	timeflg = false;
}


//　ステート実行
void BaseballState_SPAttack_P::Execute(BaseballPlayer* b){
	
	if (!timeflg)
	{
		m_Timer++;
		if (m_Timer == 1)
		{
			Sound::Play(Sound::Skill);
			FreezeGame(20);
		}
		if (m_Timer == 21)
		{
			m_Timer = 0;
			timeflg = true;
		}
	}
	else
	{
		// スティックの値セット
		m_pSpAttack_P->SetStickValue(
			controller::GetStickValue(controller::stick::left, b->m_PlayerInfo.number));

		// 更新
		if (m_pSpAttack_P->Update() == false)
		{
			return;
		}
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

void BaseballState_SPAttack_P::FreezeGame(UINT frame)
{
	std::list<LpGameObjectBase> UpdateObjList;

	UpdateObjList.push_back(m_pBaseBall);

	DefGameObjMgr.FreezeOtherObjectUpdate(UpdateObjList, frame);
}


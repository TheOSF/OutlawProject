#include "BaseballState_SPAttack_P.h"
#include "BaseballPlayerState.h"
#include "Baseball_HitEvent.h"
#include "../../Ball/Baseball_SpAtk_Ball.h"
#include "Computer/BaseballPlayerState_ComMove.h"
#include "../../Camera/Camera.h"
#include "../../Collision/Collision.h"
#include "../../GameSystem/GameController.h"
#include "../CharacterFunction.h"
#include "../CharacterManager.h"
#include "../CharacterEvasionClass.h"
#include "../../Sound/Sound.h"
#include "../../Effect/SpecialAttackEffect.h"
//　コンストラクタ
BaseballState_SPAttack_P::BaseballState_SPAttack_P() :
m_Timer(0),
timeflg(false),
m_pSpAttack_P(nullptr),
m_pStateFunc()
{

}


//　ステート開始
void BaseballState_SPAttack_P::Enter(BaseballPlayer* b)
{
	m_Timer = 0;

	//以前の移動値をリセット
	chr_func::XZMoveDown(b, 1);

	// 遠距離(バッター)クラス作成
	m_pSpAttack_P = this->CreateSpAttack_P(b);

	m_pBaseBall = b;
	timeflg = false;
	m_pStateFunc = &BaseballState_SPAttack_P::State_Shot;

	//　スキルゲージリセット
	chr_func::ResetSkillGauge(b);
}


//　ステート実行
void BaseballState_SPAttack_P::Execute(BaseballPlayer* b)
{

	//　実行
	(this->*m_pStateFunc)();

	//　モーション更新とか
	b->m_Renderer.Update(2);
	chr_func::CreateTransMatrix(b, b->m_ModelSize, &b->m_Renderer.m_TransMatrix);


}

//　ステート終了
void BaseballState_SPAttack_P::Exit(BaseballPlayer* b){
	delete m_pSpAttack_P;
}

void BaseballState_SPAttack_P::State_Shot()
{
	if (!timeflg)
	{
		m_Timer++;
		if (m_Timer == 1)
		{
			Sound::Play(Sound::Skill);
			FreezeGame(55);
			//エフェクト
			new SpecialAttackEffect(m_pBaseBall, 55);
			m_pBaseBall->m_Renderer.SetMotion(baseball_player::_mb_Stand_B);
		}
		if (m_Timer == 56)
		{
			m_Timer = 0;
			timeflg = true;
		}
	}
	else
	{

		// スティックの値セット
		if (m_pBaseBall->m_PlayerInfo.player_type == PlayerType::_Player)
		{
			m_pSpAttack_P->SetStickValue(
				controller::GetStickValue(controller::stick::left, m_pBaseBall->m_PlayerInfo.number));
		}
		target = nullptr;
		target = CalcTarget();
		const float AngleSpeed = D3DXToRadian(3);

		//ターゲットがいたら
		if (target != nullptr)
		{
			chr_func::AngleControll(m_pBaseBall, target->m_Params.pos, AngleSpeed*2.0f);
		}

		// 更新
		if (m_pSpAttack_P->Update() == false)
		{
			return;
		}

		m_Timer++;

		if (m_Timer == 30)
		{
			m_Timer = 0;
			m_pStateFunc = &BaseballState_SPAttack_P::State_End;
		}
	}

}

void BaseballState_SPAttack_P::State_End()
{
	if (m_pBaseBall->m_PlayerInfo.player_type == PlayerType::_Player)
	{
		//攻撃終了時に通常移動モードに戻る
		m_pBaseBall->SetState(new BaseballState_PlayerControll_Move());
	}
	else
	{
		m_pBaseBall->SetState(new BaseballPlayerState_ComMove());
	}
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
			m_pBaseball->m_Renderer.Update(2);

			// 転送行列更新
			chr_func::CreateTransMatrix(
				m_pBaseball,
				m_pBaseball->m_ModelSize,
				&m_pBaseball->m_Renderer.m_TransMatrix);

		}

		// ダメージ判定開始 & ボール発射
		void Shot()
		{

			Vector3 pos, move;

			chr_func::GetFront(m_pBaseball, &move);
			move *= 3.8f;
			pos = m_pBaseball->m_Params.pos;
			pos.y = BallBase::UsualBallShotY;

			//　威力とか
			new Baseball_SpAtk_Ball(m_pBaseball, pos, move, 50);
		}

		//　遠距離攻撃開始
		void AttackStart()override
		{

			//　☆モーション
			m_pBaseball->m_Renderer.SetMotion(baseball_player::_mb_Shot_P);
		}

		void AttackEnd()
		{
			//if (m_pBaseball->m_PlayerInfo.player_type == PlayerType::_Player)
			//{
			//	//攻撃終了時に通常移動モードに戻る
			//	m_pBaseball->SetState(new BaseballState_PlayerControll_Move());
			//}
			//else
			//{
			//	m_pBaseball->SetState(new BaseballPlayerState_ComMove());
			//}

		}

	};

	CharacterShotAttack::AttackParams atk;

	atk.AllFrame = 40;
	atk.AttackPower = 20;
	atk.MaxTurnRadian = PI / 4;
	atk.MoveDownSpeed = 1.2f;
	atk.ShotFrame = 15;
	//DamageManager::HitEventBase NoDmgHitEvent;   //ノーダメージ

	return m_pSpAttack_P = new CharacterShotAttack(
		b,
		new SpAttackEvent(b),
		atk,
		new  DamageManager::HitEventBase()
		);
}

void BaseballState_SPAttack_P::FreezeGame(UINT frame)
{
	std::list<LpGameObjectBase> UpdateObjList;

	UpdateObjList.push_back(m_pBaseBall);

	DefGameObjMgr.FreezeOtherObjectUpdate(UpdateObjList, frame);
}


//　ターゲット選定
CharacterBase* BaseballState_SPAttack_P::CalcTarget()const
{
	Vector3 v1, v2;

	const float HomingAngle = PI / 4;
	float MostNear = 10000;
	float TempLen;
	CharacterBase* pTarget = nullptr;

	//　map代入
	const CharacterManager::CharacterMap& chr_map =
		DefCharacterMgr.GetCharacterMap();


	for (auto it = chr_map.begin(); it != chr_map.end(); ++it)
	{

		//　死んでるor自分ならcontinue
		if (chr_func::isDie(it->first) ||
			it->first->m_PlayerInfo.number == m_pBaseBall->m_PlayerInfo.number)
		{
			continue;
		}

		//　視野角計算
		chr_func::GetFront(m_pBaseBall, &v1);

		v2 = it->first->m_Params.pos - m_pBaseBall->m_Params.pos;
		v2.y = 0;

		//角度外なら適していない
		if (Vector3Radian(v1, v2) > HomingAngle)
		{
			continue;
		}

		TempLen = v2.Length();

		if (MostNear > TempLen)
		{
			MostNear = TempLen;
			pTarget = it->first;
		}
	}

	return pTarget;
}

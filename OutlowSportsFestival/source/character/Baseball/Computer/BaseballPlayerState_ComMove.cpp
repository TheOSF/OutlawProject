#include "BaseballPlayerState_ComMove.h"
#include "../BaseballState_PlayerControll_ShotAttack_B.h"
#include "../BaseballState_PlayerControll_ShotAttack_P.h"
#include "../BaseballState_Change.h"
#include "../BaseballPlayerState_Counter.h"
#include "../../../GameSystem/GameController.h"
#include "../../CharacterFunction.h"
#include "../../../Ball/UsualBall.h"
#include "../../../Damage/Damage.h"
#include "../../CharacterCounterClass.h"
#include "../../CharacterMoveClass.h"
#include "../../CharacterShotAttackClass.h"
#include "../../CharacterManager.h"
#include "../Baseball_HitEvent.h"
#include "../BaseballPlayerState_Attack_B.h"
#include "../BaseballPlayerState_Attack_P.h"



//ステート開始
void BaseballPlayerState_ComMove::Enter(BaseballPlayer* b)
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

	//移動クラスの作成
	m_pMoveClass = new CharacterUsualMove(
		b,
		p,
		new BaseballMoveEvent(b),
		new BaseballHitEvent(b)
		);


	//移動コントロールクラスの作成
	if (b->m_PlayerInfo.strong_type == StrongType::__ErrorType)
	{
		m_pMoveControllClass = new SandBagMove_Com_ControllClass();
	}
	else
	{
		m_pMoveControllClass = new ComBaseballMoveControllClass(b);
	}

}


void BaseballPlayerState_ComMove::Execute(BaseballPlayer* b)
{
	//スティック値をセット
	m_pMoveClass->SetStickValue(m_pMoveControllClass->Move(b));

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

	//　更新
	//　動き
	m_pMoveClass->Update();

	//モデルのワールド変換行列を更新
	chr_func::CreateTransMatrix(b, b->m_ModelSize, &b->m_Renderer.m_TransMatrix);

}

void BaseballPlayerState_ComMove::Exit(BaseballPlayer* b)
{
	delete m_pMoveClass;
	delete m_pMoveControllClass;
}


//　バッター時
void BaseballPlayerState_ComMove::Batter(BaseballPlayer* b)
{

	//b->SetState(new BaseballState_PlayerControll_ShotAttack_B((new PlayerShotControllClass_B(b)));
		
}

//　投手時
void  BaseballPlayerState_ComMove::Pitcher(BaseballPlayer* b)
{
	static int time = 0;
	time++;
	if (time >= 120)
	{
		b->SetState(new BaseballState_PlayerControll_ShotAttack_P());
		time = 0;
	}
}



//---------------------------------------------------------------------
//                      SandBagMoveControllClass
//---------------------------------------------------------------------

Vector2 SandBagMove_Com_ControllClass::Move(BaseballPlayer* b)
{
	//スティックの値としてコンピュータ操作の移動を計算する
	const Vector3 Target = DefCharacterMgr.GetRoundStartPos(b->m_PlayerInfo.number);
	Vector3 v = Target - b->m_Params.pos;

	if (v.Length() < 1.0f)
	{
		v = Vector3Zero;
	}

	return Vector2Normalize(Vector2(v.x, v.z));
}

//---------------------------------------------------------------------
//                      ComBaseballMoveControllClass
//---------------------------------------------------------------------

//　コンストラクタ
ComBaseballMoveControllClass::ComBaseballMoveControllClass(BaseballPlayer* b) :
m_pBaseball(b)
{
	//　行動タイプ
	m_Type = 1;
	m_Vec = Vector3Zero;

	//　各パラメータセット
	GetParams(m_Param, b->m_PlayerInfo.strong_type);
	//　初期関数セット
	m_pStateFunc = &ComBaseballMoveControllClass::StateChoise;
	//　初期値セット
	m_Count = rand() % 20;
	m_MoveTargetPos = Vector3Zero;
}

ComBaseballMoveControllClass::~ComBaseballMoveControllClass()
{

}

//　移動パターン選択
void  ComBaseballMoveControllClass::StateChoise(Vector2&)
{
	switch (m_Type)
	{
		// スタート地点へ
	case 0:
		m_pStateFunc = &ComBaseballMoveControllClass::StateMove_StartPos;
		break;
		//　ターゲット(プレイヤー)のところへ
	case 1:
		m_pStateFunc = &ComBaseballMoveControllClass::StateMove_TargetPos;
		break;

	default:
		break;
	}
	
}

Vector2 ComBaseballMoveControllClass::Move(BaseballPlayer* b)
{
	//　バッターとピッチャー切り替え
	Change(b);

	(this->*m_pStateFunc)(Vector2(m_Vec.x, m_Vec.z));

	return Vector2Normalize(Vector2(m_Vec.x, m_Vec.z));
}

//　切り替え
void ComBaseballMoveControllClass::Change(BaseballPlayer* b)
{
	nearpos = GetTargetPos() - b->m_Params.pos;
	
	if (nearpos.Length() < 10.0f && !b->getBatterFlg() ||
		nearpos.Length() > 41.0f && b->getBatterFlg())
	{
		b->SetState(new BaseballState_Change());
	}

}

void ComBaseballMoveControllClass::GetParams(Param& out, StrongType::Value st)
{
	switch (st)
	{
		//弱い
	case StrongType::_Weak:
		out.BallCounter = 0.3f;
		out.BallCounterSpeed = 0.5f;
		out.BallCounterTec = 0.1f;
		out.RunStop = 0.8f;
		out.RunPlaceTec = 0.1f;
		out.DangerEscape = 0.2f;
		out.ActionFrequence = 0.3f;
		out.NamePlay = 1.0f;
		break;

		//普通
	case StrongType::_Usual:
		out.BallCounter = 0.6f;
		out.BallCounterSpeed = 0.7f;
		out.BallCounterTec = 0.7f;
		out.RunStop = 0.4f;
		out.RunPlaceTec = 0.5f;
		out.DangerEscape = 0.5f;
		out.ActionFrequence = 0.6f;
		out.NamePlay = 0.5f;
		break;

		//強い
	case StrongType::_Strong:
		out.BallCounter = 1.0f;
		out.BallCounterSpeed = 1.0f;
		out.BallCounterTec = 1.0f;
		out.RunStop = 0.2f;
		out.RunPlaceTec = 1.0f;
		out.DangerEscape = 1.0f;
		out.ActionFrequence = 1.0f;
		out.NamePlay = 0.0f;
		break;

	default:
		MyAssert(false, "認識できないstrongTypeがComMoveControllClass::GetParamsに送られました");
		break;
	}
}


//　スタート位置へ戻る
void ComBaseballMoveControllClass::StateMove_StartPos(Vector2& out)
{
	//スティックの値としてコンピュータ操作の移動を計算する
	const Vector3 Target = DefCharacterMgr.GetRoundStartPos(m_pBaseball->m_PlayerInfo.number);
	Vector3 v = Target - m_pBaseball->m_Params.pos;
	if (v.Length() < 1.0f)
	{
		v = Vector3Zero;
	}
	m_Vec.x = v.x;
	m_Vec.z = v.z;

}

//　ターゲット(キャラ)の位置へ
void ComBaseballMoveControllClass::StateMove_TargetPos(Vector2& out)
{
	Vector3 v = GetTargetPos() - m_pBaseball->m_Params.pos;
	m_Vec.x = v.x;
	m_Vec.z = v.z;
}


void ComBaseballMoveControllClass::StateStop(Vector2& out)
{

}

//　ターゲット(キャラ)
Vector3 ComBaseballMoveControllClass::GetTargetPos()
{
	Vector3 v1, v2;

	const float HomingAngle = PI / 4;
	float MostHp = 0;
	float TempHp;
	Vector3 pTarget = Vector3Zero;

	//　map代入
	const CharacterManager::CharacterMap& chr_map =
		DefCharacterMgr.GetCharacterMap();


	for (auto it = chr_map.begin(); it != chr_map.end(); ++it)
	{
		//　死んでるor自分ならcontinue
		if (chr_func::isDie(it->first) ||
			it->first->m_PlayerInfo.number == m_pBaseball->m_PlayerInfo.number)
		{
			continue;
		}

		TempHp = it->first->m_Params.HP;

		//　一番体力が高いキャラ選択
		if (MostHp < TempHp)
		{
			MostHp = TempHp;
			pTarget = it->first->m_Params.pos;
		}
	}

	return pTarget;

}

Vector3 ComBaseballMoveControllClass::GetMoveTargetPos()
{
	Vector3 ret;

	//野球　＝　遠距離or近距離が有利なので、
	//　臨機応変に切り替えるのがBest

	struct TargetInfo
	{
		bool      ok;
		Vector3   pos;
	};

	TargetInfo targets[8];

	const CharacterManager::CharacterMap& ChrMap = DefCharacterMgr.GetCharacterMap();

	for (int i = 0; i < (int)ARRAYSIZE(targets); ++i)
	{
		//ランダムに座標を作成
		targets[i].pos = Vector3(frand() - 0.5f, 0, frand() - 0.5f) * 50.f;
		targets[i].pos += Vector3Normalize(targets[i].pos) * 5.0f;


		//その点が良いかどうか
		for (auto& it : ChrMap)
		{
			if (it.first == m_pBaseball ||
				chr_func::isDie(it.first))
			{
				continue;
			}

		}
		return targets[i].pos;
	}
	
	return Vector3Zero;
}
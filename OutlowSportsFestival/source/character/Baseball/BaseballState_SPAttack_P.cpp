#include "BaseballState_SPAttack_P.h"
#include "BaseballPlayerState.h"
#include "Baseball_HitEvent.h"
#include "../../Ball/UsualBall.h"
#include "../../Ball/MilderHoming.h"
#include "../../Camera/Camera.h"
#include "../../Collision/Collision.h"
#include "../../GameSystem/GameController.h"
#include "../CharacterFunction.h"
#include "../CharacterManager.h"
#include "../CharacterEvasionClass.h"
Baseball_SPAttack_P::Baseball_SPAttack_P(BaseballPlayer* b, BallBase::Params& param):
m_Locus(20),
m_DeleteFlag(false),
m_pBaseball(b)
{
	LPIEXMESH		pBallMesh;

	//パラメータ代入
	m_Params = param;

	//ダメージ判定のパラメータを代入
	m_Damage.pBall = this;
	m_Damage.pParent = param.pParent;
	m_Damage.m_Param.size = 1;	//大きさはボールによって異なる?
	m_Damage.type = DamageBase::Type::_WeekDamage;
	m_Damage.Value = 0.1f;
	m_Damage.m_Enable = true;
	UpdateDamageClass();

	//ボールのメッシュを作成
	UsualBall::GetBallMesh(param.pParent->m_PlayerInfo.chr_type, &pBallMesh);

	//メッシュのレンダラー作成(最終的にメッシュを使いまわして描画するべき)
	m_pMeshRenderer = new MeshRenderer(
		pBallMesh,
		false,
		MeshRenderer::RenderType::UseColor
		);

	D3DXQuaternionIdentity(&m_Ballrot);

	UpdateMesh();

	//軌跡の設定
	m_Locus.m_Division = 0;
	m_Locus.m_StartParam.Width = 0.4f;
	m_Locus.m_EndParam.Width = 0.1f;

	UpdateLocusColor();
}

Baseball_SPAttack_P::~Baseball_SPAttack_P()
{
	delete m_pMeshRenderer;
}


bool Baseball_SPAttack_P::Update()
{
	UpdateMove();
	UpdateMesh();
	UpdateDamageClass();

	if (m_Params.type != BallBase::Type::_DontWork)
	{
		UpdateWallCheck();
	}

	Vector3 v;
	Vector3Cross(v, m_Params.move, DefCamera.GetForward());
	v.Normalize();

	m_Locus.AddPoint(m_Params.pos, v);

	return !m_DeleteFlag && !isOutofField() && m_DeleteFrame > 0;
}


bool Baseball_SPAttack_P::Msg(MsgType mt)
{

	return false;
}


bool Baseball_SPAttack_P::isOutofField()const
{
	return Vector3Length(m_Params.pos) > 100;
}


void Baseball_SPAttack_P::UpdateDamageClass()
{
	m_Damage.vec = m_Params.move;
	m_Damage.m_Param.pos = m_Params.pos;
}

void Baseball_SPAttack_P::UpdateMesh()
{
	//メッシュのワールド変換行列を更新する

	Matrix m;
	const float s = UsualBall::GetBallScale(m_Params.pParent->m_PlayerInfo.chr_type);

	D3DXMatrixScaling(&m, s, s, s);	//大きさはボールによって変える必要がある

	m._41 = m_Params.pos.x;
	m._42 = m_Params.pos.y;
	m._43 = m_Params.pos.z;

	m_pMeshRenderer->SetMatrix(m);
}

void Baseball_SPAttack_P::UpdateLocusColor()
{
	const DWORD Color = CharacterBase::GetPlayerColor(m_Params.pParent->m_PlayerInfo.number);

	m_Locus.m_StartParam.Color = Vector4(
		float((Color >> 16) & 0xFF) / 255.f,
		float((Color >> 8) & 0xFF) / 255.f,
		float(Color & 0xFF) / 255.f,
		0.5f
		);

	m_Locus.m_EndParam.Color = Vector4(1, 1, 1, 0);
}

void Baseball_SPAttack_P::UpdateMove()
{

	m_Params.pos += m_Params.move;

	//敵に当たっていたら攻撃判定をなくす
	if (m_Damage.HitCount > 0)
	{
		//攻撃判定のない状態にする
		ToNoWork();
	}
}

void Baseball_SPAttack_P::UpdateWallCheck()
{
	Vector3 Out, Pos(m_Params.pos), Vec(m_Params.move);
	float Dist = m_Params.move.Length()*2.0f;
	int Material;

	Vec.Normalize();

	if (DefCollisionMgr.RayPick(
		&Out,
		&Pos,
		&Vec,
		&Dist,
		&Material,
		CollisionManager::RayType::_Usual
		)
		)
	{
		//攻撃判定のない状態にする
		ToNoWork();
	}

}

void Baseball_SPAttack_P::Counter(CharacterBase* pCounterCharacter)
{
	m_Damage.pParent = m_Params.pParent = pCounterCharacter;

	UpdateLocusColor();
}

void Baseball_SPAttack_P::ToNoWork()
{
	//攻撃判定のない状態にする

	if (m_DeleteFlag)
	{
		return;
	}

	m_Params.type = BallBase::Type::_DontWork;
	m_Damage.m_Enable = false;
	m_DeleteFlag = true;

	m_BaseMatrix = m_pMeshRenderer->GetMatrix();

	m_BaseMatrix._41 = 0;
	m_BaseMatrix._42 = 0;
	m_BaseMatrix._43 = 0;
}

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
		MilderHoming* mild;
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
			param.move *= 0.5f;
			param.pos = m_pBaseball->m_Params.pos;
			param.pos.y = BallBase::UsualBallShotY;
			param.pParent = m_pBaseball;
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
	atk.MoveDownSpeed = 1.2f;
	atk.ShotFrame = 15;

	return m_pSpAttack_P = new CharacterShotAttack(
		b,
		new SpAttackEvent(b),
		atk,
		new  BaseballHitEvent(b)
		);
}

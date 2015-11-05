#include "Baseball_SpAtk_Ball.h"
#include "../character/CharacterBase.h"
#include "../character/CharacterFunction.h"
#include "../Render/MeshRenderer.h"
#include "../Camera/Camera.h"
#include "../GameSystem/ResourceManager.h"
#include "../Stage/PhysicsMeshObject.h"
#include "../Collision/Collision.h"
#include "BallFadeOutRenderer.h"
#include "../Effect/EffectFactory.h"
#include "../Effect/ThunderEffect.h"



Baseball_SpAtk_Ball::Baseball_SpAtk_Ball(
	BallBase::Params	params,			//ボールパラメータ
	DamageBase::Type	damage_type,	//ダメージ判定のタイプ
	float				damage_val,		//ダメージ量
	UINT                hit_num         //ヒット数
	) :

	m_DeleteFrame(180),
	m_pRigitBody(nullptr),
	m_HitNum(hit_num),
	m_HitCountSave(0),
	m_HitStopFrame(0),
	m_pStateFunc(&Baseball_SpAtk_Ball::StateFlyMove)
{

	class PhysicUpdateCallBack_Baseball :public MeshRenderer::PreRenderCallBack
	{
		RigidBody* const pRigidBody;
	public:
		PhysicUpdateCallBack_Baseball(RigidBody* const pRigidBody) :pRigidBody(pRigidBody){}

		void Execute(MeshRenderer* pRenderer, Type type)
		{

		}
	};

	LPIEXMESH		pBallMesh;

	//パラメータ代入
	m_Params = params;

	{
		//ダメージ判定のパラメータを代入
		m_Damage.pBall = this;
		m_Damage.pParent = params.pParent;
		m_Damage.m_Param.width = 1;	//大きさはボールによって異なる?
		m_Damage.type = damage_type;
		m_Damage.Value = damage_val;
		m_Damage.m_Enable = true;
		m_Damage.m_Param.pos1 = m_Params.pos;
		m_Damage.m_Param.pos2 = m_Params.pos;

		UpdateDamageClass();
	}

	{
		const float MeshScale = GetBallScale(params.pParent->m_PlayerInfo.chr_type);
		Matrix m;

		//ボールのメッシュを作成
		GetBallMesh(params.pParent->m_PlayerInfo.chr_type, &pBallMesh);

		//メッシュのレンダラー作成(最終的にメッシュを使いまわして描画するべき)
		m_pMeshRenderer = new MeshRenderer(
			pBallMesh,
			false,
			MeshRenderer::RenderType::UseColor
			);

		D3DXMatrixScaling(&m, MeshScale, MeshScale, MeshScale);

		m._41 = m_Params.pos.x;
		m._42 = m_Params.pos.y;
		m._43 = m_Params.pos.z;

		m_pMeshRenderer->SetMatrix(m);
	}

	//カメラのゆれ
	DefCamera.SetShock(Vector2(0.4f, 0.4f), 30);

}

Baseball_SpAtk_Ball::~Baseball_SpAtk_Ball()
{
	delete m_pMeshRenderer;
	if (m_pRigitBody != nullptr)
	{
		DefBulletSystem.RemoveRigidBody(m_pRigitBody);
	}
}

bool Baseball_SpAtk_Ball::GetBallMesh(
	CharacterType::Value	type,
	LPIEXMESH*				ppOut)
{
	*ppOut = nullptr;

	*ppOut = DefResource.Get(Resource::MeshType::BaseBall_ball);

	return (*ppOut != nullptr);
}


float Baseball_SpAtk_Ball::GetBallScale(
	CharacterType::Value	type    //ボールのキャラクタタイプ
	)
{
	switch (type)
	{

	case CharacterType::_Baseball:
		return 0.0045f;

	default:
		break;
	}

	MyAssert(false, "存在しないタイプのキャラクタタイプがUsualBall::GetBallScaleに渡されました　type= %d ", (int)type);

	return 0;
}

Baseball_SpAtk_Ball::PhysicsParam Baseball_SpAtk_Ball::GetBallPhysics(
	CharacterType::Value	type	//ボールのキャラクタタイプ
	)
{
	PhysicsParam params[] =
	{
		{ 0.5f, 100.0f, 0.4f, 0.2f },
		{ 0.5f, 100.0f, 0.4f, 0.2f },
		{ 0.5f, 100.0f, 0.55f, 0.2f },
		{ 0.5f, 100.0f, 0.5f, 0.2f },
		{ 0.5f, 100.0f, 0.5f, 0.2f },
		{ 0.5f, 100.0f, 0.5f, 0.2f },
	};

	MyAssert((int)type >= 0 && (int)type < (int)ARRAYSIZE(params), "存在しないタイプのキャラクタタイプがBaseBall_SpAtk_Ball::GetBallPhysicsに渡されました　type= %d ", (int)type);

	return params[(int)type];
}

bool Baseball_SpAtk_Ball::Update()
{
	return (this->*m_pStateFunc)();
}


bool Baseball_SpAtk_Ball::Msg(MsgType mt)
{

	return false;
}


bool Baseball_SpAtk_Ball::isOutofField()const
{
	return Vector3Length(m_Params.pos) > 100;
}


void Baseball_SpAtk_Ball::UpdateDamageClass()
{
	m_Damage.vec = m_Params.move;
	m_Damage.vec.Normalize();
	m_Damage.vec *= 0.5f;
	m_Damage.vec.y = 0.3f;
	m_Damage.m_Param.width = 2.0f;
	m_Damage.m_Param.pos2 = m_Damage.m_Param.pos1;
	m_Damage.m_Param.pos1 = m_Params.pos;
}


bool Baseball_SpAtk_Ball::UpdateWallCheck(Vector3& outNewMove)
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
		CollisionManager::RayType::_Ball
		)
		)
	{
		outNewMove = Vector3Refrect(m_Params.move, Vec);
		outNewMove *= 0.75f;
		return true;
	}

	return false;
}



void Baseball_SpAtk_Ball::ToNoWork()
{
	//すでに攻撃判定のない状態になっていたらreturn
	if (m_Params.type == BallBase::Type::_DontWork)
	{
		return;
	}

	//攻撃判定のない状態にする
	m_Params.type = BallBase::Type::_DontWork;
	m_Damage.m_Enable = false;

	//ステート関数を切り替え
	m_pStateFunc = &Baseball_SpAtk_Ball::StatePhysicMove;

	m_BaseMatrix = m_pMeshRenderer->GetMatrix();

	m_BaseMatrix._41 = 0;
	m_BaseMatrix._42 = 0;
	m_BaseMatrix._43 = 0;

	const PhysicsParam p = GetBallPhysics(m_Params.pParent->m_PlayerInfo.chr_type);

	m_pRigitBody = DefBulletSystem.AddRigidSphere(
		p.Mass,
		RigidBody::ct_dynamic,
		m_Params.pos,
		Vector3Zero,
		p.Radius,
		p.Friction,
		p.Restitution,
		m_Params.move * 10.0f
		);
}


bool Baseball_SpAtk_Ball::StateFlyMove()
{

	//移動更新
	{
		//ヒットストップフレームなら移動更新をしない
		if (m_HitStopFrame == 0)
		{
			m_Params.pos += m_Params.move;
		}
		else
		{
			//ヒットストップカウント減少
			--m_HitStopFrame;
		}
	}



	//　雷エフェクト
	for (int i = 0; i < 5; i++)
	{
		new ThunderEffect
			(m_Params.pos-m_Params.move*0.2f*(float)i, 
			m_Params.pos + Vector3Rand() * 5,
			2.5f,
			0.1f,
			50,
			Vector4(0, 0, 1, 0),
			15);
	}

	//ダメージ関連の更新
	{
		//もしダメージカウントが前回と違った(攻撃があたったならヒットストップフレームを設定)
		if (m_Damage.HitCount != m_HitCountSave)
		{
			m_HitCountSave = m_Damage.HitCount;
			m_HitStopFrame = 5; //適当です
		}

		//ダメージ判定の位置を現在の位置に更新
		UpdateDamageClass();
	}

	//ステージとのあたり判定
	{
		////もし壁に当たっていたらダメージ判定のない状態へ移行する
		//Vector3 NewMoveVec(0, 0, 0);

		//if (UpdateWallCheck(NewMoveVec))
		//{
		//	//新しい移動値をセット
		//	m_Params.move = NewMoveVec;

		//	//攻撃判定のない状態にする
		//	ToNoWork();
		//}

	
	}

	//メッシュ更新
	{
		Matrix m = m_pMeshRenderer->GetMatrix();

		m._41 = m_Params.pos.x;
		m._42 = m_Params.pos.y;
		m._43 = m_Params.pos.z;

		m_pMeshRenderer->SetMatrix(m);

	}

	//フィールド外なら更新失敗
	return !isOutofField();
}


bool Baseball_SpAtk_Ball::StatePhysicMove()
{
	//RigidBodyクラスの行列を自身に適用する
	{
		Matrix M;
		Vector3 PrePos = m_Params.pos;

		m_pRigitBody->Get_TransMatrix(M);

		M = m_BaseMatrix * M;

		m_pMeshRenderer->SetMatrix(M);

		m_Params.pos = Vector3(M._41, M._42, M._43);
		m_Params.move = m_Params.pos - PrePos;

	}


	//寿命管理
	{
	//消滅タイマー
	m_DeleteFrame--;

	//消去時間なら
	if (m_DeleteFrame == 0)
	{
		//フェードアウトして消えるボールクラスを作成する
		iexMesh* pMesh;

		GetBallMesh(m_Params.pParent->m_PlayerInfo.chr_type, &pMesh);

		new BallFadeOutRenderer(
			pMesh,
			m_BaseMatrix,
			m_pRigitBody,
			60
			);

		//自身で開放しないようにnullに設定しておく
		m_pRigitBody = nullptr;
	}
}


	//フィールド外,もしくは消滅タイマーが０　なら更新失敗
	return !isOutofField() && m_DeleteFrame > 0;
}
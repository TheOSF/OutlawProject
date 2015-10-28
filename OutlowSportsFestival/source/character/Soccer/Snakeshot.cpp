#include "Snakeshot.h"
#include "../../character/CharacterBase.h"
#include "../../Render/MeshRenderer.h"
#include "Collision/Collision.h"

#include "Camera/Camera.h"

#include "character/CharacterFunction.h"
#include "character/CharacterManager.h"
#include "Ball/BallFadeOutRenderer.h"

Snakeshot::Snakeshot(
	BallBase::Params	params,			//ボールパラメータ
	float				damage_val   	//ダメージ量
	) :
	m_DeleteFrame(180),
	m_Locus(30),
	m_pRigitBody(nullptr)
{
	LPIEXMESH		pBallMesh;

	m_Locus.m_Division = 1;

	//パラメータ代入
	m_Params = params;
	
	//ダメージ判定のパラメータを代入
	m_Damage.pBall = this;
	m_Damage.pParent = params.pParent;
	m_Damage.m_Param.size = 1;	//大きさはボールによって異なる可能性がある
	m_Damage.type = DamageBase::Type::_VanishDamage;
	m_Damage.Value = damage_val;
	m_Damage.m_Enable = true;
	UpdateDamageClass();


	//ボールのメッシュを作成
	UsualBall::GetBallMesh(params.pParent->m_PlayerInfo.chr_type, &pBallMesh);

	//メッシュのレンダラー作成(最終的にメッシュを使いまわして描画するべき)
	m_pMeshRenderer = new MeshRenderer(
		pBallMesh,
		false,
		MeshRenderer::RenderType::UseColor
		);


	UpdateMesh();

	//軌跡の設定
	m_Locus.m_Division = 0;
	m_Locus.m_StartParam.Width = 0.8f;
	m_Locus.m_EndParam.Width = 0.1f;


	UpdateLocusColor();

	//物理パラメータ初期化
	PhysicsParam.Friction = 0.8f;
	PhysicsParam.Restitution = 0.25f;
	PhysicsParam.Mass = 1.5f;

	
	m_pStatefunc = &Snakeshot::State_TargetDecision;
}
Snakeshot::~Snakeshot()
{
	delete	m_pMeshRenderer;
}

bool Snakeshot::Update()
{
	(this->*m_pStatefunc)();

	{
		Vector3 v;
		Vector3Cross(v, m_Params.move, DefCamera.GetForward());
		v.Normalize();

		m_Locus.AddPoint(m_Params.pos, v);
	}


	UpdateDamageClass();

	


	return m_pStatefunc != &Snakeshot::State_Delete;
}
bool Snakeshot::Msg(MsgType mt)
{

	return false;
}

void Snakeshot::State_TargetDecision()
{
	//ターゲット選択してState_ToTagetMoveに移行
	//もしターゲットがなければState_NoWorkに移行


	//ターゲット取得
	m_pTarget = CalcTarget();

	//ターゲットがいたら
	if (m_pTarget != nullptr)
	{
		//State_ToTagetMoveに移行
		m_pStatefunc = &Snakeshot::State_ToTagetMove;
	}
	else
	{
		//ターゲットがなければState_NoWorkに移行
		m_pStatefunc = &Snakeshot::State_NoWork;
	}

}

void Snakeshot::State_ToTagetMove()
{
	//ターゲットに向かって移動し、もしターゲットが死んでいたら再度
	//State_TargetDecisionに移行

	//もしターゲットが死んでいたら再度State_TargetDecisionに移行
	if (chr_func::isDie(m_pTarget))
	{
		m_pStatefunc = &Snakeshot::State_TargetDecision;
	}

	//敵に当たっていたら攻撃判定をなくす
	if (m_Damage.HitCount > 0)
	{
		//攻撃判定のない状態にする
		m_pStatefunc = &Snakeshot::State_NoWork;
	}

	if (isHitWall())
	{
		//攻撃判定のない状態にする
		m_pStatefunc = &Snakeshot::State_NoWork;
	}

	//ステージ範囲なら消去ステートへ
	if (isOutofField())
	{
		m_pStatefunc = &Snakeshot::State_Delete;
	}

	Homing(m_pTarget->m_Params.pos);
	UpdateMesh();

}
void Snakeshot::State_NoWork()
{
	//一定時間でState_Deleteに移行

	ToNoWork();
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

		if (m_DeleteFrame == 0)
		{
			iexMesh* pMesh;

			UsualBall::GetBallMesh(m_Params.pParent->m_PlayerInfo.chr_type, &pMesh);

			new BallFadeOutRenderer(
				pMesh,
				m_BaseMatrix,
				m_pRigitBody,
				180
				);

			m_pRigitBody = nullptr;

			m_pStatefunc = &Snakeshot::State_Delete;
		}
	}
	//軌跡
	m_Locus.m_StartParam.Color.w *= 0.95f;

	if (m_Locus.m_StartParam.Color.w < 0.1f)
	{
		m_Locus.m_Visible = false;
	}
	if (m_Locus.m_Visible)
	{
		//軌跡の点を追加
		{
			Vector3 v;
			Vector3Cross(v, m_Params.move, DefCamera.GetForward());
			v.Normalize();

			m_Locus.AddPoint(m_Params.pos, v);
		}
	}
}


void Snakeshot::State_Delete()
{
	//何もしない
}


bool Snakeshot::isOutofField()const
{
	return Vector3Length(m_Params.pos) > 100;
}
void Snakeshot::UpdateDamageClass()
{
	m_Damage.vec = m_Params.move;
	m_Damage.m_Param.pos = m_Params.pos;
}
void Snakeshot::UpdateMesh()
{
	//メッシュのワールド変換行列を更新する

	Matrix m;
	float Ballsize = UsualBall::GetBallScale(m_Params.pParent->m_PlayerInfo.chr_type);

	D3DXMatrixScaling(&m, Ballsize, Ballsize, Ballsize);	//大きさはボールによって変える必要がある

	m._41 = m_Params.pos.x;
	m._42 = m_Params.pos.y;
	m._43 = m_Params.pos.z;

	m_pMeshRenderer->SetMatrix(m);
}
void Snakeshot::UpdateLocusColor()
{
	const DWORD Color = CharacterBase::GetPlayerColor(m_Params.pParent->m_PlayerInfo.number);

    m_Locus.m_StartParam.Color = Vector4(
        float((Color >> 16) & 0xFF)  / 255.f,
        float((Color >> 8)  & 0xFF)  / 255.f,
        float( Color        & 0xFF)  / 255.f, 
        0.5f
        );
	m_Locus.m_StartParam.HDRColor
		= Vector4(
		float((Color >> 16) & 0xFF) / 255.f,
		float((Color >> 8) & 0xFF) / 255.f,
		float(Color & 0xFF) / 255.f,
		0.2f
		);

    m_Locus.m_EndParam.Color = Vector4(1, 1, 1, 0);
}

void Snakeshot::SetHDR()
{
	const DWORD Color = CharacterBase::GetPlayerColor(m_Params.pParent->m_PlayerInfo.number);

	m_pMeshRenderer->m_HDR = Vector3(
		float((Color >> 16) & 0xFF) / 255.f,
		float((Color >> 8) & 0xFF) / 255.f,
		float(Color & 0xFF)       / 255.f
		);
}

bool Snakeshot::isHitWall()
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
		//移動量を反射
		return true;
	}

	return false;
}


void Snakeshot::Counter(CharacterBase* pCounterCharacter)
{
	m_Damage.pParent = m_Params.pParent = pCounterCharacter;
	
	UpdateLocusColor();

	m_pStatefunc = &Snakeshot::State_TargetDecision;
}

void Snakeshot::ToNoWork()
{
	//攻撃判定のない状態にする

	if (m_pRigitBody != nullptr)
	{
		return;
	}

	m_Params.type = BallBase::Type::_DontWork;
	m_Damage.m_Enable = false;

	

	m_BaseMatrix = m_pMeshRenderer->GetMatrix();

	m_BaseMatrix._41 = 0;
	m_BaseMatrix._42 = 0;
	m_BaseMatrix._43 = 0;

	m_pRigitBody = DefBulletSystem.AddRigidSphere(
		PhysicsParam.Mass,
		RigidBody::ct_dynamic,
		m_Params.pos,
		Vector3Zero,
		0.5f,
		0.8f,
		0.2f,
		m_Params.move * 45.0f
		);
}

//　ホーミング計算
void Snakeshot::Homing(Vector3 TargetPos)
{
	//引数の位置に向かって移動する(ホーミング)
	const RADIAN HomingRad = D3DXToRadian(4);
	RADIAN rotate;

	Vector3 v1, v2;
	v1 = m_Params.move;
	v2 = TargetPos - m_Params.pos;

	rotate = Vector3Dot(v1, v2) / (v1.Length()*v2.Length());
	rotate = acosf(rotate);
	rotate = min(HomingRad, rotate);
	
	if (v1.z*v2.x - v1.x*v2.z < 0)
	{
		rotate = -rotate;
	}

	m_Params.move = Vector3RotateAxis(Vector3AxisY, rotate, m_Params.move);
	m_Params.move.Normalize();
	m_Params.move *= 0.5f;
	m_Params.pos += m_Params.move;
}

//　遠距離ターゲット選定
CharacterBase* Snakeshot::CalcTarget()const
{
	Vector3 v1, v2;

	const float HomingAngle = PI / 4;
	float MostNear = 1000;
	float TempLen;
	CharacterBase* pTarget = nullptr;

	//　map代入
	const CharacterManager::CharacterMap& chr_map =
		DefCharacterMgr.GetCharacterMap();
	

	for (auto it = chr_map.begin(); it != chr_map.end(); ++it)
	{
		
		//　死んでるor自分ならcontinue
		if (chr_func::isDie(it->first) ||
			it->first->m_PlayerInfo.number == m_Params.pParent->m_PlayerInfo.number)
		{
			continue;
		}

		//　視野角計算
		chr_func::GetFront(m_Params.pParent, &v1);

		v2 = it->first->m_Params.pos - m_Params.pos;
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

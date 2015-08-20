#include "MilderHoming.h"
#include "../character/CharacterBase.h"
#include "../Render/MeshRenderer.h"

#include "character/CharacterFunction.h"
#include "character/CharacterManager.h"

MilderHoming::MilderHoming(
	BaseballPlayer*		b,
	BallBase::Params	params,			//ボールパラメータ
	Vector3		        t,				//ボールターゲット
	DamageBase::Type	damage_type,	//ダメージ判定のタイプ
	float				damage_val,  	//ダメージ量
	int					number			//ターゲットのナンバー
	) :
	m_FreezeCount(0),
	m_FreezeDeleteFrame(60),
	homingtime(0),
	pBaseball(b)
{

	LPIEXMESH		pBallMesh;

	//パラメータ代入
	m_BallBase.m_Params = params;
	//ターゲット代入
	target = t;
	num = number;
	//　加速度初期化
	pBaseball->setAcc(0.2f);
	//ダメージ判定のパラメータを代入
	m_Damage.pBall = &m_BallBase;
	m_Damage.pParent = params.pParent;
	m_Damage.m_Param.size = 1;	//大きさはボールによって異なる可能性がある
	m_Damage.type = damage_type;
	m_Damage.Value = damage_val;
	UpdateDamageClass();


	//ボールのメッシュを作成
	GetBallMesh(params.pParent->m_PlayerInfo.chr_type, &pBallMesh);

	//メッシュのレンダラー作成(最終的にメッシュを使いまわして描画するべき)
	m_pMeshRenderer = new MeshRenderer(
		pBallMesh,
		true,
        MeshRenderer::RenderType::UseColor
		);

	D3DXQuaternionIdentity(&m_Ballrot);

    UpdateMesh();
}

MilderHoming::~MilderHoming()
{
	delete	m_pMeshRenderer;
}

bool MilderHoming::GetBallMesh(
	CharacterType::Value	type,
	LPIEXMESH*				ppOut)
{
	*ppOut = nullptr;

	switch (type)
	{
	
	case CharacterType::_Baseball:
		*ppOut = new iexMesh("DATA\\CHR\\golf_ball\\golf_ball.imo");
		break;
	case CharacterType::_Americanfootball:
		*ppOut = new iexMesh("DATA\\CHR\\Soccer_ball\\soccer_ball.imo");
		break;
	default:
		break;
	}

	return (*ppOut != nullptr);
}


bool MilderHoming::Update()
{

	homingtime++;
	//　ホーミング時間
	if (homingtime < 60){
		m_BallBase.m_Params = pBaseball->TargetDecision(m_BallBase.m_Params, target);
	}

	m_BallBase.m_Params.pos += m_BallBase.m_Params.move;

	if (isOutofField())
	{
		m_FreezeCount = m_FreezeDeleteFrame;
	}

	UpdateMesh();
	UpdateDamageClass();

	return m_FreezeCount < m_FreezeDeleteFrame;
}


bool MilderHoming::Msg(MsgType mt)
{

	return false;
}


bool MilderHoming::isOutofField()const
{
	return Vector3Length(m_BallBase.m_Params.pos) > 100;
}


void MilderHoming::UpdateDamageClass()
{
	m_Damage.vec = m_BallBase.m_Params.move;
	m_Damage.m_Param.pos = m_BallBase.m_Params.pos;
}

void MilderHoming::UpdateMesh()
{
	//メッシュのワールド変換行列を更新する

	Matrix m;

	D3DXMatrixScaling(&m, 0.009f, 0.009f, 0.009f);	//大きさはボールによって変える必要がある

	m._41 = m_BallBase.m_Params.pos.x;
	m._42 = m_BallBase.m_Params.pos.y;
	m._43 = m_BallBase.m_Params.pos.z;

	m_pMeshRenderer->SetMatrix(m);
}
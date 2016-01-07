#include "BaseballEquip.h"
#include "BaseballPlayer.h"
#include "Stage/PhysicsMeshObject.h"
#include "../../GameSystem/ResourceManager.h"
#include "../../Collision/Collision.h"
#include "../../Ball/BallFadeOutRenderer.h"
#include "../CharacterManager.h"
#include "../CharacterFunction.h"
#include "../../Ball/BallFadeOutRenderer.h"


BaseballEquip::BaseballEquip(BaseballPlayer* b, int BoneNum, MeshType mt) :
m_Baseball(b),
meshType(mt),
m_RotateSpeed(0.15f, 0.05f, 0.05f),
m_Locus(15),
tempflg(true),
Deleteflg(false),
m_Glavity(-0.038f),
m_pRigitBody(nullptr),

m_pStateFunc(&BaseballEquip::StateEquip)
{

	//　消去開始時間
	m_DeleteFrame = DeleteTimeSet;

	//　装備
	Install(mt);

	//　メッシュのスケーリング
	const float MeshScale = 0.1f;
	D3DXMatrixScaling(&m, MeshScale, MeshScale, MeshScale);

	//　プレイヤーについていく
	NormalMove();
	
	//　てかり
	UpdateColor();

}

BaseballEquip::~BaseballEquip()
{
	delete m_pMeshRenderer;

	if (m_pRigitBody != nullptr)
	{
		DefBulletSystem.RemoveRigidBody(m_pRigitBody);
	}
}

bool BaseballEquip::Update()
{
	return (this->*m_pStateFunc)();
}


bool BaseballEquip::Msg(MsgType mt)
{
	return false;
}


//**********************************************
//**********************************************
//**********************************************


//　装備
void BaseballEquip::Install(MeshType mt)
{
	switch (mt)
	{
	case Cap:
		pHeadMesh = DefResource.Get(Resource::MeshType::Soccer_ball);
		m_pMeshRenderer = new MeshRenderer(pHeadMesh, false, MeshRenderer::RenderType::UseColorSpecular);
		break;
	case Helmet:
		pHeadMesh = DefResource.Get(Resource::MeshType::BaseBall_ball);
		m_pMeshRenderer = new MeshRenderer(pHeadMesh, false, MeshRenderer::RenderType::UseColorSpecular);
		break;
	case Grove:
		pArmMesh = DefResource.Get(Resource::MeshType::Amefoot_ball);
		m_pMeshRenderer = new MeshRenderer(pArmMesh, false, MeshRenderer::RenderType::UseColorSpecular);
		break;
	case Bat:
		pArmMesh = DefResource.Get(Resource::MeshType::Tennis_ball);
		m_pMeshRenderer = new MeshRenderer(pArmMesh, false, MeshRenderer::RenderType::UseColorSpecular);
		break;
	default:
		break;
	}
	m_pMeshRenderer->SetMatrix(m);
}

void BaseballEquip::NormalMove()
{
	switch (meshType)
	{
	case Cap:
	case Helmet:
		NormalMove_Head();
		break;
	case Grove:
	case Bat:
		NormalMove_Arm();
		break;
	default:
		break;
	}
}

//　通常移動
void BaseballEquip::NormalMove_Head()
{
	m_Baseball->m_Renderer.GetWorldBoneMatirx(param.BoneMat, 3);

	param.TempPos = Vector3(param.BoneMat._41, param.BoneMat._42, param.BoneMat._43);

	param.Forward = Vector3(param.BoneMat._31, param.BoneMat._32, param.BoneMat._33);
	param.Forward.Normalize();

	param.Pos = param.TempPos + param.Forward*1.2f;

	m._41 = param.Pos.x;
	m._42 = param.Pos.y + 1.2f;
	m._43 = param.Pos.z - 0.5f;

	m_pMeshRenderer->SetMatrix(m);
}

//　通常移動
void BaseballEquip::NormalMove_Arm()
{
	if (meshType == Bat)
	{
		m_Baseball->m_Renderer.GetWorldBoneMatirx(param.BoneMat, 11);
	}
	else
	{
		m_Baseball->m_Renderer.GetWorldBoneMatirx(param.BoneMat, 18);
	}

	param.TempPos = Vector3(param.BoneMat._41, param.BoneMat._42, param.BoneMat._43);

	param.Forward = Vector3(param.BoneMat._31, param.BoneMat._32, param.BoneMat._33);
	param.Forward.Normalize();

	param.Pos = param.TempPos + param.Forward;

	m._41 = param.Pos.x;
	m._42 = param.Pos.y-0.8f;
	m._43 = param.Pos.z;

	m_pMeshRenderer->SetMatrix(m);
}

//　後方に落ちる
void BaseballEquip::ToPhysicMove()
{
	param.Move = m_Baseball->m_Params.move;
	//自身の前方ベクトル
	Vector3 MyFront;
	chr_func::GetFront(m_Baseball, &MyFront);
	//　自身の後方ベクトル
	Vector3 MyBack = -MyFront;
	//　後方に飛ぶように
	param.Move = MyBack*0.2f;
	//　飛ぶ高さ
	param.Move.y = 0.35f;

	m_pStateFunc = &BaseballEquip::StateChangeEquip;
}
bool BaseballEquip::StateEquip()
{
	//　プレイヤーについていく
	NormalMove();

	return true;
}

//　装備品変更
bool BaseballEquip::StateChangeEquip()
{
	//　動き
	UpdateMove();
	//　地面もしくは壁などに当たったらtrueに
	if (UpdateWallCheck())
	{
		
		//　一部リセット
		m_DeleteFrame = DeleteTimeSet;
		Deleteflg = false;

		//　一時保存
		tempflg = m_Baseball->getBatterFlg();

		m_pStateFunc = &BaseballEquip::StateHitGround;
	}

	return true;
}

bool BaseballEquip::StateHitGround()
{
	--m_DeleteFrame;

	UpdatePhysicMove();

	if (m_DeleteFrame <= 0)
	{
		switch (meshType)
		{
		case Cap:
		case Helmet:
			new BallFadeOutRenderer(
				pHeadMesh,
				m_BaseMatrix,
				m_pRigitBody,
				30
				);
			break;
		case Grove:
		case Bat:
			new BallFadeOutRenderer(
				pArmMesh,
				m_BaseMatrix,
				m_pRigitBody,
				30
				);
			break;
		default:
			break;
		}
		
		//自身で開放しないようにnullに設定しておく
		m_pRigitBody = nullptr;

		m_pStateFunc = &BaseballEquip::StateEnd;
	}

	return true;
}

bool BaseballEquip::StateEnd()
{
	return false;
}

//　てかりとか
void BaseballEquip::UpdateColor()
{
	const COLORf Color = CharacterBase::GetPlayerColorF(m_Baseball->m_PlayerInfo.number);

	m_Locus.m_StartParam.Color = Color.toVector4();
	m_Locus.m_StartParam.Color.w = 0.3f;
	m_Locus.m_StartParam.HDRColor = m_Locus.m_StartParam.Color;

	m_Locus.m_StartParam.HDRColor.w = 0.5f;

	m_Locus.m_EndParam.Color = m_Locus.m_StartParam.Color;
	m_Locus.m_EndParam.Color.w = 0;

	m_Locus.m_EndParam.HDRColor = m_Locus.m_StartParam.HDRColor;
	m_Locus.m_EndParam.HDRColor.w = 0;


	{
		//メッシュを光らせる
		COLORf Color = CharacterBase::GetPlayerColorF(m_Baseball->m_PlayerInfo.number);
		m_pMeshRenderer->m_HDR = Vector3(1, 1, 1) * 0.1f;

	}
}


//　動き
void BaseballEquip::UpdateMove()
{
	param.Move.y += m_Glavity;
	param.Pos += param.Move;

	m._41 = param.Pos.x + param.Move.x;
	m._42 = param.Pos.y + param.Move.y;
	m._43 = param.Pos.z + param.Move.z;

	m_pMeshRenderer->SetMatrix(m);
}

void BaseballEquip::UpdatePhysicMove()
{

	m_pRigitBody->Get_TransMatrix(m);

	m = m_BaseMatrix*m;

	m_pMeshRenderer->SetMatrix(m);
}

//　当たり判定
bool BaseballEquip::UpdateWallCheck()
{
	Vector3 Out, Pos(param.Pos), Vec(param.Move);
	float Dist = param.Move.Length()*2.0f + 1.0f;
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
		//　物理パラメータセット
		SetPhysicsParam();
		//　反射
		param.Move = Vector3Refrect(param.Move, Vec);
		return true;
	}
	return false;
}

void BaseballEquip::SetPhysicsParam()
{

	m_BaseMatrix = m_pMeshRenderer->GetMatrix();
	m_BaseMatrix._41 = 0;
	m_BaseMatrix._42 = 0;
	m_BaseMatrix._43 = 0;

	pParam.Mass = 0.5f;
	pParam.Friction = 600.0f;
	pParam.Radius = 0.30f;
	pParam.Restitution = 0.85f;

	m_pRigitBody = DefBulletSystem.AddRigidSphere(
		pParam.Mass,
		RigidBody::ct_dynamic,
		param.Pos,
		Vector3Zero,
		pParam.Radius,
		pParam.Friction,
		pParam.Restitution,
		param.Move * 30.0f
		);
}




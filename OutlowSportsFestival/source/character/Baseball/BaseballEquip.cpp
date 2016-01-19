#include "BaseballEquip.h"
#include "BaseballPlayer.h"
#include "Stage/PhysicsMeshObject.h"
#include "../../GameSystem/ResourceManager.h"
#include "../../Collision/Collision.h"
#include "../../Ball/BallFadeOutRenderer.h"
#include "../CharacterManager.h"
#include "../CharacterFunction.h"
#include "../../Ball/BallFadeOutRenderer.h"


BaseballEquip::BaseballEquip(CharacterRenderer* pRenderer, MeshType mt, float Scale) :
m_pChrRenderer(pRenderer),
m_MeshType(mt),
m_pRigitBody(nullptr),
m_pStateFunc(&BaseballEquip::StateEquip),
m_MeshScale(Scale),
m_DeleteFrame(60)
{

	//　装備
	Install(mt);

    //行列を初期化
    UpdateMeshFromBonePos();
}

BaseballEquip::~BaseballEquip()
{
	delete m_pMeshRenderer;

	if (m_pRigitBody != nullptr)
	{
		DefBulletSystem.RemoveRigidBody(m_pRigitBody);
	}
}

//　解除
void BaseballEquip::Takeoff()
{
    //すでに変わっていたらreturn
    if (m_pStateFunc == &BaseballEquip::StatePhysicMove || 
        m_pStateFunc == &BaseballEquip::StateEnd
        )
    {
        return;
    }

    m_pStateFunc = &BaseballEquip::StatePhysicMove;

    SetPhysicsParam();
}

void BaseballEquip::SetVisible(bool flg)
{
    m_pMeshRenderer->m_Visible = flg;
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
    case MeshType::Cap:
		m_pMesh = DefResource.Get(Resource::MeshType::Hat);
        m_BoneNumber = 5;
        m_MeshScale *= 0.1f;
		break;

    case MeshType::Helmet:
        m_pMesh = DefResource.Get(Resource::MeshType::Helmet);
        m_BoneNumber = 5;
        m_MeshScale *= 0.1f;
		break;

    case MeshType::Grove:
        m_pMesh = DefResource.Get(Resource::MeshType::Mitt);
        m_BoneNumber = 20;
        m_MeshScale *= 0.1f;
		break;

    case MeshType::Bat:
        m_pMesh = DefResource.Get(Resource::MeshType::Bat);
        m_BoneNumber = 32;
        m_MeshScale *= 0.1f;
		break;

	default:
		break;
	}

    m_pMeshRenderer = new MeshRenderer(m_pMesh, false, MeshRenderer::RenderType::UseColor);
}

//　通常移動
void BaseballEquip::UpdateMeshFromBonePos()
{
    Matrix Transform;

    m_pChrRenderer->GetWorldBoneMatirx(Transform, m_BoneNumber);

    switch (m_MeshType)
    {
    case MeshType::Cap:

        break;
    case MeshType::Helmet:

        break;
    case MeshType::Grove:

        break;
    case MeshType::Bat:
        Transform._31 = -Transform._31;
        Transform._32 = -Transform._32;
        Transform._33 = -Transform._33;

        break;
    }

    m_pMeshRenderer->SetMatrix(Transform);
}

//　物理挙動
void BaseballEquip::UpdatePhysicMove()
{
    Matrix M;

    m_pRigitBody->Get_TransMatrix(M);

    {
        for (int i = 0; i < 3; ++i)
        {
            for (int j = 0; j < 3; ++j)
            {
                M(i, j) *= m_MeshScale;
            }
        }
    }

    m_pMeshRenderer->SetMatrix(M);
}

bool BaseballEquip::StateEquip()
{
    UpdateMeshFromBonePos();

    return true;
}

bool BaseballEquip::StatePhysicMove()
{
	--m_DeleteFrame;

	UpdatePhysicMove();

	if (m_DeleteFrame <= 0)
	{

        //フェードアウトレンダラを作成
        new BallFadeOutRenderer(
            m_pMesh,
            Matrix(
                m_MeshScale, 0, 0, 0,
                0, m_MeshScale, 0, 0,
                0, 0, m_MeshScale, 0,
                0, 0, 0, 1
            ),
            m_pRigitBody,
            60
            );

        //自身で開放しないようにnullに設定しておく
        m_pRigitBody = nullptr;

        //終了ステートへ
		m_pStateFunc = &BaseballEquip::StateEnd;
	}

	return true;
}

bool BaseballEquip::StateEnd()
{
	return false;
}

void BaseballEquip::SetPhysicsParam()
{
    const Matrix* ChrMat = &m_pChrRenderer->m_TransMatrix;
    Matrix M = m_pMeshRenderer->GetMatrix();


    switch (m_MeshType)
    {
    case MeshType::Cap:
        m_pRigitBody = DefBulletSystem.AddRigidSphere(
            1.0f,
            RigidBody::CollisionTypes::ct_dynamic,
            Vector3(M._41, M._42, M._43),
            Vector3(0, 0, 0),
            1.0f,
            5.0f,
            0.2f,
            -Vector3Normalize(Vector3(ChrMat->_31, ChrMat->_32, ChrMat->_33))*0.5f
            );
        break;

    case MeshType::Helmet:
        m_pRigitBody = DefBulletSystem.AddRigidSphere(
            1.0f,
            RigidBody::CollisionTypes::ct_dynamic,
            Vector3(M._41, M._42, M._43),
            Vector3(0, 0, 0),
            1.0f,
            5.0f,
            0.2f,
            -Vector3Normalize(Vector3(ChrMat->_31, ChrMat->_32, ChrMat->_33))*0.5f
            );
        break;

    case MeshType::Grove:
        m_pRigitBody = DefBulletSystem.AddRigidSphere(
            1.0f,
            RigidBody::CollisionTypes::ct_dynamic,
            Vector3(M._41, M._42, M._43),
            Vector3(0, 0, 0),
            1.0f,
            5.0f,
            0.2f,
            -Vector3Normalize(Vector3(ChrMat->_31, ChrMat->_32, ChrMat->_33))*0.5f
            );
        break;

    case MeshType::Bat:
        m_pRigitBody = DefBulletSystem.AddRigidBox(
            5.0f,
            RigidBody::CollisionTypes::ct_dynamic,
            Vector3(M._41, M._42, M._43),
            Vector3(0, 0, 0),
            Vector3(0.35f, 2.5f, 0.35f),
            5.0f,
            0.2f,
            -Vector3Normalize(Vector3(ChrMat->_31, ChrMat->_32, ChrMat->_33))*0.5f
            );
        break;
    }

}




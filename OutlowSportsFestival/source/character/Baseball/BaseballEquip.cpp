#include "BaseballEquip.h"
#include "BaseballPlayer.h"
#include "Stage/PhysicsMeshObject.h"
#include "../../GameSystem/ResourceManager.h"
#include "../../Collision/Collision.h"
#include "../../Ball/BallFadeOutRenderer.h"
#include "../CharacterManager.h"
#include "../CharacterFunction.h"
#include "../../Ball/BallFadeOutRenderer.h"


BaseballEquip::BaseballEquip(BaseballPlayer* b) :
m_Baseball(b),
m_RotateSpeed(0.15f, 0.05f, 0.05f),
m_Locus(15),
tempflg(true),
Deleteflg(false),
m_Glavity(-0.038f),
m_pRigitBody(nullptr),

m_pStateFunc(&BaseballEquip::StateEquip)
{
	class PhysicUpdateCallBack :public MeshRenderer::PreRenderCallBack
	{
		RigidBody* const pRigidBody;
	public:
		PhysicUpdateCallBack(RigidBody* const pRigidBody) :pRigidBody(pRigidBody){}

		void Execute(MeshRenderer* pRenderer, Type type)
		{

		}
	};

	//�@�����J�n����
	m_DeleteFrame = DeleteTimeSet;

	//�@�؂�ւ�
	ChangeEquip();

	const float MeshScale = 0.1f;

	D3DXMatrixScaling(&m, MeshScale, MeshScale, MeshScale);

	//�@�\���ʒu�֘A
	EquipPos();

	//RendererSetMatrix(m);
	//�@�Ă���
	UpdateColor();

}

BaseballEquip::~BaseballEquip()
{
	delete m_pHelmetMeshRenderer;
	delete m_pHatMeshRenderer;

	if (m_pRigitBody != nullptr)
	{
		DefBulletSystem.RemoveRigidBody(m_pRigitBody);
	}
}

//�@�Ă���Ƃ�
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
		//���b�V�������点��
		COLORf Color = CharacterBase::GetPlayerColorF(m_Baseball->m_PlayerInfo.number);
		RendererHdr();

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

void BaseballEquip::ToPhysicMove()
{
	param.Move = m_Baseball->m_Params.move;
	//���g�̑O���x�N�g��
	Vector3 MyFront;
	chr_func::GetFront(m_Baseball, &MyFront);
	//�@���g�̌���x�N�g��
	Vector3 MyBack = -MyFront;
	//�@����ɔ�Ԃ悤��
	param.Move = MyBack*0.2f;
	//�@��ԍ���
	param.Move.y = 0.35f;
	//�@�����p�����[�^�Z�b�g
	SetPhysicsParam();

	GetEquipMesh(m_Baseball->getBatterFlg(), &pMesh);

	//�@�؂�ւ�
	ChangeEquip();

	m_pStateFunc = &BaseballEquip::StateChangeEquip;
}

bool BaseballEquip::StateEquip()
{
	//�@�\���ʒu�X�V
	EquipPos();

	return true;
}

//�@�����i�ύX
bool BaseballEquip::StateChangeEquip()
{
	//�@�n�ʂ������͕ǂȂǂɓ�����������Ń^�C�}�[�����J�n
	if (Deleteflg && m_DeleteFrame > 0)
	{
		m_DeleteFrame--;
	}

	//�@�n�ʂ�荂�������Ŏ��Ԃ�0����Ȃ�������
	if (param.Pos.y >= 0.1f && m_DeleteFrame > 0)
	{
		//�@����
		UpdateMove();
		//�@�n�ʂ������͕ǂȂǂɓ���������true��
		if (UpdateWallCheck())
		{
			Deleteflg = true;
		}

	}
	else
	{
		//�t�F�[�h�A�E�g���ď�����{�[���N���X���쐬����
		/*{
			Vector3 PrePos = param.Pos;

			m_pRigitBody->Get_TransMatrix(m);

			m = m_BaseMatrix * m;

			RendererSetMatrix();

			param.Pos = Vector3(m._41, m._42, m._43);
			param.Move = param.Pos - PrePos;
		}*/

		
		new BallFadeOutRenderer(
			pMesh,
			m_BaseMatrix,
			m_pRigitBody,
			60
			);

		//���g�ŊJ�����Ȃ��悤��null�ɐݒ肵�Ă���
		m_pRigitBody = nullptr;
		//�@�ꕔ���Z�b�g
		m_DeleteFrame = DeleteTimeSet;
		Deleteflg = false;

		//�@�\���ʒu���X�V
		/*	EquipPos();
			RendererSetMatrix();*/

		//�@�O��delete
		//RenderDelete();

		//�@�ꎞ�ۑ�
		tempflg = m_Baseball->getBatterFlg();

		m_pStateFunc = &BaseballEquip::StateEquip;
	}
	return true;
}

//�@�\���ʒu���X�V(���̏�)
void BaseballEquip::EquipPos()
{
	m_Baseball->m_Renderer.GetWorldBoneMatirx(param.BoneMat, 3);

	param.TempPos = Vector3(param.BoneMat._41, param.BoneMat._42, param.BoneMat._43);

	param.Forward = Vector3(param.BoneMat._31, param.BoneMat._32, param.BoneMat._33);
	param.Forward.Normalize();

	param.Pos = param.TempPos + param.Forward*1.2f;

	m._41 = param.Pos.x;
	m._42 = param.Pos.y + 1.2f;
	m._43 = param.Pos.z - 0.5f;

	RendererSetMatrix();

}

//�@�����i�ύX
void BaseballEquip::ChangeEquip()
{
	//�@���݂̃��[�h�ɑΉ����������i
	if (m_Baseball->getBatterFlg())
	{
		//�@�o�b�^�[
		pHelmetMesh = DefResource.Get(Resource::MeshType::Soccer_ball);
		m_pHelmetMeshRenderer = new MeshRenderer(
			pHelmetMesh,
			false,
			MeshRenderer::RenderType::UseColorSpecular
			);
		m_pHelmetMeshRenderer->SetMatrix(m);
	}
	else
	{
		//�@�s�b�`���[
		pHatMesh = DefResource.Get(Resource::MeshType::BaseBall_ball);
		m_pHatMeshRenderer = new MeshRenderer(
			pHatMesh,
			false,
			MeshRenderer::RenderType::UseColorSpecular
			);
		m_pHatMeshRenderer->SetMatrix(m);
	}

}

//�@����
void BaseballEquip::UpdateMove()
{
	//�@�ő�l�ݒ�
	const float MaxMove = 0.06f;
	if (m_Baseball->m_Params.move.Length() > MaxMove)
	{
		m_Baseball->m_Params.move.Normalize();
		m_Baseball->m_Params.move *= MaxMove;
	}

	param.Move.y += m_Glavity;
	param.Pos += param.Move;

	m._41 = param.Pos.x + param.Move.x;
	m._42 = param.Pos.y + param.Move.y;
	m._43 = param.Pos.z + param.Move.z;

	RendererSetMatrix();
}

//�@�����蔻��
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
		//�@����
		param.Move = Vector3Refrect(param.Move, Vec);
		return true;
	}
	return false;
}

bool BaseballEquip::GetEquipMesh(bool batter, LPIEXMESH* ppOut)
{
	*ppOut = nullptr;
	if (batter)
	{
		*ppOut = DefResource.Get(Resource::MeshType::Soccer_ball);
	}
	else
	{
		*ppOut = DefResource.Get(Resource::MeshType::BaseBall_ball);
	}
	return (*ppOut != nullptr);
}

void BaseballEquip::SetPhysicsParam()
{
	/*if (m_pRigitBody == nullptr)
	{
	return;
	}*/

	m_BaseMatrix = RendererGetMatrix();
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

void BaseballEquip::RendererSetMatrix()
{
	if (tempflg)
	{
		m_pHelmetMeshRenderer->SetMatrix(m);
	}
	else
	{
		m_pHatMeshRenderer->SetMatrix(m);
	}

}

void BaseballEquip::RendererHdr()
{
	if (tempflg)
	{
		m_pHelmetMeshRenderer->m_HDR = Vector3(1, 1, 1) * 0.1f;
	}
	else
	{
		m_pHatMeshRenderer->m_HDR = Vector3(1, 1, 1) * 0.1f;
	}
}

Matrix BaseballEquip::RendererGetMatrix()
{
	if (tempflg)
	{
		return m_pHelmetMeshRenderer->GetMatrix();
	}
	else
	{
		return m_pHatMeshRenderer->GetMatrix();
	}

}

void BaseballEquip::RenderDelete()
{
	if (tempflg)
	{
		delete m_pHelmetMeshRenderer;
	}
	else
	{
		delete m_pHatMeshRenderer;
	}
}
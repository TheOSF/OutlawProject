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
	BallBase::Params	params,			//�{�[���p�����[�^
	DamageBase::Type	damage_type,	//�_���[�W����̃^�C�v
	float				damage_val,		//�_���[�W��
	UINT                hit_num         //�q�b�g��
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

	//�p�����[�^���
	m_Params = params;

	{
		//�_���[�W����̃p�����[�^����
		m_Damage.pBall = this;
		m_Damage.pParent = params.pParent;
		m_Damage.m_Param.width = 1;	//�傫���̓{�[���ɂ���ĈقȂ�?
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

		//�{�[���̃��b�V�����쐬
		GetBallMesh(params.pParent->m_PlayerInfo.chr_type, &pBallMesh);

		//���b�V���̃����_���[�쐬(�ŏI�I�Ƀ��b�V�����g���܂킵�ĕ`�悷��ׂ�)
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

	//�J�����̂��
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
	CharacterType::Value	type    //�{�[���̃L�����N�^�^�C�v
	)
{
	switch (type)
	{

	case CharacterType::_Baseball:
		return 0.0045f;

	default:
		break;
	}

	MyAssert(false, "���݂��Ȃ��^�C�v�̃L�����N�^�^�C�v��UsualBall::GetBallScale�ɓn����܂����@type= %d ", (int)type);

	return 0;
}

Baseball_SpAtk_Ball::PhysicsParam Baseball_SpAtk_Ball::GetBallPhysics(
	CharacterType::Value	type	//�{�[���̃L�����N�^�^�C�v
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

	MyAssert((int)type >= 0 && (int)type < (int)ARRAYSIZE(params), "���݂��Ȃ��^�C�v�̃L�����N�^�^�C�v��BaseBall_SpAtk_Ball::GetBallPhysics�ɓn����܂����@type= %d ", (int)type);

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
	//���łɍU������̂Ȃ���ԂɂȂ��Ă�����return
	if (m_Params.type == BallBase::Type::_DontWork)
	{
		return;
	}

	//�U������̂Ȃ���Ԃɂ���
	m_Params.type = BallBase::Type::_DontWork;
	m_Damage.m_Enable = false;

	//�X�e�[�g�֐���؂�ւ�
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

	//�ړ��X�V
	{
		//�q�b�g�X�g�b�v�t���[���Ȃ�ړ��X�V�����Ȃ�
		if (m_HitStopFrame == 0)
		{
			m_Params.pos += m_Params.move;
		}
		else
		{
			//�q�b�g�X�g�b�v�J�E���g����
			--m_HitStopFrame;
		}
	}



	//�@���G�t�F�N�g
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

	//�_���[�W�֘A�̍X�V
	{
		//�����_���[�W�J�E���g���O��ƈ����(�U�������������Ȃ�q�b�g�X�g�b�v�t���[����ݒ�)
		if (m_Damage.HitCount != m_HitCountSave)
		{
			m_HitCountSave = m_Damage.HitCount;
			m_HitStopFrame = 5; //�K���ł�
		}

		//�_���[�W����̈ʒu�����݂̈ʒu�ɍX�V
		UpdateDamageClass();
	}

	//�X�e�[�W�Ƃ̂����蔻��
	{
		////�����ǂɓ������Ă�����_���[�W����̂Ȃ���Ԃֈڍs����
		//Vector3 NewMoveVec(0, 0, 0);

		//if (UpdateWallCheck(NewMoveVec))
		//{
		//	//�V�����ړ��l���Z�b�g
		//	m_Params.move = NewMoveVec;

		//	//�U������̂Ȃ���Ԃɂ���
		//	ToNoWork();
		//}

	
	}

	//���b�V���X�V
	{
		Matrix m = m_pMeshRenderer->GetMatrix();

		m._41 = m_Params.pos.x;
		m._42 = m_Params.pos.y;
		m._43 = m_Params.pos.z;

		m_pMeshRenderer->SetMatrix(m);

	}

	//�t�B�[���h�O�Ȃ�X�V���s
	return !isOutofField();
}


bool Baseball_SpAtk_Ball::StatePhysicMove()
{
	//RigidBody�N���X�̍s������g�ɓK�p����
	{
		Matrix M;
		Vector3 PrePos = m_Params.pos;

		m_pRigitBody->Get_TransMatrix(M);

		M = m_BaseMatrix * M;

		m_pMeshRenderer->SetMatrix(M);

		m_Params.pos = Vector3(M._41, M._42, M._43);
		m_Params.move = m_Params.pos - PrePos;

	}


	//�����Ǘ�
	{
	//���Ń^�C�}�[
	m_DeleteFrame--;

	//�������ԂȂ�
	if (m_DeleteFrame == 0)
	{
		//�t�F�[�h�A�E�g���ď�����{�[���N���X���쐬����
		iexMesh* pMesh;

		GetBallMesh(m_Params.pParent->m_PlayerInfo.chr_type, &pMesh);

		new BallFadeOutRenderer(
			pMesh,
			m_BaseMatrix,
			m_pRigitBody,
			60
			);

		//���g�ŊJ�����Ȃ��悤��null�ɐݒ肵�Ă���
		m_pRigitBody = nullptr;
	}
}


	//�t�B�[���h�O,�������͏��Ń^�C�}�[���O�@�Ȃ�X�V���s
	return !isOutofField() && m_DeleteFrame > 0;
}
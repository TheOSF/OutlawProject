#include "Snakeshot.h"
#include "../../character/CharacterBase.h"
#include "../../Render/MeshRenderer.h"
#include "Collision/Collision.h"

#include "Camera/Camera.h"

#include "character/CharacterFunction.h"
#include "character/CharacterManager.h"
#include "Ball/BallFadeOutRenderer.h"

Snakeshot::Snakeshot(
	BallBase::Params	params,			//�{�[���p�����[�^
	float				damage_val   	//�_���[�W��
	) :
	m_DeleteFrame(180),
	m_Locus(30),
	m_pRigitBody(nullptr)
{
	LPIEXMESH		pBallMesh;

	m_Locus.m_Division = 1;

	//�p�����[�^���
	m_Params = params;
	
	//�_���[�W����̃p�����[�^����
	m_Damage.pBall = this;
	m_Damage.pParent = params.pParent;
	m_Damage.m_Param.size = 1;	//�傫���̓{�[���ɂ���ĈقȂ�\��������
	m_Damage.type = DamageBase::Type::_VanishDamage;
	m_Damage.Value = damage_val;
	m_Damage.m_Enable = true;
	UpdateDamageClass();


	//�{�[���̃��b�V�����쐬
	UsualBall::GetBallMesh(params.pParent->m_PlayerInfo.chr_type, &pBallMesh);

	//���b�V���̃����_���[�쐬(�ŏI�I�Ƀ��b�V�����g���܂킵�ĕ`�悷��ׂ�)
	m_pMeshRenderer = new MeshRenderer(
		pBallMesh,
		false,
		MeshRenderer::RenderType::UseColor
		);


	UpdateMesh();

	//�O�Ղ̐ݒ�
	m_Locus.m_Division = 0;
	m_Locus.m_StartParam.Width = 0.8f;
	m_Locus.m_EndParam.Width = 0.1f;


	UpdateLocusColor();

	//�����p�����[�^������
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
	//�^�[�Q�b�g�I������State_ToTagetMove�Ɉڍs
	//�����^�[�Q�b�g���Ȃ����State_NoWork�Ɉڍs


	//�^�[�Q�b�g�擾
	m_pTarget = CalcTarget();

	//�^�[�Q�b�g��������
	if (m_pTarget != nullptr)
	{
		//State_ToTagetMove�Ɉڍs
		m_pStatefunc = &Snakeshot::State_ToTagetMove;
	}
	else
	{
		//�^�[�Q�b�g���Ȃ����State_NoWork�Ɉڍs
		m_pStatefunc = &Snakeshot::State_NoWork;
	}

}

void Snakeshot::State_ToTagetMove()
{
	//�^�[�Q�b�g�Ɍ������Ĉړ����A�����^�[�Q�b�g������ł�����ēx
	//State_TargetDecision�Ɉڍs

	//�����^�[�Q�b�g������ł�����ēxState_TargetDecision�Ɉڍs
	if (chr_func::isDie(m_pTarget))
	{
		m_pStatefunc = &Snakeshot::State_TargetDecision;
	}

	//�G�ɓ������Ă�����U��������Ȃ���
	if (m_Damage.HitCount > 0)
	{
		//�U������̂Ȃ���Ԃɂ���
		m_pStatefunc = &Snakeshot::State_NoWork;
	}

	if (isHitWall())
	{
		//�U������̂Ȃ���Ԃɂ���
		m_pStatefunc = &Snakeshot::State_NoWork;
	}

	//�X�e�[�W�͈͂Ȃ�����X�e�[�g��
	if (isOutofField())
	{
		m_pStatefunc = &Snakeshot::State_Delete;
	}

	Homing(m_pTarget->m_Params.pos);
	UpdateMesh();

}
void Snakeshot::State_NoWork()
{
	//��莞�Ԃ�State_Delete�Ɉڍs

	ToNoWork();
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
	//�O��
	m_Locus.m_StartParam.Color.w *= 0.95f;

	if (m_Locus.m_StartParam.Color.w < 0.1f)
	{
		m_Locus.m_Visible = false;
	}
	if (m_Locus.m_Visible)
	{
		//�O�Ղ̓_��ǉ�
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
	//�������Ȃ�
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
	//���b�V���̃��[���h�ϊ��s����X�V����

	Matrix m;
	float Ballsize = UsualBall::GetBallScale(m_Params.pParent->m_PlayerInfo.chr_type);

	D3DXMatrixScaling(&m, Ballsize, Ballsize, Ballsize);	//�傫���̓{�[���ɂ���ĕς���K�v������

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
		//�ړ��ʂ𔽎�
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
	//�U������̂Ȃ���Ԃɂ���

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

//�@�z�[�~���O�v�Z
void Snakeshot::Homing(Vector3 TargetPos)
{
	//�����̈ʒu�Ɍ������Ĉړ�����(�z�[�~���O)
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

//�@�������^�[�Q�b�g�I��
CharacterBase* Snakeshot::CalcTarget()const
{
	Vector3 v1, v2;

	const float HomingAngle = PI / 4;
	float MostNear = 1000;
	float TempLen;
	CharacterBase* pTarget = nullptr;

	//�@map���
	const CharacterManager::CharacterMap& chr_map =
		DefCharacterMgr.GetCharacterMap();
	

	for (auto it = chr_map.begin(); it != chr_map.end(); ++it)
	{
		
		//�@����ł�or�����Ȃ�continue
		if (chr_func::isDie(it->first) ||
			it->first->m_PlayerInfo.number == m_Params.pParent->m_PlayerInfo.number)
		{
			continue;
		}

		//�@����p�v�Z
		chr_func::GetFront(m_Params.pParent, &v1);

		v2 = it->first->m_Params.pos - m_Params.pos;
		v2.y = 0;

		//�p�x�O�Ȃ�K���Ă��Ȃ�
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

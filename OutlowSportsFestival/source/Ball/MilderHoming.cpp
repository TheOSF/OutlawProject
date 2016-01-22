#include "MilderHoming.h"
#include "../character/CharacterBase.h"
#include "../Render/MeshRenderer.h"
#include "Collision/Collision.h"
#include "../Stage/PhysicsMeshObject.h"
#include "Camera/Camera.h"
#include "../Effect/ParticleHDRRenderer.h"
#include "character/CharacterFunction.h"
#include "character/CharacterManager.h"
#include "BallFadeOutRenderer.h"
#include "../GameSystem/ResourceManager.h"
#include "../Effect/EffectFactory.h"
//
////�@�ő�����x
static const float  MaxAcceleration = 1.4f;
//
//const float MilderHoming::AddSkillValueRatio = 0.01f;
//
//MilderHoming::MilderHoming(
//	BallBase::Params	params,			//�{�[���p�����[�^
//	float				damage_val,   	//�_���[�W��
//	BaseballPlayer* b
//	) :
//	acc(0.21f),
//	homingcounter(0),
//	m_DeleteFrame(180),
//	m_Locus(25),
//	frontflg(true),
//	m_Baseball(b),
//	m_EffectFrameCount(0),
//	m_FirstParentType(params.pParent->m_PlayerInfo.chr_type),
//	m_pRigitBody(nullptr)
//{
//	LPIEXMESH		pBallMesh;
//
//	m_Locus.m_Division = 1;
//
//	//�p�����[�^���
//	m_Params = params;
//	{
//		//�_���[�W����̃p�����[�^����
//		m_Damage.pBall = this;
//		m_Damage.pParent = params.pParent;
//		m_Damage.m_Param.width = 0.75f;	//�傫���̓{�[���ɂ���ĈقȂ�?
//		m_Damage.type = DamageBase::Type::_WeekDamage;
//		m_Damage.Value = damage_val;
//		m_Damage.m_Enable = true;
//		m_Damage.m_Param.pos1 = m_Params.pos;
//		m_Damage.m_Param.pos2 = m_Params.pos;
//		m_Damage.m_VecPower.x = 0.4f;
//		m_Damage.m_VecPower.y = 0.2f;
//
//		UpdateDamageClass();
//	}
//
//	{
//		const float MeshScale = GetBallScale(m_FirstParentType);
//		Matrix m;
//
//		//�{�[���̃��b�V�����쐬
//		GetBallMesh(params.pParent->m_PlayerInfo.chr_type, &pBallMesh);
//
//		//���b�V���̃����_���[�쐬
//		m_pMeshRenderer = new MeshRenderer(
//			pBallMesh,
//			false,
//			MeshRenderer::RenderType::UseColorSpecular
//			);
//
//		D3DXMatrixScaling(&m, MeshScale, MeshScale, MeshScale);
//
//		m._41 = m_Params.pos.x;
//		m._42 = m_Params.pos.y;
//		m._43 = m_Params.pos.z;
//
//		m_pMeshRenderer->SetMatrix(m);
//	}
//	
//
//	//�O�Ղ̐ݒ�
//	m_Locus.m_Division = 0;
//	m_Locus.m_StartParam.Width = 0.4f;
//	m_Locus.m_EndParam.Width = 0.1f;
//
//	UpdateLocusColor();
//
//	m_pStatefunc = &MilderHoming::State_TargetDecision;
//}
//
//MilderHoming::~MilderHoming()
//{
//	delete	m_pMeshRenderer;
//	if (m_pRigitBody != nullptr)
//	{
//		DefBulletSystem.RemoveRigidBody(m_pRigitBody);
//	}
//}
//
//bool MilderHoming::Update()
//{
//	if (chr_func::isDie(m_Baseball))
//	{
//		m_pStatefunc = &MilderHoming::State_Delete;
//	}
//	else
//	{
//		(this->*m_pStatefunc)();
//
//		{
//			Vector3 v;
//			Vector3Cross(v, m_Params.move, DefCamera.GetForward());
//			v.Normalize();
//
//			m_Locus.AddPoint(m_Params.pos, v);
//		}
//
//
//		UpdateDamageClass();
//
//		UpdateMesh();
//	}
//
//	return m_pStatefunc != &MilderHoming::State_Delete;
//}
//bool MilderHoming::Msg(MsgType mt)
//{
//
//	return false;
//}
//
//
//UsualBall::MoveControll* MilderHoming::GetMilderHomingMove()
//{
//    class MilderHomingMoveClass :public UsualBall::MoveControll
//    {
//
//        float acc = 0.21f;
//        CharacterBase* pTarget = nullptr;
//    public:
//
//        void Move(UsualBall* pBall)
//        {
//            //�^�[�Q�b�g�v�Z
//            if (pTarget == nullptr)
//            {
//                CharacterBase* pTargetTemp = nullptr;
//
//                if (chr_func::CalcAtkTarget(
//                    pBall->m_Params.pParent,
//                    pBall->m_Params.pos,
//                    Vector3Normalize(pBall->m_Params.move),
//                    PI / 4,
//                    1000.0f,
//                    &pTargetTemp
//                    ))
//                {
//                    pTarget = pTargetTemp;
//                }
//            }
//
//            //�^�[�Q�b�g�������ꍇ�z�[�~���O
//            if (pTarget != nullptr)
//            {
//                const RADIAN HomingRad = D3DXToRadian(1);
//                RADIAN rotate;
//
//                Vector3 v1, v2;
//                v1 = pBall->m_Params.move;
//                v2 = pTarget->m_Params.pos - pBall->m_Params.pos;
//                v2.y = 0.0f;
//
//                rotate = Vector3Dot(v1, v2) / (v1.Length()*v2.Length());
//                rotate = acosf(rotate);
//                rotate = min(HomingRad, rotate);
//
//                if (v1.z*v2.x - v1.x*v2.z < 0)
//                {
//                    rotate = -rotate;
//                }
//
//                pBall->m_Params.move = Vector3RotateAxis(Vector3AxisY, rotate, pBall->m_Params.move);
//            }
//
//            //����
//            {
//                //�@�����x����
//                if (acc <= 0.5f)
//                {
//                    acc += 0.01f;
//                }
//                //�@���ȏ�ł���ɉ���&�z�[�~���O�J�n
//                else
//                {
//                    acc += 0.02f;
//                }
//                //�@�ő�����x
//                if (acc >= MaxAcceleration){
//                    acc = MaxAcceleration;
//                }
//            }
//            
//            //�O�i
//            pBall->m_Params.move.Normalize();
//            pBall->m_Params.move *= 0.7f*acc;
//            pBall->m_Params.pos += pBall->m_Params.move;
//        }
//    };
//
//    return new MilderHomingMoveClass();
//}
//
//void MilderHoming::State_TargetDecision()
//{
//
//	//�^�[�Q�b�g�擾
//	m_pTarget = CalcTarget();
//
//	//�^�[�Q�b�g��������
//	if (m_pTarget != nullptr)
//	{
//		//State_ToTagetMove�Ɉڍs
//		m_pStatefunc = &MilderHoming::State_ToTagetMove;
//	}
//	else
//	{
//		//�^�[�Q�b�g���Ȃ����State_Normal�Ɉڍs
//		m_pStatefunc = &MilderHoming::State_Normal;
//	}
//
//}
//
//void MilderHoming::State_ToTagetMove()
//{
//
//	//�����^�[�Q�b�g������ł�����ēxState_TargetDecision�Ɉڍs
//	if (chr_func::isDie(m_pTarget))
//	{
//		m_pStatefunc = &MilderHoming::State_TargetDecision;
//	}
//
//	//�@�����蔻��Ƃ�
//	Cheak();
//
//	//�@�ǔ��I
//	Homing(m_pTarget->m_Params.pos);
//
//
//}
//
//void MilderHoming::State_NoWork()
//{
//	//��莞�Ԃ�State_Delete�Ɉڍs
//
//	ToNoWork();
//
//
//	Matrix M;
//	Vector3 PrePos = m_Params.pos;
//
//	m_pRigitBody->Get_TransMatrix(M);
//
//	M = m_BaseMatrix * M;
//
//	m_pMeshRenderer->SetMatrix(M);
//
//	m_Params.pos = Vector3(M._41, M._42, M._43);
//	m_Params.move = m_Params.pos - PrePos;
//
//
//	//�O��
//	m_Locus.m_StartParam.Color.w *= 0.95f;
//
//	m_Locus.m_StartParam.HDRColor.w *= 0.95f;
//
//	//���������ȉ��Ȃ�`�悵�Ȃ�
//	if (m_Locus.m_StartParam.Color.w < 0.01f)
//	{
//		m_Locus.m_Visible = false;
//	}
//
//	if (m_Locus.m_Visible)
//	{
//		//�O�Ղ̓_��ǉ�
//		AddLocusPoint();
//	}
//
//	//���Ń^�C�}�[
//	m_DeleteFrame--;
//
//	if (m_DeleteFrame == 0)
//	{
//		iexMesh* pMesh;
//
//		UsualBall::GetBallMesh(m_Params.pParent->m_PlayerInfo.chr_type, &pMesh);
//
//		new BallFadeOutRenderer(
//			pMesh,
//			m_BaseMatrix,
//			m_pRigitBody,
//			60
//			);
//
//		m_pRigitBody = nullptr;
//
//		m_pStatefunc = &MilderHoming::State_Delete;
//	}
//}
//
////�@�^���������
//void MilderHoming::State_Normal()
//{
//	if (chr_func::isDie(m_Baseball))
//	{
//		m_pStatefunc = &MilderHoming::State_NoWork;
//	}
//	else
//	{
//		//�@�����蔻��Ƃ�
//		Cheak();
//		//�ړ��͑O����
//		if (frontflg)
//		{
//			chr_func::GetFront(m_Baseball, &m_Params.move);
//			m_Params.move *= 0.6f;
//			frontflg = false;
//		}
//		//m_Params.move *= 0.7f;
//		m_Params.pos += m_Params.move;
//
//	}
//}
//
//void MilderHoming::State_Counter()
//{
//	if (chr_func::isDie(m_Baseball))
//	{
//		m_pStatefunc = &MilderHoming::State_NoWork;
//	}
//	else
//	{
//		//�@�����蔻��Ƃ�
//		Cheak();
//		//�ړ��͑O����
//		if (frontflg)
//		{
//			m_Params.move *= 0.6f;
//			frontflg = false;
//		}
//		m_Params.pos += m_Params.move;
//
//	}
//}
//void MilderHoming::State_Delete()
//{
//	//�������Ȃ�
//}
//
//void MilderHoming::Cheak()
//{
//	//�G�ɓ������Ă�����U��������Ȃ���
//	if (m_Damage.HitCount > 0)
//	{
//		//�U������̂Ȃ���Ԃɂ���
//		m_pStatefunc = &MilderHoming::State_NoWork;
//	}
//
//	//�����ǂɓ������Ă�����_���[�W����̂Ȃ���Ԃֈڍs����
//	Vector3 NewMoveVec(0, 0, 0);
//
//	//�@�ǂɓ������Ă�����U��������Ȃ���
//	if (isHitWall(NewMoveVec))
//	{
//		//�V�����ړ��l���Z�b�g
//		m_Params.move = NewMoveVec;
//		//�U������̂Ȃ���Ԃɂ���
//		m_pStatefunc = &MilderHoming::State_NoWork;
//	}
//
//	//�X�e�[�W�͈͂Ȃ�����X�e�[�g��
//	if (isOutofField())
//	{
//		m_pStatefunc = &MilderHoming::State_Delete;
//	}
//
//	//�p�[�e�B�N��
//	{
//		const float EffectScale = UsualBall::GetBallScale(m_FirstParentType);
//
//		m_EffectFrameCount = max(m_EffectFrameCount - 1, 0);
//
//		if (m_EffectFrameCount % 2 == 0 && m_EffectFrameCount > 0)
//		{
//			EffectFactory::CircleAnimation(
//				m_Params.pos,
//				m_Params.move,
//				Vector3Zero,
//				Vector3Zero,
//				Vector2(23.f, 23.f)*EffectScale*((float)m_EffectFrameCount / 45.0f),
//				0xFFFFFFFF,
//				CharacterBase::GetPlayerColor(m_Params.pParent->m_PlayerInfo.number)
//				);
//		}
//	}
//}
//
//
//bool MilderHoming::isOutofField()const
//{
//	return Vector3Length(m_Params.pos) > 100;
//}
//void MilderHoming::UpdateDamageClass()
//{
//	m_Damage.m_Vec = m_Params.move;
//
//	m_Damage.m_Param.pos2 = m_Damage.m_Param.pos1;
//
//	{
//		m_Damage.m_Param.pos1 = m_Params.pos;
//
//		if (Vector3Distance(m_Damage.m_Param.pos1, m_Damage.m_Param.pos2) < m_Damage.m_Param.width)
//		{
//			Vector3 v = m_Damage.m_Param.pos2 - m_Damage.m_Param.pos1;
//
//			v.Normalize();
//
//			m_Damage.m_Param.pos2 = m_Damage.m_Param.pos1 + v*m_Damage.m_Param.width;
//		}
//	}
//
//}
//void MilderHoming::UpdateMesh()
//{
//	//���b�V���̃��[���h�ϊ��s����X�V����
//	Matrix m;
//	float s = UsualBall::GetBallScale(CharacterType::_Baseball);
//
//	D3DXMatrixScaling(&m, s, s, s);	//�傫���̓{�[���ɂ���ĕς���K�v������
//
//	m._41 = m_Params.pos.x;
//	m._42 = m_Params.pos.y;
//	m._43 = m_Params.pos.z;
//
//	m_pMeshRenderer->SetMatrix(m);
//}
//void MilderHoming::UpdateLocusColor()
//{
//	const COLORf Color = CharacterBase::GetPlayerColorF(m_Params.pParent->m_PlayerInfo.number);
//
//	m_Locus.m_StartParam.Color = Color.toVector4();
//	m_Locus.m_StartParam.Color.w = 0.3f;
//	m_Locus.m_StartParam.HDRColor = m_Locus.m_StartParam.Color;
//
//	m_Locus.m_StartParam.HDRColor.w = 0.5f;
//
//	m_Locus.m_EndParam.Color = m_Locus.m_StartParam.Color;
//	m_Locus.m_EndParam.Color.w = 0;
//
//	m_Locus.m_EndParam.HDRColor = m_Locus.m_StartParam.HDRColor;
//	m_Locus.m_EndParam.HDRColor.w = 0;
//
//
//	{
//		//���b�V�������点��
//		COLORf Color = CharacterBase::GetPlayerColorF(m_Params.pParent->m_PlayerInfo.number);
//
//		m_pMeshRenderer->m_HDR = Vector3(1, 1, 1) * 0.1f;
//
//	}
//}
//
//void MilderHoming::SetHDR()
//{
//	const DWORD Color = CharacterBase::GetPlayerColor(m_Params.pParent->m_PlayerInfo.number);
//
//	m_pMeshRenderer->m_HDR = Vector3(
//		float((Color >> 16) & 0xFF) / 255.f,
//		float((Color >> 8) & 0xFF) / 255.f,
//		float(Color & 0xFF) / 255.f
//		);
//}
//
//bool MilderHoming::isHitWall(Vector3& outNewMove)
//{
//	Vector3 Out, Pos(m_Params.pos), Vec(m_Params.move);
//	float Dist = m_Params.move.Length()*2.0f;
//	int Material;
//
//	Vec.Normalize();
//
//	if (DefCollisionMgr.RayPick(
//		&Out,
//		&Pos,
//		&Vec,
//		&Dist,
//		&Material,
//		CollisionManager::RayType::_Ball
//		)
//		)
//	{
//		outNewMove = Vector3Refrect(m_Params.move, Vec);
//		outNewMove *= GetBallPhysics(m_FirstParentType).Restitution;
//		//�ړ��ʂ𔽎�
//		return true;
//	}
//
//	return false;
//}
//
//
//void MilderHoming::Counter(CharacterBase* pCounterCharacter)
//{
//
//	m_Damage.pParent = m_Params.pParent = pCounterCharacter;
//
//	UpdateLocusColor();
//
//	m_Damage.m_VecPower.x = Vector3XZLength(m_Params.move)*1.2f;
//	//m_Damage.type = DamageBase::Type::_VanishDamage;
//	m_Damage.Value += 1.0f; //�_���[�W�𑝂₷
//
//	////�G�t�F�N�g�J�E���g�ݒ�
//	m_EffectFrameCount = 45;
//	m_pStatefunc = &MilderHoming::State_Counter;
//
//	
//}
//
//void MilderHoming::ToNoWork()
//{
//	//�U������̂Ȃ���Ԃɂ���
//
//	if (m_pRigitBody != nullptr)
//	{
//		return;
//	}
//
//	m_Params.type = BallBase::Type::_DontWork;
//	m_Damage.m_Enable = false;
//
//	//�X�e�[�g�֐���؂�ւ�
//
//
//	m_BaseMatrix = m_pMeshRenderer->GetMatrix();
//
//	m_BaseMatrix._41 = 0;
//	m_BaseMatrix._42 = 0;
//	m_BaseMatrix._43 = 0;
//
//	const PhysicsParam p = GetBallPhysics(m_FirstParentType);
//
//	m_pRigitBody = DefBulletSystem.AddRigidSphere(
//		p.Mass,
//		RigidBody::ct_dynamic,
//		m_Params.pos,
//		Vector3Zero,
//		0.5f,
//		0.8f,
//		0.25f,
//		m_Params.move * 45.0f
//		);
//
//	//�@������
//	acc = 0.2f;
//	homingcounter = 0;
//	frontflg = true;
//}
//
////�@�z�[�~���O�v�Z
//void MilderHoming::Homing(Vector3 TargetPos)
//{
//
//	//�@�ǔ����ԑ���
//	homingcounter++;
//
//	if (homingcounter <= 120)
//	{
//		//�����̈ʒu�Ɍ������Ĉړ�����(�z�[�~���O)
//		const RADIAN HomingRad = D3DXToRadian(1);
//		RADIAN rotate;
//
//		Vector3 v1, v2;
//		v1 = m_Params.move;
//		v2 = TargetPos - m_Params.pos;
//
//		rotate = Vector3Dot(v1, v2) / (v1.Length()*v2.Length());
//		rotate = acosf(rotate);
//		rotate = min(HomingRad, rotate);
//
//		if (v1.z*v2.x - v1.x*v2.z < 0)
//		{
//			rotate = -rotate;
//		}
//
//		//�@�����x����
//		if (acc <= 0.5f)
//		{
//			acc += 0.01f;
//		}
//		//�@���ȏ�ł���ɉ���&�z�[�~���O�J�n
//		else
//		{
//			acc += 0.02f;
//		}
//		//�@�ő�����x
//		if (acc >= MaxAcceleration){
//			acc = MaxAcceleration;
//		}
//
//		m_Params.move = Vector3RotateAxis(Vector3AxisY, rotate, m_Params.move);
//		m_Params.move.Normalize();
//		m_Params.move *= 0.7f*acc;
//	}
//
//	m_Params.pos += m_Params.move;
//
//}
//
////�@�������^�[�Q�b�g�I��
//CharacterBase* MilderHoming::CalcTarget()const
//{
//	Vector3 v1, v2;
//
//	const float HomingAngle = PI / 4;
//	float MostNear = 1000;
//	float TempLen;
//	CharacterBase* pTarget = nullptr;
//
//	//�@map���
//	const CharacterManager::CharacterMap& chr_map =
//		DefCharacterMgr.GetCharacterMap();
//
//
//	for (auto it = chr_map.begin(); it != chr_map.end(); ++it)
//	{
//
//		//�@����ł�or�����Ȃ�continue
//		if (chr_func::isDie(it->first) ||
//			it->first->m_PlayerInfo.number == m_Params.pParent->m_PlayerInfo.number)
//		{
//			continue;
//		}
//
//		//�@����p�v�Z
//		chr_func::GetFront(m_Params.pParent, &v1);
//
//		v2 = it->first->m_Params.pos - m_Params.pos;
//		v2.y = 0;
//
//		//�p�x�O�Ȃ�K���Ă��Ȃ�
//		if (Vector3Radian(v1, v2) > HomingAngle)
//		{
//			continue;
//		}
//
//		TempLen = v2.Length();
//
//		if (MostNear > TempLen)
//		{
//			MostNear = TempLen;
//			pTarget = it->first;
//		}
//	}
//
//	return pTarget;
//}
//
//
//bool MilderHoming::GetBallMesh(
//	CharacterType::Value	type,
//	LPIEXMESH*				ppOut)
//{
//	*ppOut = nullptr;
//
//	switch (type)
//	{
//	
//	case CharacterType::_Baseball:
//		*ppOut = DefResource.Get(Resource::MeshType::BaseBall_ball);
//		break;
//	
//	default:
//		break;
//	}
//
//	return (*ppOut != nullptr);
//}
//
//float MilderHoming::GetBallScale(
//	CharacterType::Value	type    //�{�[���̃L�����N�^�^�C�v
//	)
//{
//	switch (type)
//	{
//	case CharacterType::_Americanfootball:
//		return 0.18f;
//
//	case CharacterType::_Baseball:
//		return 0.1f;
//
//	case CharacterType::_Lacrosse:
//		return 0.18f;
//
//	case CharacterType::_Soccer:
//		return 0.18f;
//
//
//	case CharacterType::_Tennis:
//		return 0.1f;
//
//
//	case CharacterType::_Volleyball:
//		return 0.18f;
//
//	default:
//		break;
//	}
//
//	MyAssert(false, "���݂��Ȃ��^�C�v�̃L�����N�^�^�C�v��UsualBall::GetBallScale�ɓn����܂����@type= %d ", (int)type);
//
//
//	return 0;
//}
//
//
//MilderHoming::PhysicsParam MilderHoming::GetBallPhysics(
//	CharacterType::Value	type	//�{�[���̃L�����N�^�^�C�v
//	)
//{
//	PhysicsParam params[] =
//	{
//		{ 0.5f, 600.0f, 0.30f, 0.85f },
//		{ 0.5f, 600.0f, 0.28f, 0.85f },
//		{ 1.0f, 600.0f, 0.58f, 0.85f },
//		{ 0.5f, 600.0f, 0.55f, 0.85f },
//		{ 0.5f, 600.0f, 0.55f, 0.85f },
//		{ 0.5f, 600.0f, 0.55f, 0.85f },
//	};
//
//	MyAssert((int)type >= 0 && (int)type < (int)ARRAYSIZE(params), "���݂��Ȃ��^�C�v�̃L�����N�^�^�C�v��UsualBall::GetBallPhysics�ɓn����܂����@type= %d ", (int)type);
//
//	return params[(int)type];
//}
//
//void MilderHoming::AddLocusPoint()
//{
//	Vector3 v;
//	Vector3Cross(v, m_Params.move, DefCamera.GetForward());
//	v.Normalize();
//
//	m_Locus.AddPoint(m_Params.pos, v);
//}
//
//
//bool MilderHoming::StatePhysicMove()
//{
//	//RigidBody�N���X�̍s������g�ɓK�p����
//	{
//		Matrix M;
//		Vector3 PrePos = m_Params.pos;
//
//		m_pRigitBody->Get_TransMatrix(M);
//
//		M = m_BaseMatrix * M;
//
//		m_pMeshRenderer->SetMatrix(M);
//
//		m_Params.pos = Vector3(M._41, M._42, M._43);
//		m_Params.move = m_Params.pos - PrePos;
//
//	}
//
//
//	//�����Ǘ�
//	{
//	//���Ń^�C�}�[
//	m_DeleteFrame--;
//
//	//�������ԂȂ�
//	if (m_DeleteFrame == 0)
//	{
//		//�t�F�[�h�A�E�g���ď�����{�[���N���X���쐬����
//		iexMesh* pMesh;
//
//		GetBallMesh(m_FirstParentType, &pMesh);
//
//		new BallFadeOutRenderer(
//			pMesh,
//			m_BaseMatrix,
//			m_pRigitBody,
//			60
//			);
//
//		//���g�ŊJ�����Ȃ��悤��null�ɐݒ肵�Ă���
//		m_pRigitBody = nullptr;
//	}
//}
//
//	//�O�Ղ̕s�����x�����X�Ɍ��炵�Ă���
//	{
//		//�O��
//		m_Locus.m_StartParam.Color.w *= 0.95f;
//		m_Locus.m_StartParam.HDRColor.w *= 0.95f;
//
//		//���������ȉ��Ȃ�`�悵�Ȃ�
//		if (m_Locus.m_StartParam.Color.w < 0.01f)
//		{
//			m_Locus.m_Visible = false;
//		}
//
//		if (m_Locus.m_Visible)
//		{
//			//�O�Ղ̓_��ǉ�
//			AddLocusPoint();
//		}
//	}
//
//	//���l������
//	{
//		m_pMeshRenderer->m_HDR *= 0.92f;
//	}
//
//
//	//���Ń^�C�}�[���O�@�Ȃ�X�V���s
//	return  m_DeleteFrame > 0;
//}


UsualBall::MoveControll* MilderHoming::GetMilderHomingMove()
{
    class MilderHomingMoveClass :public UsualBall::MoveControll
    {

        float acc = 0.21f;
        CharacterBase* pTarget = nullptr;
        bool Countered = false;
    public:

        void Counter(CharacterBase* pCounterChr, UsualBall* pBall)
        {
            pTarget = nullptr;
            Countered = true;
        }

        void HomingMove(UsualBall* pBall)
        {
            //�^�[�Q�b�g�v�Z
            if (pTarget == nullptr)
            {
                CharacterBase* pTargetTemp = nullptr;

                if (chr_func::CalcAtkTarget(
                    pBall->m_Params.pParent,
                    pBall->m_Params.pos,
                    Vector3Normalize(pBall->m_Params.move),
                    PI / 4,
                    1000.0f,
                    &pTargetTemp
                    ))
                {
                    pTarget = pTargetTemp;
                }
            }

            //�^�[�Q�b�g�������ꍇ�z�[�~���O
            if (pTarget != nullptr)
            {
                const RADIAN HomingRad = D3DXToRadian(1);
                RADIAN rotate;

                Vector3 v1, v2;
                v1 = pBall->m_Params.move;
                v2 = pTarget->m_Params.pos - pBall->m_Params.pos;
                v2.y = 0.0f;

                if (v2.Length() > 0.0f && v1.Length() > 0.0f)
                {
                    rotate = Vector3Dot(v1, v2) / (v1.Length()*v2.Length());
                    rotate = acosf(fClamp(rotate, 1, -1));
                    rotate = min(HomingRad, rotate);

                    if (v1.z*v2.x - v1.x*v2.z < 0)
                    {
                        rotate = -rotate;
                    }

                    pBall->m_Params.move = Vector3RotateAxis(Vector3AxisY, rotate, pBall->m_Params.move);
                }
            }

            //����
            {
                //�@�����x����
                if (acc <= 0.5f)
                {
                    acc += 0.01f;
                }
                //�@���ȏ�ł���ɉ���&�z�[�~���O�J�n
                else
                {
                    acc += 0.02f;
                }
                //�@�ő�����x
                if (acc >= MaxAcceleration){
                    acc = MaxAcceleration;
                }
            }

            //�O�i
            pBall->m_Params.move.Normalize();
            pBall->m_Params.move *= 0.7f*acc;
        }
       
        void Move(UsualBall* pBall)
        {
            if (Countered == false)
            {
                HomingMove(pBall);
            }
            
            pBall->m_Params.pos += pBall->m_Params.move;
        }
    };

    return new MilderHomingMoveClass();
}

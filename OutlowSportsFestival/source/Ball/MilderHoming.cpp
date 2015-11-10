#include "MilderHoming.h"
#include "../character/CharacterBase.h"
#include "../Render/MeshRenderer.h"
#include "Collision/Collision.h"

#include "Camera/Camera.h"

#include "character/CharacterFunction.h"
#include "character/CharacterManager.h"
#include "Ball/BallFadeOutRenderer.h"

#include "../Effect/EffectFactory.h"

//�@�ő�����x
#define MaxAcceleration 1.0f

MilderHoming::MilderHoming(
    BallBase::Params	params,			//�{�[���p�����[�^
    float				damage_val,   	//�_���[�W��
	BaseballPlayer* b
    ) :
    acc(0.2f),
    homingcounter(0),
    m_DeleteFrame(180),
    m_Locus(30),
	frontflg(true),
	bp(b),
	m_EffectFrameCount(0),
	m_FirstParentType(params.pParent->m_PlayerInfo.chr_type),
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
    m_Damage.type = DamageBase::Type::_WeekDamage;
    m_Damage.Value = damage_val;
    m_Damage.m_Enable = true;
    UpdateDamageClass();


    //�{�[���̃��b�V�����쐬
    UsualBall::GetBallMesh(params.pParent->m_PlayerInfo.chr_type, &pBallMesh);

    //���b�V���̃����_���[�쐬(�ŏI�I�Ƀ��b�V�����g���܂킵�ĕ`�悷��ׂ�)
    m_pMeshRenderer = new MeshRenderer(
        pBallMesh,
        false,
        MeshRenderer::RenderType::UseColorSpecular
        );


    UpdateMesh();

    //�O�Ղ̐ݒ�
    m_Locus.m_Division = 0;
    m_Locus.m_StartParam.Width = 0.4f;
    m_Locus.m_EndParam.Width = 0.1f;


    UpdateLocusColor();

    //�����p�����[�^������
    PhysicsParam.Friction = 0.4f;
    PhysicsParam.Restitution = 0.25f;
    PhysicsParam.Mass = 1.5f;


    m_pStatefunc = &MilderHoming::State_TargetDecision;
}

MilderHoming::~MilderHoming()
{
    delete	m_pMeshRenderer;
}

bool MilderHoming::Update()
{
    (this->*m_pStatefunc)();

    {
        Vector3 v;
        Vector3Cross(v, m_Params.move, DefCamera.GetForward());
        v.Normalize();

        m_Locus.AddPoint(m_Params.pos, v);
    }


    UpdateDamageClass();

    UpdateMesh();


    return m_pStatefunc != &MilderHoming::State_Delete;
}
bool MilderHoming::Msg(MsgType mt)
{

    return false;
}

void MilderHoming::State_TargetDecision()
{

    //�^�[�Q�b�g�擾
    m_pTarget = CalcTarget();

    //�^�[�Q�b�g��������
    if (m_pTarget != nullptr)
    {
        //State_ToTagetMove�Ɉڍs
        m_pStatefunc = &MilderHoming::State_ToTagetMove;
    }
    else
    {
        //�^�[�Q�b�g���Ȃ����State_Normal�Ɉڍs
		m_pStatefunc = &MilderHoming::State_Normal;
    }

}

void MilderHoming::State_ToTagetMove()
{

    //�����^�[�Q�b�g������ł�����ēxState_TargetDecision�Ɉڍs
    if (chr_func::isDie(m_pTarget))
    {
        m_pStatefunc = &MilderHoming::State_TargetDecision;
    }

	//�@�����蔻��Ƃ�
	Cheak();

	//�@�ǔ��I
    Homing(m_pTarget->m_Params.pos);

}

void MilderHoming::State_NoWork()
{
    //��莞�Ԃ�State_Delete�Ɉڍs

    ToNoWork();


    Matrix M;
    Vector3 PrePos = m_Params.pos;

    m_pRigitBody->Get_TransMatrix(M);

    M = m_BaseMatrix * M;

    m_pMeshRenderer->SetMatrix(M);

    m_Params.pos = Vector3(M._41, M._42, M._43);
    m_Params.move = m_Params.pos - PrePos;


    //�O��
    m_Locus.m_StartParam.Color.w *= 0.95f;

    if (m_Locus.m_StartParam.Color.w < 0.1f)
    {
        m_Locus.m_Visible = false;
    }

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
            60
            );

        m_pRigitBody = nullptr;

        m_pStatefunc = &MilderHoming::State_Delete;
    }
}

//�@�^���������
void MilderHoming::State_Normal()
{
	//�@�����蔻��Ƃ�
	Cheak();
	//�ړ��͑O����
	if (frontflg)
	{
		chr_func::GetFront(bp, &m_Params.move);
		m_Params.move *= 0.6f;
		frontflg = false;
	}
	//m_Params.move *= 0.7f;
	m_Params.pos += m_Params.move;


}

void MilderHoming::State_Delete()
{
    //�������Ȃ�
}

void MilderHoming::Cheak()
{
	//�G�ɓ������Ă�����U��������Ȃ���
	if (m_Damage.HitCount > 0)
	{
		//�U������̂Ȃ���Ԃɂ���
		m_pStatefunc = &MilderHoming::State_NoWork;
	}

	//�@�ǂɓ������Ă�����U��������Ȃ���
	if (isHitWall())
	{
		//�U������̂Ȃ���Ԃɂ���
		m_pStatefunc = &MilderHoming::State_NoWork;
	}

	//�X�e�[�W�͈͂Ȃ�����X�e�[�g��
	if (isOutofField())
	{
		m_pStatefunc = &MilderHoming::State_Delete;
	}

	//�p�[�e�B�N��
	{
		const float EffectScale = UsualBall::GetBallScale(m_FirstParentType);

		m_EffectFrameCount = max(m_EffectFrameCount - 1, 0);

		if (m_EffectFrameCount % 2 == 0 && m_EffectFrameCount > 0)
		{
			EffectFactory::CircleAnimation(
				m_Params.pos,
				m_Params.move,
				Vector3Zero,
				Vector3Zero,
				Vector2(23.f, 23.f)*EffectScale*((float)m_EffectFrameCount / 45.0f),
				0xFFFFFFFF,
				CharacterBase::GetPlayerColor(m_Params.pParent->m_PlayerInfo.number)
				);
		}
	}
}


bool MilderHoming::isOutofField()const
{
    return Vector3Length(m_Params.pos) > 100;
}
void MilderHoming::UpdateDamageClass()
{
    m_Damage.vec = m_Params.move;
    m_Damage.m_Param.pos = m_Params.pos;
}
void MilderHoming::UpdateMesh()
{
    //���b�V���̃��[���h�ϊ��s����X�V����
    Matrix m;
	float s = UsualBall::GetBallScale(CharacterType::_Baseball);

    D3DXMatrixScaling(&m, s, s, s);	//�傫���̓{�[���ɂ���ĕς���K�v������

    m._41 = m_Params.pos.x;
    m._42 = m_Params.pos.y;
    m._43 = m_Params.pos.z;

    m_pMeshRenderer->SetMatrix(m);
}
void MilderHoming::UpdateLocusColor()
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

void MilderHoming::SetHDR()
{
    const DWORD Color = CharacterBase::GetPlayerColor(m_Params.pParent->m_PlayerInfo.number);

    m_pMeshRenderer->m_HDR = Vector3(
        float((Color >> 16) & 0xFF) / 255.f,
        float((Color >> 8) & 0xFF) / 255.f,
        float(Color & 0xFF) / 255.f
        );
}

bool MilderHoming::isHitWall()
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
        //�ړ��ʂ𔽎�
        return true;
    }

    return false;
}


void MilderHoming::Counter(CharacterBase* pCounterCharacter)
{
    m_Damage.pParent = m_Params.pParent = pCounterCharacter;

    UpdateLocusColor();

	//�@�J�E���^�[���ꂽ�琁�����&�_���[�W1.3�{
	m_Damage.type = DamageBase::Type::_VanishDamage;
	m_Damage.Value += 1.0f; //�_���[�W�𑝂₷

	//�G�t�F�N�g�J�E���g�ݒ�
	m_EffectFrameCount = 45;

    m_pStatefunc = &MilderHoming::State_TargetDecision;
}

void MilderHoming::ToNoWork()
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
        0.25f,
        m_Params.move * 45.0f
        );

    //�@������
    acc = 0.2f;
    homingcounter = 0;
	frontflg = true;
}

//�@�z�[�~���O�v�Z
void MilderHoming::Homing(Vector3 TargetPos)
{
    //�@�ǔ����ԑ���
    homingcounter++;

    if (homingcounter <= 60)
    {
        //�����̈ʒu�Ɍ������Ĉړ�����(�z�[�~���O)
        const RADIAN HomingRad = D3DXToRadian(1);
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

        //�@�����x����
        if (acc <= 0.36f)
        {
            acc += 0.018f;
        }
        //�@���ȏ�ł���ɉ���&�z�[�~���O�J�n
        else
        {
            acc += 0.04f;
        }
        //�@�ő�����x
        if (acc >= MaxAcceleration){
            acc = MaxAcceleration;
        }

        m_Params.move = Vector3RotateAxis(Vector3AxisY, rotate, m_Params.move);
        m_Params.move.Normalize();
        m_Params.move *= 0.7f*acc;
    }
    m_Params.pos += m_Params.move;
}

//�@�������^�[�Q�b�g�I��
CharacterBase* MilderHoming::CalcTarget()const
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

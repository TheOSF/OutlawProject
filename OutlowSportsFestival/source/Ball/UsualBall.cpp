#include "UsualBall.h"
#include "../character/CharacterBase.h"
#include "../Render/MeshRenderer.h"
#include "../Camera/Camera.h"
#include "../GameSystem/ResourceManager.h"
#include "../Stage/PhysicsMeshObject.h"
#include "../Collision/Collision.h"
#include "BallFadeOutRenderer.h"
#include "../Effect/ParticleHDRRenderer.h"
#include "../Effect/ParticleMoveObject.h"
#include "../Effect/EffectFactory.h"


UsualBall::UsualBall(
    BallBase::Params	params,			//�{�[���p�����[�^
    DamageBase::Type	damage_type,	//�_���[�W����̃^�C�v
    float				damage_val,		//�_���[�W��
    UINT                hit_num         //�q�b�g��
    ) :
    m_DeleteFrame(180),
    m_Locus(15),
    m_pRigitBody(nullptr),
    m_HitNum(hit_num),
    m_HitCountSave(0),
    m_HitStopFrame(0),
    m_pStateFunc(&UsualBall::StateFlyMove),
    m_RotateSpeed(0.15f, 0.05f, 0.05f),
    m_EffectFrameCount(0),
    m_FirstParentType(params.pParent->m_PlayerInfo.chr_type)
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

	LPIEXMESH		pBallMesh;

	//�p�����[�^���
    m_Params = params;

    {
        //�_���[�W����̃p�����[�^����
        m_Damage.pBall = this;
        m_Damage.pParent = params.pParent;
        m_Damage.m_Param.width = 2.0f;	//�傫���̓{�[���ɂ���ĈقȂ�?
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
            MeshRenderer::RenderType::UseColorSpecular
            );

        D3DXMatrixScaling(&m, MeshScale, MeshScale, MeshScale);

        m._41 = m_Params.pos.x;
        m._42 = m_Params.pos.y;
        m._43 = m_Params.pos.z;

        m_pMeshRenderer->SetMatrix(m);
    }


    {
        //�O�Ղ̐ݒ�
        m_Locus.m_Division = 0;
        m_Locus.m_StartParam.Width = 3.6f * GetBallScale(params.pParent->m_PlayerInfo.chr_type);
        m_Locus.m_EndParam.Width = 0.1f;

        UpdateColor();
    }

  

}

UsualBall::~UsualBall()
{
    delete m_pMeshRenderer;
    if (m_pRigitBody != nullptr)
    {
        DefBulletSystem.RemoveRigidBody(m_pRigitBody);
    }
}

bool UsualBall::GetBallMesh(
	CharacterType::Value	type,
	LPIEXMESH*				ppOut)
{
	*ppOut = nullptr;

	switch (type)
	{
	case CharacterType::_Tennis:
        *ppOut = DefResource.Get(Resource::MeshType::Tennis_ball);
		break;
	case CharacterType::_Baseball:
        *ppOut = DefResource.Get(Resource::MeshType::BaseBall_ball);
		break;
	case CharacterType::_Americanfootball:
        *ppOut = DefResource.Get(Resource::MeshType::Amefoot_ball);
		break;
	case CharacterType::_Soccer:
        *ppOut = DefResource.Get(Resource::MeshType::Soccer_ball);
		break;
	case CharacterType::_Lacrosse:
        *ppOut = DefResource.Get(Resource::MeshType::Lacrosse_ball);
		break;
	case CharacterType::_Volleyball:
        *ppOut = DefResource.Get(Resource::MeshType::Volley_ball);
	default:
		break;
	}

	return (*ppOut != nullptr);
}


float UsualBall::GetBallScale(
    CharacterType::Value	type    //�{�[���̃L�����N�^�^�C�v
    )
{
    switch (type)
    {
    case CharacterType::_Americanfootball:
        return 0.18f;

    case CharacterType::_Baseball:
        return 0.1f;

    case CharacterType::_Lacrosse:
        return 0.18f;

    case CharacterType::_Soccer:
        return 0.18f;


    case CharacterType::_Tennis:
        return 0.1f;


    case CharacterType::_Volleyball:
        return 0.18f;

    default:
        break;
    }

    MyAssert(false, "���݂��Ȃ��^�C�v�̃L�����N�^�^�C�v��UsualBall::GetBallScale�ɓn����܂����@type= %d ", (int)type);


    return 0;
}

UsualBall::PhysicsParam UsualBall::GetBallPhysics(
    CharacterType::Value	type	//�{�[���̃L�����N�^�^�C�v
    )
{
    PhysicsParam params[]=
    {
        { 0.5f, 100.0f, 0.36f, 0.2f },
        { 0.5f, 100.0f, 0.36f, 0.2f },
        { 0.5f, 100.0f, 0.55f, 0.2f },
        { 0.5f, 100.0f, 0.5f, 0.2f },
        { 0.5f, 100.0f, 0.5f, 0.2f },
        { 0.5f, 100.0f, 0.5f, 0.2f },
    };                     

    MyAssert((int)type >= 0 && (int)type < (int)ARRAYSIZE(params), "���݂��Ȃ��^�C�v�̃L�����N�^�^�C�v��UsualBall::GetBallPhysics�ɓn����܂����@type= %d ", (int)type);

    return params[(int)type];
}

bool UsualBall::Update()
{
    return (this->*m_pStateFunc)();       
}


bool UsualBall::Msg(MsgType mt)
{

	return false;
}

void UsualBall::SetRotateSpeed(CrVector3 AngleSpeed)
{
    m_RotateSpeed = AngleSpeed;
}


bool UsualBall::isOutofField()const
{
	return Vector3Length(m_Params.pos) > 100;
}


void UsualBall::UpdateDamageClass()
{
	m_Damage.vec = m_Params.move;

    m_Damage.m_Param.pos2 = m_Damage.m_Param.pos1;

    {
        m_Damage.m_Param.pos1 = m_Params.pos;

        if (Vector3Distance(m_Damage.m_Param.pos1, m_Damage.m_Param.pos2) < m_Damage.m_Param.width)
        {
            Vector3 v = m_Damage.m_Param.pos2 - m_Damage.m_Param.pos1;

            v.Normalize();

            m_Damage.m_Param.pos2 = m_Damage.m_Param.pos1 + v*m_Damage.m_Param.width;
        }
    }
	
}


void UsualBall::UpdateColor()
{
    const COLORf Color = CharacterBase::GetPlayerColorF(m_Params.pParent->m_PlayerInfo.number);

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
        COLORf Color = CharacterBase::GetPlayerColorF(m_Params.pParent->m_PlayerInfo.number);

        m_pMeshRenderer->m_HDR = Vector3(1,1,1) * 0.1f;

    }
}


bool UsualBall::UpdateWallCheck(Vector3& outNewMove)
{
    Vector3 Out, Pos(m_Params.pos), Vec(m_Params.move);
    float Dist = m_Params.move.Length()*2.0f;
    int Material;

    Vec.Normalize();

    if (DefCollisionMgr.RayPick
		(
        &Out,
        &Pos,
        &Vec,
        &Dist,
        &Material,
        CollisionManager::RayType::_Usual
		)
		)
    {
        outNewMove = Vector3Refrect(m_Params.move, Vec);
        outNewMove *= 0.75f;
        return true;
    }

    return false;
}

void UsualBall::AddLocusPoint()
{
    Vector3 v;
    Vector3Cross(v, m_Params.move, DefCamera.GetForward());
    v.Normalize();

    m_Locus.AddPoint(m_Params.pos, v);
}

void UsualBall::Counter(CharacterBase* pCounterCharacter)
{
    m_Damage.pParent = m_Params.pParent = pCounterCharacter;

    UpdateColor();

    m_Damage.type = DamageBase::Type::_VanishDamage;
    m_Damage.Value += 1.0f; //�_���[�W�𑝂₷

    //�G�t�F�N�g�J�E���g�ݒ�
    m_EffectFrameCount = 45;


    //EffectFactory::CircleAnimationBillbord(
    //    m_Params.pos,
    //    Vector3Zero,
    //    Vector3Zero,
    //    Vector2(8, 10), 
    //    0xFFFFFFFF,
    //    RS_ADD
    //    );

}

void UsualBall::ToNoWork()
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
    m_pStateFunc = &UsualBall::StatePhysicMove;

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
        m_Params.move * 45.0f
        );
}

RATIO UsualBall::GetMovePower()const
{
    const float m = m_Params.move.Length();
    return min(m / 1.5f, 0.8f);
}

bool UsualBall::StateFlyMove()
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

    //�_���[�W�֘A�̍X�V
    {
        //�����_���[�W�J�E���g���O��ƈ����(�U�������������Ȃ�q�b�g�X�g�b�v�t���[����ݒ�)
        if (m_Damage.HitCount != m_HitCountSave)
        {
            m_HitCountSave = m_Damage.HitCount;
            m_HitStopFrame = 5; //�K���ł�
        }

        //�q�b�g�ő�l�Ȃ�_���[�W����̂Ȃ���Ԃֈڍs����
        if (m_Damage.HitCount >= (int)m_HitNum)
        {
            m_Params.move *= -0.25f;
            m_Params.move.y += 0.2f;


            //�U������̂Ȃ���Ԃɂ���
            ToNoWork();

        }

        //�_���[�W����̈ʒu�����݂̈ʒu�ɍX�V
        UpdateDamageClass();
}

    //�X�e�[�W�Ƃ̂����蔻��
    {
        //�����ǂɓ������Ă�����_���[�W����̂Ȃ���Ԃֈڍs����
        Vector3 NewMoveVec(0, 0, 0);

        if (UpdateWallCheck(NewMoveVec))
        {
            //�V�����ړ��l���Z�b�g
            m_Params.move = NewMoveVec;

            //�U������̂Ȃ���Ԃɂ���
            ToNoWork();
        }
    }

    //���b�V���X�V
    {
        Matrix m = m_pMeshRenderer->GetMatrix();
        
        m._41 = m._42 = m._43 = 0.0f;
        
        {
            Matrix R;
            //��]
            D3DXMatrixRotationYawPitchRoll(&R, m_RotateSpeed.x, m_RotateSpeed.y, m_RotateSpeed.z);
            m *= R;
        }
        
        m._41 = m_Params.pos.x;
        m._42 = m_Params.pos.y;
        m._43 = m_Params.pos.z;

        m_pMeshRenderer->SetMatrix(m);

        //�O�Ղ̓_��ǉ�
        AddLocusPoint();
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


    //�t�B�[���h�O�Ȃ�X�V���s
    return !isOutofField();
}

bool UsualBall::StatePhysicMove()
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

    //�O�Ղ̕s�����x�����X�Ɍ��炵�Ă���
    {
        //�O��
        m_Locus.m_StartParam.Color.w *= 0.95f;
        m_Locus.m_StartParam.HDRColor.w *= 0.95f;

        //���������ȉ��Ȃ�`�悵�Ȃ�
        if (m_Locus.m_StartParam.Color.w < 0.01f)
        {
            m_Locus.m_Visible = false;
        }

        if (m_Locus.m_Visible)
        {
            //�O�Ղ̓_��ǉ�
            AddLocusPoint();
        }
    }

    //���l������
    {
        m_pMeshRenderer->m_HDR *= 0.92f;
    }

    //�X�P�[����0.75�{�Ɂc�H
    {
        
    }

    //�t�B�[���h�O,�������͏��Ń^�C�}�[���O�@�Ȃ�X�V���s
    return !isOutofField() && m_DeleteFrame > 0;
}
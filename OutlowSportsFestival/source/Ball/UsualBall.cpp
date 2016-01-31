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
#include "../character/CharacterFunction.h"


const float UsualBall::AddSkillValueRatio = 0.01f;

UsualBall::UsualBall(
    BallBase::Params	params,			//�{�[���p�����[�^
    DamageBase::Type	damage_type,	//�_���[�W����̃^�C�v
    float				damage_val,		//�_���[�W��
    MoveControll*       pMoveControll,   //�ړ��N���X
    int                 hit_num,        //�q�b�g��
    int                 live_frame   
    ) :
    m_DeleteFrame(120),
    m_Locus(15),
    m_pRigitBody(nullptr),
    m_HitNum(hit_num),
    m_HitCountSave(0),
    m_HitStopFrame(0),
    m_pStateFunc(&UsualBall::StateFlyMove),
    m_RotateSpeed(0.15f, 0.05f, 0.05f),
    m_FirstParentType(params.pParent->m_PlayerInfo.chr_type),
    m_FlyLiveFrame((int)live_frame),
    m_pMoveControll(pMoveControll)
{
    
	LPIEXMESH		pBallMesh;

	//�p�����[�^���
    m_Params = params;


    D3DXMatrixIdentity(&m_BaseMatrix);

    {
        //�_���[�W����̃p�����[�^����
        m_Damage.pBall = this;
        m_Damage.pParent = params.pParent;
        m_Damage.m_Param.width = 0.75f;	//�傫���̓{�[���ɂ���ĈقȂ�?
        m_Damage.type = damage_type;
        m_Damage.Value = damage_val;
        m_Damage.m_Enable = true;
        m_Damage.m_Param.pos1 = m_Params.pos;
        m_Damage.m_Param.pos2 = m_Params.pos;
        m_Damage.m_VecPower.x = 0.4f;
        m_Damage.m_VecPower.y = 0.5f;
        m_Damage.m_Vec.y = 0.0f;

        UpdateDamageClass();
    }

    {
        m_MeshScale = GetBallScale(m_FirstParentType);
        Matrix m;

        //�{�[���̃��b�V�����쐬
        GetBallMesh(params.pParent->m_PlayerInfo.chr_type, &pBallMesh);

        //���b�V���̃����_���[�쐬
        m_pMeshRenderer = new MeshRenderer(
            pBallMesh,
            false,
            MeshRenderer::RenderType::UseColorSpecular
            );

        D3DXMatrixScaling(&m, m_MeshScale, m_MeshScale, m_MeshScale);

        m._41 = m_Params.pos.x;
        m._42 = m_Params.pos.y;
        m._43 = m_Params.pos.z;

        m_pMeshRenderer->SetMatrix(m);
    }


    {
        //�O�Ղ̐ݒ�
        m_Locus.m_Division = 0;
        m_Locus.m_StartParam.Width = 3.6f * GetBallScale(m_FirstParentType);
        m_Locus.m_EndParam.Width = 0.1f;

        UpdateColor();
    }

    //�G�t�F�N�g�̐e�Z�b�g
    m_BallEffect.SetParent(this);

}

UsualBall::~UsualBall()
{
    delete m_pMoveControll;
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
        return 0.12f;

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
        { 0.5f, 600.0f, 0.30f, 0.85f },
        { 0.5f, 600.0f, 0.28f, 0.85f },
        { 1.0f, 600.0f, 0.58f, 0.85f },
        { 0.5f, 600.0f, 0.55f, 0.85f },
        { 0.5f, 600.0f, 0.55f, 0.85f },
        { 0.5f, 600.0f, 0.55f, 0.85f },
    };                     

    MyAssert((int)type >= 0 && (int)type < (int)ARRAYSIZE(params), "���݂��Ȃ��^�C�v�̃L�����N�^�^�C�v��UsualBall::GetBallPhysics�ɓn����܂����@type= %d ", (int)type);

    return params[(int)type];
}

// RigidBody�𐶐�����
RigidBody* UsualBall::CreateRigidBody(
    CharacterType::Value type, //�{�[���̃L�����N�^�^�C�v
    Params params // �{�[���̃p�����[�^
    )
{
    PhysicsParam physics = GetBallPhysics(type);

    switch ( type )
    {
    case CharacterType::_Baseball:
    case CharacterType::_Soccer:
    case CharacterType::_Tennis:
    case CharacterType::_Lacrosse:
    case CharacterType::_Volleyball:
        return DefBulletSystem.AddRigidSphere(
            physics.Mass,
            RigidBody::ct_dynamic,
            params.pos,
            Vector3Zero,
            physics.Radius,
            physics.Friction,
            physics.Restitution,
            params.move * 45.0f
            );

    case CharacterType::_Americanfootball:
        return DefBulletSystem.AddRigidCapsure(
            physics.Mass,
            RigidBody::ct_dynamic,
            params.pos,
            Vector3Zero, // �Ƃ肠�����[��
            physics.Radius,
            physics.Radius*1.5f, 
            physics.Friction,
            physics.Restitution,
            params.move * 45.0f
            );

    default:
        break;
    }

    // �G���[
    MyAssert(false, "���݂��Ȃ��^�C�v�̃L�����N�^�^�C�v��UsualBall::GetBallPhysics�ɓn����܂����@type= %d ", (int)type);

    return nullptr;
}

UsualBall::MoveControll* UsualBall::GetUsualMoveControll()
{
    class UsualMoveControllClass :public UsualBall::MoveControll
    {
    public:
        void Move(UsualBall* pBall)
        {
            pBall->m_Params.pos += pBall->m_Params.move;
        }
    };

    return new UsualMoveControllClass();
}

//������ɌĂԂ����̃N���X��Ԃ�(�����������I�ɃL�����N�^�̈ʒu�ɂ��킹��)
UsualBall::MoveControll* UsualBall::GetUsual_ControllHeightMoveControll()
{
    class GetUsualControllHeightMoveControllclass :public UsualBall::MoveControll
    {
    public:
        void Move(UsualBall* pBall)
        {
            pBall->m_Params.pos += pBall->m_Params.move;
            pBall->m_Params.pos.y += (UsualBall::UsualBallShotY - pBall->m_Params.pos.y)*0.2f;
        }
    };

    return new GetUsualControllHeightMoveControllclass();
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
    
    {
        Vector3 DamageVec = m_Params.move;
        DamageVec.Normalize();

        m_Damage.m_Vec.x = DamageVec.x;
        m_Damage.m_Vec.z = DamageVec.z;
    }
	

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

        m_pMeshRenderer->m_HDR = Vector3(1,1,1) * 0.0f;
        m_pMeshRenderer->m_Lighting = Vector3(1, 1, 1) * 0.2f;
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
        outNewMove *= GetBallPhysics(m_FirstParentType).Restitution;
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

void UsualBall::ScaleUpdate()
{
    const float s = m_MeshScale;
    Vector3 v;

    {
        v.x = m_BaseMatrix._11;
        v.y = m_BaseMatrix._12;
        v.z = m_BaseMatrix._13;

        v.Normalize();
        v *= s;

        m_BaseMatrix._11 = v.x;
        m_BaseMatrix._12 = v.y;
        m_BaseMatrix._13 = v.z;
    }

    {
        v.x = m_BaseMatrix._21;
        v.y = m_BaseMatrix._22;
        v.z = m_BaseMatrix._23;

        v.Normalize();
        v *= s;

        m_BaseMatrix._21 = v.x;
        m_BaseMatrix._22 = v.y;
        m_BaseMatrix._23 = v.z;
    }

    {
        v.x = m_BaseMatrix._31;
        v.y = m_BaseMatrix._32;
        v.z = m_BaseMatrix._33;

        v.Normalize();
        v *= s;

        m_BaseMatrix._31 = v.x;
        m_BaseMatrix._32 = v.y;
        m_BaseMatrix._33 = v.z;
    }
}

void UsualBall::Counter(CharacterBase* pCounterCharacter)
{
    //�ړ��N���X�ɒʒm
    m_pMoveControll->Counter(pCounterCharacter, this);

    m_Damage.pParent = m_Params.pParent = pCounterCharacter;

    UpdateColor();
    m_Locus.m_StartParam.HDRColor.w = 0.65f;

    m_FlyLiveFrame = 180;

    m_Damage.m_VecPower.x = Vector3XZLength(m_Params.move)*1.2f;
    m_Damage.type = DamageBase::Type::_VanishDamage;
    m_Damage.Value += 1.0f; //�_���[�W�𑝂₷
    m_Damage.m_Vec.y = 0.4f;

    //�G�t�F�N�g�ݒ�
    m_BallEffect.Counter();

}

void UsualBall::ToNoWork()
{
    //���łɍU������̂Ȃ���ԂɂȂ��Ă�����return
    if (m_pRigitBody != nullptr)
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

    m_pRigitBody = CreateRigidBody(
        m_FirstParentType,
        m_Params
        );
}

void UsualBall::CatchStart()
{
    if (m_FlyLiveFrame < 60)
    {
        m_FlyLiveFrame = 60;
    }
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
            m_pMoveControll->Move(this);
           // m_Params.pos += m_Params.move;
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

    //��s���ԃJ�E���g
    {
        //��s���Ԃ��I���Ȃ�
        if (--m_FlyLiveFrame <= 0)
        {
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

    //�G�t�F�N�g�X�V
    m_BallEffect.Update();

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

            GetBallMesh(m_FirstParentType, &pMesh);

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
        m_pMeshRenderer->m_Lighting += (Vector3(-0.1f, -0.1f, -0.1f) - m_pMeshRenderer->m_Lighting)*0.2f;
    }


    //�X�P�[������
    {
        m_MeshScale += (GetBallScale(m_FirstParentType)*0.85f - m_MeshScale)*0.1f;
        ScaleUpdate();
    }


    //���Ń^�C�}�[���O�@�Ȃ�X�V���s
    return  m_DeleteFrame > 0;
}
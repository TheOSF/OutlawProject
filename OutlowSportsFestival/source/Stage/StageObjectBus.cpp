#include "StageObjectBus.h"
#include "../GameSystem/ResourceManager.h"
#include "../debug/DebugDraw.h"

StageObjectBus::StageObjectBus(
    CrVector3 pos,   //�o���ʒu
    CrVector3 vec,   //�o�������Ƃ��̌���
    CrVector3 goal_pos,   //���B�ʒu
    CrVector3 goal_vec,   //���B�����Ƃ��̌���
    float     Speed, //���x
    bool      light  //�w�b�h���C�g(on off)
    ) :
    m_TargetSpeed(Speed),
    m_MoveSpeed(Speed),
    m_LiveFlg(true),
    m_Angle(0)
{
    const Vector3 forward = Vector3Normalize(vec);
    Matrix InitMatrix;


    //���b�V���ǂݍ���
    m_pMesh = new MeshRenderer(
        DefResource.Get(Resource::MeshType::Bus),
        false,
        MeshRenderer::RenderType::UseColor
        );

    {
        //�����̃��[���h�ϊ��s����Z�o

        const float CarScale = 0.2f;  //�Ƃ肠����

        Vector3 r, u(Vector3AxisY), f;


        f = forward;
        Vector3Cross(r, u, f);

        r.Normalize();
        u.Normalize();
        f.Normalize();

        D3DXMatrixIdentity(&InitMatrix);

        InitMatrix._11 = r.x;
        InitMatrix._12 = r.y;
        InitMatrix._13 = r.z;

        InitMatrix._21 = u.x;
        InitMatrix._22 = u.y;
        InitMatrix._23 = u.z;

        InitMatrix._31 = f.x;
        InitMatrix._32 = f.y;
        InitMatrix._33 = f.z;

        for (int i = 0; i < 12; ++i)
        {
            InitMatrix[i] *= CarScale;
        }

        InitMatrix._41 = pos.x;
        InitMatrix._42 = pos.y;
        InitMatrix._43 = pos.z;

        m_pMesh->SetMatrix(InitMatrix);
    }

    {
        //RigidBody�N���X�̍쐬
        m_Angle = acosf(Vector3Normalize(vec).z);
        m_Angle = (forward.x < 0.0f) ? (-m_Angle) : (m_Angle);

        m_pRigidBody = DefBulletSystem.AddRigidBox(
            500,
            RigidBody::CollisionTypes::ct_kinematic,
            pos,
            Vector3(0, m_Angle, 0),
            Vector3(2, 2, 4),
            1.0f,
            0.2f,
            Vector3Zero
            );
    }

    {
        //�w�b�h���C�g�̍쐬

        m_Light.Visible = light;

        if (light)
        {
            m_Light.param.color = Vector3(1, 1, 0);
            m_Light.param.Shadow.visible = false;
            m_Light.param.Shadow.pDepthRenderer = DefRendererMgr.GetDepthRenderer();
            m_Light.param.size = 7.5f;

            LightPosUpdate();
        }

    }

    {
        //�_���[�W����쐬
        m_Damage.HitCount = 0;
        m_Damage.m_Enable = true;
        m_Damage.m_Param.width = 4.0f;
        m_Damage.pBall = nullptr;
        m_Damage.pParent = nullptr;
        m_Damage.type = DamageBase::Type::_VanishDamage;
        m_Damage.Value = 10.0f;

        m_Damage.m_Vec = Vector3Normalize(forward - Vector3AxisZ);
        m_Damage.m_Vec.y = 0.2f;

        UpdateDamage();
    }

}


StageObjectBus::~StageObjectBus()
{
    delete m_pMesh;
    DefBulletSystem.RemoveRigidBody(m_pRigidBody);
}


bool StageObjectBus::Update()
{
    //�������Ă����ꍇ�̏���
    if (m_Damage.HitCount > 0)
    {
        AttackSucces();
    }

    //�ړ�
    Move();

    //��������ʒu�̍X�V
    UpdateRigidBody();

    //�����蔻��ʒu�̍X�V
    UpdateDamage();


    //���C�g�ʒu�̍X�V
    if (m_Light.Visible)
    {
        LightPosUpdate();
    }

    //�S�[�����Ă������������
    if (m_LiveFlg)
    {
        m_LiveFlg = !isGoal();
    }


    return m_LiveFlg;
}

bool StageObjectBus::Msg(MsgType mt)
{

    //���E���h���Z�b�g���ɏ���
    if (mt == MsgType::_RoundReset)
    {
        m_LiveFlg = false;
    }



    return false;
}

void  StageObjectBus::AttackSucces()
{
    m_Damage.HitCount = 0;
    m_MoveSpeed *= 0.35f;
}

void  StageObjectBus::Move()
{
    Matrix t = m_pMesh->GetMatrix();

    Vector3 v = m_GoalPos - Vector3(t._41, t._42, t._43);

    m_MoveSpeed += (m_TargetSpeed - m_MoveSpeed)*0.03f;

    if (v.Length() > m_MoveSpeed)
    {
        v.Normalize();
        v *= m_MoveSpeed;
    }

    t._41 += v.x;
    t._42 += v.y;
    t._43 += v.z;

    m_pMesh->SetMatrix(t);
}

void  StageObjectBus::UpdateRigidBody()
{
    Matrix t = m_pMesh->GetMatrix();

    m_pRigidBody->TransformKinematicObject(
        Vector3(t._41, t._42, t._43),
        Vector3(0, m_Angle, 0)
        );
}

void  StageObjectBus::UpdateDamage()
{
    const Matrix& t = m_pMesh->GetMatrix();

    Vector3 p(t._41, t._42, t._43);
    Vector3 v(t._31, t._32, t._33);

    v.Normalize();
    v *= 4.5f;

    m_Damage.m_Param.pos1 = p + v;
    m_Damage.m_Param.pos2 = p - v;
}

bool StageObjectBus::isGoal()
{
    const Matrix& t = m_pMesh->GetMatrix();

    return Vector3Distance(Vector3(t._41, t._42, t._43), m_GoalPos) < 1.0f;
}


void  StageObjectBus::LightPosUpdate()
{
    const Matrix& t = m_pMesh->GetMatrix();

    Vector3 pos(0, 1, 0.0f), target(0, -5, 100.0f);

    pos = Vector3MulMatrix(pos, t);
    target = Vector3MulMatrix(target, t);



    //new DebugDrawPole(
    //    pos, 
    //    target,
    //    m_Light.param.size
    //    );

    m_Light.param.origin = pos;
    m_Light.param.target = target;
}
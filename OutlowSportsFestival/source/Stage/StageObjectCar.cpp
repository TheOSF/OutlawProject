#include "StageObjectCar.h"
#include "../GameSystem/ResourceManager.h"
#include "../debug/DebugDraw.h"

StageObjectCar::StageObjectCar(
    CrVector3 pos,   //�o���ʒu
    CrVector3 goal,  //�ړ��^�[�Q�b�g
    float     Speed, //���x
    CarType   type,  //�ԃ^�C�v
    bool      light  //�w�b�h���C�g(on off)
    ):
    m_Speed(Speed),
    m_GoalPos(goal),
    m_LiveFlg(true),
    m_Angle(0)
{
    const Vector3 forward = Vector3Normalize(goal - pos);
    Matrix InitMatrix;

    {
        //���b�V���ǂݍ���

        iexMesh* pCarMesh;

        switch (type)
        {
        case CarType::Car1:
            pCarMesh = DefResource.Get(Resource::MeshType::Car1);
            break;

        default:
            MyAssert(false, "���݂��Ȃ�CarType���w�肳��܂���");
            break;
        }

        m_pCarMesh = new MeshRenderer(
            pCarMesh,
            false,
            MeshRenderer::RenderType::UseColor
            );
    }


    {
        //�����̃��[���h�ϊ��s����Z�o

        const float CarScale = GetCarScale(type);

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

        m_pCarMesh->SetMatrix(InitMatrix);
    }

    {
        //RigidBody�N���X�̍쐬


        m_Angle = acosf(Vector3Normalize(goal - pos).z);
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
        m_Damage.m_Param.width = 4.5f;
        m_Damage.pBall = nullptr;
        m_Damage.pParent = nullptr; 
        m_Damage.type = DamageBase::Type::_VanishDamage;
        m_Damage.Value = 10.0f;

        m_Damage.vec = Vector3Normalize(forward - Vector3AxisZ);
        m_Damage.vec.y = 0.2f;

        UpdateDamage();
    }

}


StageObjectCar::~StageObjectCar()
{
    delete m_pCarMesh;
    DefBulletSystem.RemoveRigidBody(m_pRigidBody);
}


bool StageObjectCar::Update()
{
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

bool StageObjectCar::Msg(MsgType mt)
{

    //���E���h���Z�b�g���ɏ���
    if (mt == MsgType::_RoundReset)
    {
        m_LiveFlg = false;
    }



    return false;
}

void  StageObjectCar::Move()
{
    Matrix t = m_pCarMesh->GetMatrix();

    Vector3 v = m_GoalPos - Vector3(t._41, t._42, t._43);

    if (v.Length() > m_Speed)
    {
        v.Normalize();
        v *= m_Speed;
    }

    t._41 += v.x;
    t._42 += v.y;
    t._43 += v.z;

    m_pCarMesh->SetMatrix(t);
}

void  StageObjectCar::UpdateRigidBody()
{
    Matrix t = m_pCarMesh->GetMatrix();

    m_pRigidBody->TransformKinematicObject(
        Vector3(t._41, t._42, t._43),
        Vector3(0, m_Angle, 0)
        );
}

void  StageObjectCar::UpdateDamage()
{
    const Matrix& t = m_pCarMesh->GetMatrix();

    Vector3 p(t._41, t._42, t._43);
    Vector3 v(t._31, t._32, t._33);

    v.Normalize();
    v *= 6.2f;

    m_Damage.m_Param.pos1 = p + v;
    m_Damage.m_Param.pos2 = p - v;
}

bool StageObjectCar::isGoal()
{
    const Matrix& t = m_pCarMesh->GetMatrix();

    return Vector3Distance(Vector3(t._41, t._42, t._43), m_GoalPos) < 1.0f;
}

float StageObjectCar::GetCarScale(CarType type)
{
    return 0.2f;  //�Ƃ肠����
}

void  StageObjectCar::LightPosUpdate()
{
    const Matrix& t = m_pCarMesh->GetMatrix();

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
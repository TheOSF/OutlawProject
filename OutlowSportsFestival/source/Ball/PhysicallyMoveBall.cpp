#include "PhysicallyMoveBall.h"
#include "UsualBall.h"
#include "../character/CharacterBase.h"
#include "../Render/MeshRenderer.h"
#include "../Camera/Camera.h"
#include "../GameSystem/ResourceManager.h"
#include "../Stage/PhysicsMeshObject.h"
#include "../Collision/Collision.h"
#include "BallFadeOutRenderer.h"


PhysicallyMoveBall::PhysicallyMoveBall(
    BallBase::Params	params,			//ボールパラメータ
    DamageBase::Type	damage_type,	//ダメージ判定のタイプ
    float				damage_val,		//ダメージ量
    float               glavity         //重力
    ) :
    m_DeleteFrame(180),
    m_Locus(20),
    m_pRigitBody(nullptr),
    m_Glavity(glavity),
    m_DeleteFlag(false)
{
    LPIEXMESH		pBallMesh;

    //パラメータ代入
    m_Params = params;

    //ダメージ判定のパラメータを代入
    m_Damage.pBall = this;
    m_Damage.pParent = params.pParent;
    m_Damage.m_Param.size = 1;	//大きさはボールによって異なる?
    m_Damage.type = damage_type;
    m_Damage.Value = damage_val;
    m_Damage.m_Enable = true;
    UpdateDamageClass();

    //ボールのメッシュを作成
    UsualBall::GetBallMesh(params.pParent->m_PlayerInfo.chr_type, &pBallMesh);

    //メッシュのレンダラー作成
    m_pMeshRenderer = new MeshRenderer(
        pBallMesh,
        false,
        MeshRenderer::RenderType::UseColor
        );

    D3DXQuaternionIdentity(&m_Ballrot);

    UpdateMesh();

    //軌跡の設定
    m_Locus.m_Division = 0;
    m_Locus.m_StartParam.Width = 0.4f;
    m_Locus.m_EndParam.Width = 0.1f;

    UpdateLocusColor();

    //物理パラメータ初期化
    PhysicsParam.Friction = 0.8f;
    PhysicsParam.Restitution = 0.25f;
    PhysicsParam.Mass = 1.5f;
}

PhysicallyMoveBall::~PhysicallyMoveBall()
{
    delete m_pMeshRenderer;
    if (m_pRigitBody != nullptr)
    {
        DefBulletSystem.RemoveRigidBody(m_pRigitBody);
    }
}


bool PhysicallyMoveBall::Update()
{
    UpdateMove();
    UpdateMesh();
    UpdateDamageClass();

    if (m_Params.type != BallBase::Type::_DontWork)
    {
        UpdateWallCheck();
    }

    Vector3 v;
    Vector3Cross(v, m_Params.move, DefCamera.GetForward());
    v.Normalize();

    m_Locus.AddPoint(m_Params.pos, v);

    return (!m_DeleteFlag) && (!isOutofField()) && (m_DeleteFrame > 0);
}


bool PhysicallyMoveBall::Msg(MsgType mt)
{

    return false;
}


bool PhysicallyMoveBall::isOutofField()const
{
    return Vector3Length(m_Params.pos) > 100;
}


void PhysicallyMoveBall::UpdateDamageClass()
{
    m_Damage.vec = m_Params.move;
    m_Damage.m_Param.pos = m_Params.pos;
}

void PhysicallyMoveBall::UpdateMesh()
{
    //メッシュのワールド変換行列を更新する

    Matrix m;
    const float s = UsualBall::GetBallScale(m_Params.pParent->m_PlayerInfo.chr_type);

    D3DXMatrixScaling(&m, s, s, s);	//大きさはボールによって変える必要がある

    m._41 = m_Params.pos.x;
    m._42 = m_Params.pos.y;
    m._43 = m_Params.pos.z;

    m_pMeshRenderer->SetMatrix(m);
}

void PhysicallyMoveBall::UpdateLocusColor()
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

void PhysicallyMoveBall::UpdateMove()
{
    if (m_Params.type == BallBase::Type::_DontWork)
    {
        Matrix M;
        Vector3 PrePos = m_Params.pos;

        m_pRigitBody->Get_TransMatrix(M);

        M = m_BaseMatrix * M;

        m_pMeshRenderer->SetMatrix(M);

        m_Params.pos = Vector3(M._41, M._42, M._43);
        m_Params.move = m_Params.pos - PrePos;

        //軌跡
        m_Locus.m_StartParam.Color.w *= 0.95f;

        if (m_Locus.m_StartParam.Color.w < 0.1f)
        {
            m_Locus.m_Visible = false;
        }

        //消滅タイマー
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
        }
    }
    else
    {
        m_Params.move.y += m_Glavity;
        m_Params.pos += m_Params.move;

        //敵に当たっていたら攻撃判定をなくす
        if (m_Damage.HitCount > 0)
        {
            //攻撃判定のない状態にする
            ToNoWork();
        }
    }
}

void PhysicallyMoveBall::SetHDR()
{
    const DWORD Color = CharacterBase::GetPlayerColor(m_Params.pParent->m_PlayerInfo.number);

    m_pMeshRenderer->m_HDR = Vector3(
        float((Color >> 16) & 0xFF) / 255.f,
        float((Color >> 8) & 0xFF) / 255.f,
        float(Color & 0xFF) / 255.f
        );
}

void PhysicallyMoveBall::UpdateWallCheck()
{
    Vector3 Out, Pos(m_Params.pos), Vec(m_Params.move);
    float Dist = m_Params.move.Length()*2.0f + 1.0f;
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
        //攻撃判定のない状態にする
        ToNoWork();
    }

}

void PhysicallyMoveBall::Counter(CharacterBase* pCounterChr)
{
    m_Params.pParent = m_Damage.pParent = pCounterChr;
    UpdateLocusColor();
}

void PhysicallyMoveBall::ToNoWork()
{
    //攻撃判定のない状態にする

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
        m_Params.move * 60.0f
        );
}
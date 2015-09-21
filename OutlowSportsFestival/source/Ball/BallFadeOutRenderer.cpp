#include "BallFadeOutRenderer.h"

#include "../Library/Bullet/BulletSystem.h"
#include "../Camera/Camera.h"



BallFadeOutRenderer::BallFadeOutRenderer(
    LPIEXMESH     pBallMesh,       //ボールのメッシュへのポインタ(deleteしない)
    const Matrix& BaseTransMatrix, //スケール、回転行列
    RigidBody*    pRigidBody,      //剛体クラスへのポインタ
    UINT          delete_frame     //消えるまでのフレーム
    ) :
    m_pBallMesh(pBallMesh),
    m_BaseTransMatrix(BaseTransMatrix),
    m_pRigidBody(pRigidBody),
    m_Delete_frame(delete_frame),
    m_Counter(delete_frame),
    m_Pos(0, 0, 0)
{
    assert(m_Delete_frame > 0 && "m_Delete_frameの値が０になっています");
}


BallFadeOutRenderer::~BallFadeOutRenderer()
{
    DefBulletSystem.RemoveRigidBody(m_pRigidBody);
}

bool BallFadeOutRenderer::Update()
{
    return --m_Counter > 0;
}


bool BallFadeOutRenderer::Msg(MsgType mt)
{
    return false;
}

void BallFadeOutRenderer::Render()
{
    Matrix m;

    m_pRigidBody->Get_TransMatrix(m);
    m = m_BaseTransMatrix*m;

    m_Pos.x = m._41;
    m_Pos.y = m._42;
    m_Pos.z = m._43;

    m_pBallMesh->TransMatrix = m;

    shader->SetValue("g_Color", Vector4(1, 1, 1, (float)m_Counter / (float)m_Delete_frame));
    m_pBallMesh->Render(shader, "forward");
}

void BallFadeOutRenderer::CalcZ()
{
    m_SortZ = DefCamera.GetCameraZ(m_Pos);
}
#include "StagePhysicMoveObject.h"
#include "../Ball/BallFadeOutRenderer.h"

StagePhysicMoveObject::StagePhysicMoveObject(
    MeshRenderer* pRenderMesh,       //描画するメッシュ
    RigidBody*    pRigidBody,        //描画
    const Matrix& InitTransMatrix,   //初期姿勢
    UINT          FadeOutStartFrame, //フェードアウトし始めるフレーム
    UINT          FadeOutEndFrame    //フェードアウトして消えるフレーム
    ) :
    m_pRenderMesh(pRenderMesh),
    m_pRigidBody(pRigidBody),
    m_InitTransMatrix(InitTransMatrix),
    m_FadeOutStartFrame(FadeOutStartFrame),
    m_FadeOutEndFrame(FadeOutEndFrame),
    m_Frame(0)
{

}

StagePhysicMoveObject::~StagePhysicMoveObject()
{
    delete m_pRenderMesh;
    DefBulletSystem.RemoveRigidBody(m_pRigidBody);
}

bool StagePhysicMoveObject::Update()
{
    Matrix M;

    m_pRigidBody->Get_TransMatrix(M);

    M = m_InitTransMatrix * M;

    m_pRenderMesh->SetMatrix(M);

    if (++m_Frame > m_FadeOutStartFrame)
    {
        new BallFadeOutRenderer(
            m_pRenderMesh->m_pMesh,
            m_InitTransMatrix,
            m_pRigidBody,
            (m_FadeOutEndFrame > m_FadeOutStartFrame) ? (m_FadeOutEndFrame - m_FadeOutStartFrame) : (0)
            );
        
        m_pRenderMesh->m_pMesh = nullptr;
        m_pRigidBody = nullptr;

        return false;
    }

    return true;
}

bool StagePhysicMoveObject::Msg(GameObjectBase::MsgType type)
{
    return false;
}
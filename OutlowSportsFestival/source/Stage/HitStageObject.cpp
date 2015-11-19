#include "HitStageObject.h"

HitStageObject::HitStageObject(
    MeshRenderer*   pMeshRenderer,  //メッシュ描画クラス
    MeshCollider*   pMeshCollider   //メッシュあたり判定クラス
    ):
    m_pMeshRenderer(pMeshRenderer),
    m_pMeshCollider(pMeshCollider)
{

}

HitStageObject::~HitStageObject()
{
    delete m_pMeshRenderer;
    delete m_pMeshCollider;
}


void HitStageObject::SetMatrix(const Matrix& mat)
{
    if (m_pMeshRenderer != nullptr)
    {
        m_pMeshRenderer->SetMatrix(mat);
    }

    if (m_pMeshRenderer != nullptr)
    {
        m_pMeshCollider->SetWorldMatrix(mat);
    }
}

bool HitStageObject::Update()
{
    return true;
}

bool HitStageObject::Msg(MsgType mt)
{
    return false;
}
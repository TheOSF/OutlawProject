#include "ForwardHDR_MeshRenderer.h"
#include "../Camera/Camera.h"

//**************************************************************************
//		メッシュレンダラークラス(メッシュをマイフレーム自動的に描画する)
//**************************************************************************

ForwardHDR_MeshRenderer::ForwardHDR_MeshRenderer(
    LPIEXMESH	pMesh,		//描画するメッシュのポインタ
    bool		MeshDelete	//第一引数のメッシュをdeleteするかどうか
    ) :
    m_pMesh(pMesh),
    m_MeshDelete(MeshDelete),
    m_HDR_Color(1, 1, 1),
    m_Color(1,1,1,1)
{
    D3DXMatrixIdentity(&m_TransMatrix);
}

ForwardHDR_MeshRenderer::~ForwardHDR_MeshRenderer()
{
    if (m_MeshDelete)
    {
        delete m_pMesh;
    }
}

void ForwardHDR_MeshRenderer::Render()
{
    
    shader->SetValue("g_HDR_Color", m_HDR_Color);
    shader->SetValue("g_Color", Vector4(m_Color.r, m_Color.g, m_Color.b, m_Color.a));
    
    m_pMesh->TransMatrix = m_TransMatrix;
    m_pMesh->Render(shader, "HDR_Forward");
}

void ForwardHDR_MeshRenderer::SetMatrix(const Matrix& mat)
{
    m_TransMatrix = mat;
}


const Matrix& ForwardHDR_MeshRenderer::GetMatrix()const
{
    return m_TransMatrix;
}

void ForwardHDR_MeshRenderer::CalcZ()
{
    m_SortZ = 
        DefCamera.GetCameraZ(
            Vector3(
                m_pMesh->TransMatrix._41,
                m_pMesh->TransMatrix._42,
                m_pMesh->TransMatrix._43
        ));
}
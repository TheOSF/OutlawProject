#include "ForwardHDR_MeshRenderer.h"
#include "../Camera/Camera.h"

//**************************************************************************
//		���b�V�������_���[�N���X(���b�V�����}�C�t���[�������I�ɕ`�悷��)
//**************************************************************************

ForwardHDR_MeshRenderer::ForwardHDR_MeshRenderer(
    LPIEXMESH	pMesh,		//�`�悷�郁�b�V���̃|�C���^
    bool		MeshDelete	//�������̃��b�V����delete���邩�ǂ���
    ) :
    m_pMesh(pMesh),
    m_MeshDelete(MeshDelete),
    m_HDR_Color(1, 1, 1)
{

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
    m_pMesh->Render(shader, "HDR_Forward");
}

void ForwardHDR_MeshRenderer::SetMatrix(const Matrix& mat)
{
    m_pMesh->TransMatrix = mat;
}


const Matrix& ForwardHDR_MeshRenderer::GetMatrix()const
{
    return m_pMesh->TransMatrix;
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
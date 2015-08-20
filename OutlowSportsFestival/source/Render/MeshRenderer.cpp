#include "MeshRenderer.h"

//**************************************************************************
//		���b�V�������_���[�N���X(���b�V�����}�C�t���[�������I�ɕ`�悷��)
//**************************************************************************

MeshRenderer::MeshRenderer(
	LPIEXMESH	pMesh,
	bool		MeshDelete,
    RenderType  type
	) :
	m_pMesh(pMesh),
	m_MeshDelete(MeshDelete),
    m_RenderType(type)
{

}

MeshRenderer::~MeshRenderer()
{
	if (m_MeshDelete)
	{
		delete m_pMesh;
	}
}

void MeshRenderer::GbufRender(
    iexShader*        pShader,                       //�V�F�[�_�[�N���X
    DeferredGbufRenderer::TechniqueSetter*  pSetter  //�e�N�j�b�N�Ǘ��N���X
    )
{
    char str[256];
    pSetter->NoTexture(str,256);
    m_pMesh->Render(pShader, str);
}

void MeshRenderer::MasterRender()
{
    switch (m_RenderType)
    {
    case RenderType::NoTexture:
        m_pMesh->Render(shader, "DeffLightNoTex");
        break;

    case RenderType::UseColor:
        m_pMesh->Render(shader, "DeffLightNoSp");
        break;

    case RenderType::UseColorSpecular:
        m_pMesh->Render(shader, "DeffLight");
        break;
    }

}

void MeshRenderer::DepthRender(iexShader* pShader, const char* pTec)
{
    char str[256];
    strcpy_s<256>(str, pTec);
    m_pMesh->Render(pShader, str);
}


void MeshRenderer::SetMatrix(const Matrix& mat)
{
	m_pMesh->TransMatrix = mat;
}


const Matrix& MeshRenderer::GetMatrix()const
{
	return m_pMesh->TransMatrix;
}
#include "MeshRenderer.h"

//**************************************************************************
//		メッシュレンダラークラス(メッシュをマイフレーム自動的に描画する)
//**************************************************************************

MeshRenderer::MeshRenderer(
    LPIEXMESH	pMesh,
    bool		MeshDelete,
    RenderType  type,
    GbufRenderType    gbuf_Type,
    PreRenderCallBack* pCallBack
    ) :
    m_pMesh(pMesh),
    m_MeshDelete(MeshDelete),
    m_RenderType(type),
    m_Gbuf_Type(gbuf_Type),
    m_HDR(0, 0, 0),
    m_pCallBack(pCallBack),
    m_Lighting(0, 0, 0),
    m_Visible(true)
{
    D3DXMatrixIdentity(&m_TransMatrix);

    if (m_pCallBack == nullptr)
    {
        m_pCallBack = new PreRenderCallBack();
    }
}

MeshRenderer::~MeshRenderer()
{
	if (m_MeshDelete)
	{
		delete m_pMesh;
	}

    delete m_pCallBack;
}

void MeshRenderer::GbufRender(
    iexShader*        pShader,                       //シェーダークラス
    DeferredGbufRenderer::TechniqueSetter*  pSetter  //テクニック管理クラス
    )
{
    if (m_Visible == false)
    {
        return;
    }

    char str[256];

    switch (m_Gbuf_Type)
    {
    case GbufRenderType::NoTexture:
        pSetter->NoTexture(str, 256);

        break;

    case GbufRenderType::UseNormal:
        pSetter->UseNormal(str, 256);

        break;

    case GbufRenderType::UseNormalHeight:
        pSetter->UseNormalHeight(str, 256);

        break;

    default:
        MyAssert(false,"対応していないGbufRenderTypeが渡されました");
        break;
    }

    
    m_pCallBack->Execute(this, PreRenderCallBack::GbufRender);

    m_pMesh->TransMatrix = m_TransMatrix;
    m_pMesh->Render(pShader, str);
}

void MeshRenderer::MasterRender()
{
    if (m_Visible == false)
    {
        return;
    }

    shader->SetValue("g_HDR_Color", m_HDR);
    shader->SetValue("g_Lighting_Color", m_Lighting);
    

    m_pCallBack->Execute(this, PreRenderCallBack::MasterRender);

    m_pMesh->TransMatrix = m_TransMatrix;

    switch (m_RenderType)
    {

    case RenderType::NoTexture:
        m_pMesh->Render(shader, "DeffLightNoTex");
        break;

    case RenderType::UseColor:
        m_pMesh->Render(shader, "DeffLightNoSp");
        break;

    case RenderType::UseColor_DarkToAlpha:
        m_pMesh->Render(shader, "DeffLight_DarkToAlpha");
        break;

    case RenderType::UseColorSpecular:
        m_pMesh->Render(shader, "DeffLight");
        break;


    default:
        MyAssert(false, "作成されていない描画タイプ");
        break;
    }

}

void MeshRenderer::DepthRender(iexShader* pShader, const char* pTec, DepthRenderType type)
{

    if (m_Visible == false||type != DepthRenderType::DirLight)
    {
        return;
    }

    m_pCallBack->Execute(this, PreRenderCallBack::DepthRender);

    char str[256];
    strcpy_s<256>(str, pTec);
    m_pMesh->TransMatrix = m_TransMatrix;
    m_pMesh->Render(pShader, str);
}


void MeshRenderer::SetMatrix(const Matrix& mat)
{
    m_TransMatrix = mat;
}


const Matrix& MeshRenderer::GetMatrix()const
{
    return m_TransMatrix;
}
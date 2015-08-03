#include "ShaderManager.h"
#include "../IexSystem/System.h"


ShaderManager* ShaderManager::m_pInstance = nullptr;
const int ShaderManager::m_NumPointLight = 3;
const int ShaderManager::m_NumShadowCamera = 4;



ShaderManager::PointLight::PointLight(CrVector3 pos, float size, Vector3 color) :
pos(pos),
size(size),
color(color)
{
    DefShaderMgr.RegistLight(this);
}

ShaderManager::PointLight::~PointLight()
{
    DefShaderMgr.EraceLight(this);
}


ShaderManager::ShadowCamera::ShadowCamera()
{

}

ShaderManager::ShadowCamera::~ShadowCamera()
{

}

void ShaderManager::ShadowCamera::SetViewParam(
    CrVector3 pos,
    CrVector3 target
    )
{
    D3DXMatrixLookAtLH(
        &View,
        (D3DXVECTOR3*)&pos,
        (D3DXVECTOR3*)&target,
        (D3DXVECTOR3*)&Vector3AxisY
        );
}


void ShaderManager::ShadowCamera::SetProjParam(
    float Size,
    float Near,
    float Far
    )
{
    D3DXMatrixOrthoLH(&Proj,
        Size,
        Size,
        Near,
        Far
        );
}

//------------------------------------------------------

ShaderManager::ShaderManager()
{
    //トゥーンスペキュラテクスチャ読み込み
    m_pToonSpecularTex = new iex2DObj("DATA\\Shader\\SpecularToon.png");
    shader->SetValue("ToonSpMap", m_pToonSpecularTex->GetTexture());

    m_pShadowTex = new iex2DObj*[m_NumShadowCamera];

    for (int i = 0; i < m_NumShadowCamera; ++i)
    {
        m_pShadowTex[i] = new iex2DObj(512, 512, IEX2D_FLOAT2);
    }
}

ShaderManager::~ShaderManager()
{
    delete m_pToonSpecularTex;

    for (int i = 0; i < m_NumShadowCamera; ++i)
    {
        delete m_pShadowTex[i];
    }

    delete[] m_pShadowTex;
}

void ShaderManager::RegistLight(PointLight* pLight)
{
    m_PointLightList.push_front(pLight);
}

void ShaderManager::EraceLight(PointLight* pLight)
{
    PointLightList::iterator  it = m_PointLightList.begin();
    PointLightList::iterator  end = m_PointLightList.end();

    while (it != end)
    {
        if (*it == pLight)
        {
            m_PointLightList.erase(it);
            return;
        }
        ++it;
    }
}

ShaderManager& ShaderManager::GetInstance()
{
    if (m_pInstance == nullptr)
    {
        m_pInstance = new ShaderManager();
    }
    return *m_pInstance;
}

void ShaderManager::Release()
{
    delete m_pInstance;
    m_pInstance = nullptr;
}

void ShaderManager::SetDirLight(Vector3 vec, Vector3 color)
{
    vec.Normalize();
    shader->SetValue("g_DirLightVec", vec);
    shader->SetValue("g_DirLightColor", color);
}

void ShaderManager::SetCamera(Vector3 pos, Vector3 target)
{
    Vector3 vec = target - pos;
    Matrix m = matProjection;


    shader->SetValue("g_P_mat", m);

    m = matView;
    shader->SetValue("g_V_mat", m);

    m *= matProjection;
    vec.Normalize();

    shader->SetValue("g_VP_mat", m);

    shader->SetValue("g_CameraPos", pos);
    shader->SetValue("g_CameraVec", vec);
}

void ShaderManager::Update()
{
    //ポイントライトの情報を更新

    PointLightList::iterator  it = m_PointLightList.begin();
    PointLightList::iterator  end = m_PointLightList.end();

    Vector3 temp_pos  [m_NumPointLight];
    Vector3 temp_color[m_NumPointLight];
    float   temp_size [m_NumPointLight];

    for (int i = 0;
        i < m_NumPointLight &&it != end;
        ++i, ++it)
    {
        temp_pos[i] = (*it)->pos;
        temp_color[i] = (*it)->color;
        temp_size[i] = (*it)->size;
    }

    shader->SetValue(
        "g_PointLightPos", temp_pos, (UINT)m_NumPointLight
        );

    shader->SetValue(
        "g_PointLightColor", temp_color, (UINT)m_NumPointLight
        );

    shader->SetValue(
        "g_PointLightSize", temp_size, (UINT)m_NumPointLight
        );

    shader->SetValue(
        "g_PointLightUseNum", min(m_NumPointLight, (int)m_PointLightList.size())
        );
}
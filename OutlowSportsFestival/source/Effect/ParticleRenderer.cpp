#include "ParticleRenderer.h"
#include "../Camera/Camera.h"

//----------------------------------------------------------
//   通常パーティクルのレンダラークラス
//----------------------------------------------------------

ParticleRenderer::ParticleRenderer():
m_SoftEnable(false),
m_SoftDepth(0.001f)
{

}

ParticleRenderer::~ParticleRenderer()
{

}

void ParticleRenderer::CalcZ()
{
    m_SortZ = DefCamera.GetCameraZ(m_Param.pos);
}

void ParticleRenderer::Render()
{
    LVERTEX v[4];

    CalcParticleVertex(v);

    if (m_SoftEnable)
    {
        shader->SetValue("g_SoftDepth", m_SoftDepth);
        shader->SetValue("g_Color", m_Param.color.toVector4());
        iexPolygon::Render3D(v, 2, m_pTexture, shader, "SoftParticle");
    }
    else
    {
        iexPolygon::Render3D(v, 2, m_pTexture, m_Param.dw_Flag);
    }
    
}
#include "ParticleHDRRenderer.h"
#include "../Camera/Camera.h"
#include "../GameSystem/ResourceManager.h"

//----------------------------------------------------------
//   HDRパーティクルのレンダラークラス
//----------------------------------------------------------

ParticleHDRRenderer::ParticleHDRRenderer() :
m_Zenable(true),
m_HDRcolor(0)
{
    m_pTexture = DefResource.Get(Resource::TextureType::White);
}

ParticleHDRRenderer::~ParticleHDRRenderer()
{

}

void ParticleHDRRenderer::CalcZ()
{
    m_SortZ = DefCamera.GetCameraZ(m_Param.pos);
}

void ParticleHDRRenderer::Render()
{
    LVERTEX p[4];
    HdrLVERTEX v[4];

    CalcParticleVertex(p);

    for (int i = 0; i < 4; ++i)
    {
        v[i].pos = p[i].pos;
        v[i].HdrColor = m_HDRcolor;

        v[i].tu = p[i].tu;
        v[i].tv = p[i].tv;

        v[i].color = p[i].color;
    }

    if (m_Zenable == false)
    {
        iexSystem::Device->SetRenderState(D3DRS_ZENABLE, FALSE);

        iexPolygon::PolygonRender3DHDR(
            v, 2, m_pTexture, shader, "HDR_ForwardPolygon"
            );

        iexSystem::Device->SetRenderState(D3DRS_ZENABLE, TRUE);

        return;
    }

    iexPolygon::PolygonRender3DHDR(
        v, 2, m_pTexture, shader, "HDR_ForwardPolygon"
        );

}
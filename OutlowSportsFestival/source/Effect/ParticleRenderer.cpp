#include "ParticleRenderer.h"
#include "../Camera/Camera.h"

//----------------------------------------------------------
//   �ʏ�p�[�e�B�N���̃����_���[�N���X
//----------------------------------------------------------

ParticleRenderer::ParticleRenderer()
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

    iexPolygon::Render3D(v, 2, m_pTexture, m_Param.dw_Flag);
}
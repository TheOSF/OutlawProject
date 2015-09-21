#pragma once

#include "../Render/Renderer.h"
#include "ParticleBase.h"

//----------------------------------------------------------
//   HDR�p�[�e�B�N���̃����_���[�N���X
//----------------------------------------------------------

class ParticleHDRRenderer : public ForwardHDRRenderer, public ParticleBase
{
public:
    bool  m_Zenable;
    DWORD m_HDRcolor;

    ParticleHDRRenderer();
    ~ParticleHDRRenderer();

    void CalcZ();
    void Render();

};

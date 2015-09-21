#pragma once

#include "../Render/Renderer.h"
#include "ParticleBase.h"

//----------------------------------------------------------
//   HDRパーティクルのレンダラークラス
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

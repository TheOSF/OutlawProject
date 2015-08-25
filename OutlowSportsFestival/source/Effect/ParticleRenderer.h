#ifndef __FORWARD_PARTICLE_H__
#define __FORWARD_PARTICLE_H__

#include "../Render/Renderer.h"
#include "ParticleBase.h"

//----------------------------------------------------------
//   �ʏ�p�[�e�B�N���̃����_���[�N���X
//----------------------------------------------------------

class ParticleRenderer : public ForwardRenderer, public ParticleBase
{
public:
    ParticleRenderer();
    ~ParticleRenderer();

    void CalcZ();
    void Render();

};

#endif
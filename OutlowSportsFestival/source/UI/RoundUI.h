#pragma once

#include "../Render/Renderer.h"

//-------------------------------------------------------
//  ���E���h�\���N���X
//-------------------------------------------------------

class RoundUI :public ForwardRenderer
{
public:
    RoundUI(UINT Round, float Z);
    ~RoundUI();

private:
    const float  m_Z;
    const UINT   m_Round;
    iex2DObj*    m_pTexture;
    
    void CalcZ();
    //�`��(�����I�ɌĂ΂��)
    void Render();
    
    void RoundRender();
};
#pragma once

#include "../Render/Renderer.h"

//-------------------------------------------------------
//  ラウンド表示クラス
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
    //描画(自動的に呼ばれる)
    void Render();
    
    void RoundRender();
};
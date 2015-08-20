#ifndef __SMAL_BUF_BLUR_RENDERER_H__
#define __SMAL_BUF_BLUR_RENDERER_H__

#include "iextreme.h"

class SmalBufBlurRenderer
{
public:
    SmalBufBlurRenderer(UINT level,const char* ShaderFile);
    ~SmalBufBlurRenderer();

    void SetSourceTexture(iex2DObj* texture);
    void Render();

private:
public:

    struct DWORD2
    {
        DWORD x, y;
    };

    iexShader*     m_pShader;
    const UINT     m_TextureNum;
    iex2DObj**     m_pWorkTextures;
    DWORD2*        m_pWorkTextureSizes;
    Surface**      m_pDepthStencils;
    iex2DObj*      m_pSource;

    void RenderToParentTexture();
    void RenderToTexture(UINT from, UINT to);
};

#endif
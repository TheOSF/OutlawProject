#include "SmalBufBlurRenderer.h"


SmalBufBlurRenderer::SmalBufBlurRenderer(const char* ShaderFile) :
m_TextureNum(6),
m_pSource(nullptr)
{
    DWORD division;
    char path[MAX_PATH];

    sprintf_s<MAX_PATH>(path, ShaderFile);

    m_pShader = new iexShader(path);

    m_pWorkTextures = new iex2DObj*[m_TextureNum];
    m_pWorkTextureSizes = new DWORD2[m_TextureNum];

    for (UINT i = 0; i < m_TextureNum; ++i)
    {
        division = (DWORD)pow(2, (int)i + 1);

        m_pWorkTextureSizes[i].x = iexSystem::ScreenWidth / division;
        m_pWorkTextureSizes[i].y = iexSystem::ScreenHeight / division;

        m_pWorkTextures[i] = new iex2DObj(
            m_pWorkTextureSizes[i].x,
            m_pWorkTextureSizes[i].y,
            IEX2D_RENDERTARGET
            );

        sprintf(path, "%s%d", "AddTexture", (int)(i + 1));

        m_pShader->m_pShader->SetTexture(path, m_pWorkTextures[i]->GetTexture());
    }

    
    
}

SmalBufBlurRenderer::~SmalBufBlurRenderer()
{
    delete m_pShader;

    for (UINT i = 0; i < m_TextureNum; ++i)
    {
        delete m_pWorkTextures[i];
    }
    
    delete[] m_pWorkTextures;
    delete[] m_pWorkTextureSizes;
}

void SmalBufBlurRenderer::SetSourceTexture(iex2DObj* texture)
{
    m_pShader->SetValue(
        "Texture",
        texture->GetTexture()
        );

    m_pSource = texture;
}

void SmalBufBlurRenderer::Render()
{
    //現在のレンダーターゲットとビューポートとデプスステンシルバッファを保存
    D3DVIEWPORT9 SaveViewport;
    iexSystem::Device->GetViewport(&SaveViewport);

    Surface* pSaveSurface;
    iexSystem::Device->GetRenderTarget(0, &pSaveSurface);

    Surface* pSaveDepthStencil;
    iexSystem::Device->GetDepthStencilSurface(&pSaveDepthStencil);


    iexSystem::Device->SetDepthStencilSurface(NULL);



    RenderToParentTexture();

    //作業用テクスチャにブラー結果を出力
    for (UINT i = 0; i < m_TextureNum - 1; ++i)
    {
        RenderToTexture(i, i + 1);
    }

    //出力先を設定
    iexSystem::Device->SetViewport(&SaveViewport);

    iexSystem::Device->SetRenderTarget(0, pSaveSurface);
    pSaveSurface->Release();


    float alpha = 1.2f;
    float mul_alpha = 0.9f;

    m_pShader->SetValue("g_Alpha", alpha);
    m_pShader->SetValue("g_MulAlpha", mul_alpha);


    iexPolygon::RectPlus(
        0,
        0,
        iexSystem::ScreenWidth,
        iexSystem::ScreenHeight,
        m_pShader,
        "add",
        0xFFFFFFFF
        );


    iexSystem::Device->SetDepthStencilSurface(pSaveDepthStencil);
    pSaveDepthStencil->Release();

    ////ブラーテクスチャを出力
    //for (UINT i = 0; i < m_TextureNum; ++i)
    //{
    //    m_pShader->SetValue("g_Alpha", alpha);

    //    alpha *= 0.9f;

    //    m_pWorkTextures[i]->Render(
    //        0, 0, iexSystem::ScreenWidth, iexSystem::ScreenHeight,
    //        0, 0, m_pWorkTextureSizes[i].x, m_pWorkTextureSizes[i].y,
    //        m_pShader,
    //        "add"
    //        );
    //}



}


void SmalBufBlurRenderer::RenderToParentTexture()
{
    D3DVIEWPORT9 SetViewport;

    SetViewport.X = 0;
    SetViewport.Y = 0;
    SetViewport.Width = m_pWorkTextureSizes[0].x;
    SetViewport.Height = m_pWorkTextureSizes[0].y;
    SetViewport.MinZ = 0;
    SetViewport.MaxZ = 1;

    iexSystem::Device->SetViewport(&SetViewport);

    m_pWorkTextures[0]->RenderTarget(0);

    
    m_pShader->SetValue("g_offset_X", 1 / (float)iexSystem::ScreenWidth);
    m_pShader->SetValue("g_offset_Y", 1 / (float)iexSystem::ScreenHeight);
    
    
    m_pSource->Render(
        0,
        0,
        m_pWorkTextureSizes[0].x,
        m_pWorkTextureSizes[0].y,
        0,
        0,
        iexSystem::ScreenWidth,
        iexSystem::ScreenHeight,
        m_pShader,
        "gauss"
        );
}


void SmalBufBlurRenderer::RenderToTexture(UINT from, UINT to)
{
    D3DVIEWPORT9 SetViewport;

    SetViewport.X = 0;
    SetViewport.Y = 0;
    SetViewport.Width = m_pWorkTextureSizes[to].x;
    SetViewport.Height = m_pWorkTextureSizes[to].y;
    SetViewport.MinZ = 0;
    SetViewport.MaxZ = 1;

    iexSystem::Device->SetViewport(&SetViewport);

    m_pWorkTextures[to]->RenderTarget(0);

    m_pShader->SetValue("g_offset_X", 1 / (float)m_pWorkTextureSizes[from].x);
    m_pShader->SetValue("g_offset_Y", 1 / (float)m_pWorkTextureSizes[from].y);


    m_pWorkTextures[from]->Render(
        0,
        0,
        m_pWorkTextureSizes[to].x,
        m_pWorkTextureSizes[to].y,
        0,
        0,
        m_pWorkTextureSizes[from].x,
        m_pWorkTextureSizes[from].y,
        m_pShader,
        "gauss"
        );
}
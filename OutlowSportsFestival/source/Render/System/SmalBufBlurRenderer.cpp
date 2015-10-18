#include "SmalBufBlurRenderer.h"


SmalBufBlurRenderer::SmalBufBlurRenderer(UINT level, const char* ShaderFile) :
m_TextureNum(level),
m_pSource(nullptr)
{
    DWORD division;
    char path[MAX_PATH];

    sprintf_s<MAX_PATH>(path, ShaderFile);

    m_pShader = new iexShader(path);

    m_pWorkTextures = new iex2DObj*[m_TextureNum];
    m_pWorkTextureSizes = new DWORD2[m_TextureNum];
    m_pDepthStencils = new Surface*[m_TextureNum];


    for (UINT i = 0; i < m_TextureNum; ++i)
    {
        division = (DWORD)pow(2, (int)i);

        m_pWorkTextureSizes[i].x = iexSystem::ScreenWidth / division;
        m_pWorkTextureSizes[i].y = iexSystem::ScreenHeight / division;

        iexSystem::Device->CreateDepthStencilSurface(
            m_pWorkTextureSizes[i].x,
            m_pWorkTextureSizes[i].y,
            D3DFMT_D16,
            D3DMULTISAMPLE_NONE,
            0,
            FALSE,
            &m_pDepthStencils[i],
            NULL
            );

        m_pWorkTextures[i] = new iex2DObj(
            m_pWorkTextureSizes[i].x,
            m_pWorkTextureSizes[i].y,
            IEX2D_RENDERTARGET
            );
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
    delete[] m_pDepthStencils;
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

    iexSystem::Device->SetDepthStencilSurface(pSaveDepthStencil);
    pSaveDepthStencil->Release();

    float alpha = 0.85f;

    //ブラーテクスチャを出力
    for (UINT i = 0; i < m_TextureNum; ++i)
    {
        m_pShader->SetValue("g_Alpha", alpha);

        alpha *= 0.85f;

        m_pWorkTextures[i]->Render(
            0, 0, iexSystem::ScreenWidth, iexSystem::ScreenHeight,
            0, 0, m_pWorkTextureSizes[i].x, m_pWorkTextureSizes[i].y,
            m_pShader,
            "add"
            );
    }

    
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

    iexSystem::Device->SetDepthStencilSurface(m_pDepthStencils[0]);

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

    iexSystem::Device->SetDepthStencilSurface(m_pDepthStencils[to]);

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
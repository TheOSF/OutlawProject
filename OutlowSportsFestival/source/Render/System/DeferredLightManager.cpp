#include "DeferredLightManager.h"
#include <string>

DeferredLightManager::DeferredLightManager(
    UINT ScreenX,
    UINT ScreenY,
    const char* pData_FilePath
    ):
    m_ScreenX(ScreenX),
    m_ScreenY(ScreenY)
{

    char work_path[MAX_PATH];

    //カラー出力先テクスチャを生成
    m_pColorTexture = new iex2DObj(
        (DWORD)m_ScreenX,
        (DWORD)m_ScreenY,
        IEX2D_RENDERTARGET
        );


    //高輝度出力先テクスチャを生成
    m_pHRTexture = new iex2DObj(
        (DWORD)m_ScreenX,
        (DWORD)m_ScreenY,
        IEX2D_RENDERTARGET
        );


    //パーティクルテクスチャ読み込み
    sprintf_s<MAX_PATH>(work_path, "%s%s", pData_FilePath, "\\particle.png");
    m_pParticleTexture = new iex2DObj(work_path);

    //ジオメトリバッファ管理クラス生成
    sprintf_s<MAX_PATH>(work_path, "%s%s", pData_FilePath, "\\DeferredGbuf.fx");
   
    m_pGbufRenderer = new DeferredGbufRenderer(
        m_ScreenX,
        m_ScreenY,
        work_path
        );

    //ライトバッファ管理クラス生成
    m_pLightRenderer = new DeferredLightBufRenderer(
        m_ScreenX,
        m_ScreenY,
        pData_FilePath
        );

    //ライトバッファ管理クラスにGバッファテクスチャをセット
    m_pLightRenderer->SetGbufTextures(
        &m_pGbufRenderer->NormalDepth,
        &m_pGbufRenderer->RoughnessLightMap
        );

    //HDR描画管理クラス生成
    sprintf_s<MAX_PATH>(work_path, "%s%s", pData_FilePath, "\\Blur.fx");
    m_pSmalBufBlurRenderer = new SmalBufBlurRenderer(
        8,
        work_path
        );

    m_pSmalBufBlurRenderer->SetSourceTexture(m_pHRTexture);
}


DeferredLightManager::~DeferredLightManager()
{
    delete m_pLightRenderer;
    delete m_pGbufRenderer;
    delete m_pColorTexture;
    delete m_pHRTexture;
    delete m_pParticleTexture;
    delete m_pSmalBufBlurRenderer;
}


//ビュー関連のパラメータを設定
void DeferredLightManager::SetViewParam(
    const Matrix& View,
    const Matrix& Proj,
    const Vector3& CameraPos
    )
{
    Matrix VP_mat = View;
    VP_mat *= Proj;

    m_pGbufRenderer->UpdateView(VP_mat);
    m_pLightRenderer->UpdateViewParam(VP_mat, CameraPos);

}


void DeferredLightManager::Render(
    DeferredGbufRenderer::IRenderer*        pGbufRenderer,
    DeferredLightBufRenderer::IRenderer*    pLightRenderer,
    IMasterRenderer*                        pMasterRenderer,
    IForwardRenderer*                       pForwardRenderer,
    IPostEffectRenderer*                    pPostEffectRenderer
    )
{
    //スクリーンサーフェイスを保存
    Surface* pScreen;
    iexSystem::Device->GetRenderTarget(0, &pScreen);

    //テクスチャ初期化
    ClearTexture();

    //ジオメトリバッファ描画
    m_pGbufRenderer->Clear();
    m_pGbufRenderer->Render(pGbufRenderer);

    //ジオメトリバッファを元にライトバッファ描画
    m_pLightRenderer->Clear();
    m_pLightRenderer->Render(pLightRenderer);

    //ライトバッファを元にライティング後のテクスチャを描画
    pMasterRenderer->Render(
        m_pLightRenderer->m_pDiffuseTexture,
        m_pLightRenderer->m_pSpecularTexture,
        m_pColorTexture,
        m_pHRTexture
        );

    //色テクスチャをレンダーターゲットに設定
    m_pColorTexture->RenderTarget(0);
    for (DWORD i = 1; i < 4; ++i)
    {
        iexSystem::Device->SetRenderTarget(1, 0);
    }

    //フォワードレンダリングを実行
    pForwardRenderer->Render();


    //高輝度部分をブラー処理し、カラーテクスチャに出力
    m_pSmalBufBlurRenderer->Render();

    //ポストエフェクト処理し、バックバッファに出力
    pPostEffectRenderer->Render(m_pColorTexture, pScreen);

}



//デバッグ用テクスチャ描画
void DeferredLightManager::TextureRender()
{
  
    LPIEX2DOBJ tex[] =
    {
        &m_pGbufRenderer->NormalDepth,
        &m_pGbufRenderer->RoughnessLightMap,

        m_pLightRenderer->m_pDiffuseTexture,
        m_pLightRenderer->m_pSpecularTexture,

        m_pLightRenderer->m_pShadowDepthTexture,

        m_pHRTexture
    };

    const int sizeX = (int)m_ScreenX / 6;
    const int sizeY = (int)m_ScreenY / 6;
    int x = 0;
    int y = 0;

    for (int i = 0; i < ARRAYSIZE(tex); ++i)
    {
        if (i % 3 == 0 && i>0)
        {
            x = 0;
            y += sizeY;
        }

        //描画
        tex[i]->Render(
            x, y, sizeX, sizeY,
            0, 0,
            1280,
            720
            );

        x += sizeX;
    }
}


//テクスチャクリア
void DeferredLightManager::ClearTexture()
{
    //色テクスチャをレンダーターゲットに設定

    m_pColorTexture->RenderTarget(0);

    iexSystem::Device->Clear(0, 0, D3DCLEAR_TARGET, 0, 1, 0);

    m_pHRTexture->RenderTarget(0);

    iexSystem::Device->Clear(0, 0, D3DCLEAR_TARGET, 0, 1, 0);

}
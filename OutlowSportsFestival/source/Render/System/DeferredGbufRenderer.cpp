#include "DeferredGbufRenderer.h"


//---------------------------------------------
//  DeferredGbufRenderer::TechniqueSetter
//---------------------------------------------

DeferredGbufRenderer::TechniqueSetter::TechniqueSetter(DeferredGbufRenderer* pMgr) :
m_pMgr(pMgr)
{

}

DeferredGbufRenderer::TechniqueSetter::~TechniqueSetter()
{

}

//テクスチャを使用しないテクニックを得る
void DeferredGbufRenderer::TechniqueSetter::NoTexture(
    char*  out,
    size_t out_size)
{
    static const char* Technique = "NoTexture";

    //結果格納する容量チェック
    assert(strlen(Technique) + 1 < out_size && "格納文字列のサイズが足りません");

    //戻り値として格納
    strcpy(out, Technique);
}

//法線　テクスチャを使用するテクニックを得る
void DeferredGbufRenderer::TechniqueSetter::UseNormal(
    char*  out,
    size_t out_size)
{
    static const char* Technique = "UseNormal";

    //結果格納する容量チェック
    assert(strlen(Technique) + 1 < out_size && "格納文字列のサイズが足りません");


    //戻り値として格納
    strcpy(out, Technique);
}

//法線　高さ　テクスチャを使用するテクニックを得る
void DeferredGbufRenderer::TechniqueSetter::UseNormalHeight(
    char*  out,
    size_t out_size)
{
    static const char* Technique = "UseNormalHeight";

    //結果格納する容量チェック
    assert(strlen(Technique) + 1 < out_size && "格納文字列のサイズが足りません");

    //戻り値として格納
    strcpy(out, Technique);
}

//---------------------------------------------
//  public 
//---------------------------------------------

DeferredGbufRenderer::DeferredGbufRenderer(
    UINT  X,
    UINT  Y,
    const char* shader_file
    ) :
    m_X(X),
    m_Y(Y),
    NormalDepth         ((ULONG)X, (ULONG)Y, IEX2D_USEALPHA2),
    RoughnessLightMap   ((ULONG)X, (ULONG)Y, IEX2D_RENDERTARGET)
{
    //シェーダークラス作成
    char temp_fileName[MAX_PATH];
    strcpy_s<MAX_PATH>(temp_fileName, shader_file);

    m_pShader = new iexShader(temp_fileName);

}

DeferredGbufRenderer::~DeferredGbufRenderer()
{
    delete m_pShader;
}

//Gバッファをクリア
void DeferredGbufRenderer::Clear()
{
    //スクリーンのサーフェイスポインタを保存
    Surface*  pScreen;
    iexSystem::Device->GetRenderTarget(0, &pScreen);

    //レンダーターゲット設定
    SetRenderTarget();

    //描画
    iexPolygon::RectPlus(
        0,
        0,
        (int)m_X,
        (int)m_Y,
        m_pShader,
        "Clear",
        0xFFFFFFFF
        );


    //元に戻す
    iexSystem::Device->SetRenderTarget(0, pScreen);
    iexSystem::Device->SetRenderTarget(1, 0);

    pScreen->Release();
}

//Gバッファ描画
void DeferredGbufRenderer::Render(IRenderer* pRenderer)
{
    TechniqueSetter setter(this);

    //レンダーターゲット設定
    SetRenderTarget();

    //描画
    pRenderer->Render(m_pShader, &setter);
   
    //元に戻す
    iexSystem::Device->SetRenderTarget(1, 0);
}

//ビュー関連のパラメータの設定・更新
void DeferredGbufRenderer::UpdateView(
    const Matrix& VP_mat  //ビュー行列
    )
{
    Matrix m = VP_mat;
    m_pShader->SetValue("g_VP_mat", m);
    m = matView;
    m_pShader->SetValue("g_V_mat", m);
}

//---------------------------------------------
//  private
//---------------------------------------------

void DeferredGbufRenderer::SetRenderTarget()
{
    //レンダーターゲット設定
    NormalDepth.RenderTarget(0);
    RoughnessLightMap.RenderTarget(1);
}
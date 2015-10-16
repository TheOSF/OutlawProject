#ifndef __DEFERRED_MANAGER_H__
#define __DEFERRED_MANAGER_H__

#include "DeferredGbufRenderer.h"
#include "DeferredLightBufRenderer.h"
#include "SmalBufBlurRenderer.h"


//--------------------------------------------------
//  ディファード描画を管理するクラス
//--------------------------------------------------

class DeferredLightManager
{
public:
    //フォワード描画クラス
    class IForwardRenderer
    {
    public:
        virtual ~IForwardRenderer(){}
        virtual void Render() = 0;
    };

    //ライト結果を元にMRT描画を行うクラス(RT0=色,RT1=高輝度)
    class IMasterRenderer
    {
    public:
        virtual ~IMasterRenderer(){}
        virtual void Render(
            iex2DObj* pInDiffuseTexture,   //ライティング処理後の拡散反射光テクスチャ
            iex2DObj* pInSpecularTexture,  //ライティング処理後の鏡面反射光テクスチャ
            iex2DObj* pOutColorTexture,    //色情報を出力するテクスチャ
            iex2DObj* pOutHighRangeTexture //高輝度部分を出力するテクスチャ
            ) = 0;
    };

    //ポストエフェクト処理を行うインターフェイスクラス
    class IPostEffectRenderer
    {
    public:
        virtual ~IPostEffectRenderer(){}
        virtual void Render(
            iex2DObj* pIn,
            Surface*  pOut
            ) = 0;
    };

    DeferredLightManager(
        UINT ScreenX,              //スクリーン幅
        UINT ScreenY,              //スクリーン高
        const char* pData_FilePath //シェーダーフォルダへのパス
        );

    ~DeferredLightManager();

    //ビュー関連のパラメータを設定
    void SetViewParam(
        const Matrix& View,
        const Matrix& Proj,
        const Vector3& CameraPos
        );

    //描画
    void Render(
        DeferredGbufRenderer::IRenderer*        pGbufRenderer,
        DeferredLightBufRenderer::IRenderer*    pLightRenderer,
        IMasterRenderer*                        pMasterRenderer,
        IForwardRenderer*                       pForwardRenderer,
        IPostEffectRenderer*                    pPostEffectRenderer,
        IForwardRenderer*                       pUIRenderer
        );

    //デバッグ用テクスチャ描画
    void TextureRender();

    iex2DObj* GetNormalDepthTexture();

private:

    DeferredLightManager(const DeferredLightManager&);

    DeferredGbufRenderer*       m_pGbufRenderer;
    DeferredLightBufRenderer*   m_pLightRenderer;
    SmalBufBlurRenderer*        m_pSmalBufBlurRenderer;

    const UINT                  m_ScreenX;
    const UINT                  m_ScreenY;

    iex2DObj*                   m_pColorTexture;
    iex2DObj*                   m_pHRTexture;
 

    //テクスチャクリア
    void ClearTexture();
};

#endif
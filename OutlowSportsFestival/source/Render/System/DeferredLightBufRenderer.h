#ifndef __DEFERRED_LIGHT_BUF_RENDERER_H__
#define __DEFERRED_LIGHT_BUF_RENDERER_H__

#include "iextreme.h"
//-----------------------------------------------------------
//  Gバッファを元にライティングテクスチャを作成するクラス
//-----------------------------------------------------------

class DeferredLightBufRenderer
{
public:

    class IDepthRenderer
    {
    public:
        virtual ~IDepthRenderer(){}
        virtual void Render(iexShader* pShader, const char* technique) = 0;
    };

    //平行光パラメーター
    struct DirLightParam
    {
        enum class Type
        {
            Ortho,
            Perspective
        };

        Vector3 vec;
        Vector3 color;

        struct
        {
            
            bool              visible;
            IDepthRenderer*   pDepthRenderer;
            Vector3           origin;
            float             Near;
            float             Far;
            Type              type;
            float             Size;
        }
        Shadow;
    };

    //点光源パラメーター
    struct PointLightParam
    {
        Vector3 pos;
        float   size;
        Vector3 color;
    };

    //スポットライト
    struct SpotLightParam
    {
        Vector3 origin;
        Vector3 target;
        Vector3 color;
        float   size;

        struct
        {
            bool              visible;
            IDepthRenderer*   pDepthRenderer;
        }
        Shadow;
    };

    //半球ライティング
    struct HemiLightParam
    {
        Vector3 Up;
        Vector3 SkyColor;
        Vector3 GroundColor;
    };

    //環境ライティング
    struct AmbientParam
    {
        Vector3 color;
        
        struct
        {
            bool  Enable;
            float SamplingSize;
            float CalcDiffZ;
        }
        Occlusion;
    };

    //ライト描画クラス
    class LightRenderer
    {
    public:
        LightRenderer(DeferredLightBufRenderer* pMgr);
        
        void DirLight(DirLightParam& param);     //ディレクショナルライト描画
        void PointLight(PointLightParam& param); //ポイントライト描画
        void SpotLight(SpotLightParam& param);   //スポットライト描画
        void HemiLight(HemiLightParam& param);   //半球ライト描画
        void AmbientLight(AmbientParam& param);  //環境光描画
    private:
        void DirLightUseVarianceShadow(DirLightParam& param);
        void DirLightUseShadow(DirLightParam& param);
        void SpotLightUseShadow(SpotLightParam& param);

        DeferredLightBufRenderer* m_pMgr;
    };

    //ライト描画呼び出しを行うインターフェース
    class IRenderer
    {
    public:
        virtual ~IRenderer(){}
        virtual void Render(
            LightRenderer* pLightRenderer //ライト描画クラス
            ) = 0;
    };



    DeferredLightBufRenderer(
        UINT X,                     //スクリーン幅
        UINT Y,                     //スクリーン高さ
        const char* pData_FilePath  //ライト用 FX・MESHファイルへのパス
        );

    ~DeferredLightBufRenderer();
    
    //ライトバッファクリア
    void Clear();

    //ライトバッファに描画実行
    void Render(IRenderer* pRenderer);

    //計算の元となるGバッファテクスチャをセット
    void SetGbufTextures(
        iex2DObj* NormalDepth,           //法線・深度
        iex2DObj* RoughnessLightMap      //粗さ・ライトマップ値
    );

    //ビュー関連の行列を設定・更新する
    void UpdateViewParam(
        const Matrix& VP_mat,     //ビュープロジェクション行列
        const Vector3& CameraPos
        );

    iex2DObj*   m_pDiffuseTexture;           //ディフューズテクスチャ
    iex2DObj*   m_pSpecularTexture;          //スペキュラテクスチャ
    iex2DObj*   m_pShadowDepthTexture;       //影用Z値テクスチャ
private:

    const UINT     m_X, m_Y;              //スクリーンサイズXY
    Matrix         m_ViewMatrix;          //ビュー行列保管
    Matrix         m_ViewInvMatrix;       //ビュー逆行列保管
    iexShader*     m_pShader;             //シェーダークラス
    LPIEXMESH      m_pShpereMesh;         //点光源用球メッシュ
    LPIEXMESH      m_pSpotMesh;           //円柱光源用球メッシュ
                   
    Surface*       m_pDepthStencil;       //影用デプスステンシルバッファ
    D3DVIEWPORT9   m_ShadowViewport;     //影用ビューポート
  
};

#endif
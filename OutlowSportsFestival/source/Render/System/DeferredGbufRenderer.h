#ifndef __DEFERRED_GBUF_RENDERER_H__
#define __DEFERRED_GBUF_RENDERER_H__

#include "iextreme.h"

//-----------------------------------------------------------
//   Gバッファの生成・管理を行うクラス
//-----------------------------------------------------------

class DeferredGbufRenderer
{
public:

    //描画テクニックをセットするクラス
    class TechniqueSetter
    {
    public:
        TechniqueSetter(DeferredGbufRenderer* pMgr);
        ~TechniqueSetter();

        //テクスチャを使用しないテクニックを得る
        void NoTexture(
            char* out,
            size_t out_size
            );

        //法線　テクスチャを使用するテクニックを得る
        void UseNormal(
            char* out,
            size_t out_size
            );

        //法線　高さ　テクスチャを使用するテクニックを得る
        void UseNormalHeight(
            char* out,
            size_t out_size
            );
    private:
        DeferredGbufRenderer*  m_pMgr;
    };

    //ジオメトリ生成を行うクラス
    class IRenderer
    {
    public:
        virtual ~IRenderer(){}

        //描画
        virtual void Render(
            iexShader*        pShader, //シェーダークラス
            TechniqueSetter*  pSetter  //テクニック管理クラス
            ) = 0;
    };

    
    DeferredGbufRenderer(
        UINT  X,                   //スクリーンサイズX
        UINT  Y,                   //スクリーンサイズY
        const char* shader_file    //シェーダーファイルへのパス
        );

    ~DeferredGbufRenderer();

    //Gバッファをクリア
    void Clear();

    //Gバッファ描画
    void Render(IRenderer* pRenderer);

    //ビュー関連のパラメータの設定・更新
    void UpdateView(
        const Matrix& VP_mat  //ビュープロジェクション行列
        );

    iex2DObj NormalDepth;                   //法線xy プロジェクションzw
    iex2DObj RoughnessLightMap;             //粗さr ライトマップg
private:

    iexShader*         m_pShader;     //シェーダークラス
    const UINT         m_X;           //スクリーンサイズX
    const UINT         m_Y;           //スクリーンサイズY

    DeferredGbufRenderer(const DeferredGbufRenderer&);

    void SetRenderTarget();
};

#endif
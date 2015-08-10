#ifndef __RENDERER_H__
#define __RENDERER_H__

//*************************************************
//	描画クラスベース
//*************************************************
#include <map>

//ディファード描画用クラス
class DeferredRenderer
{
public:
	//コンストラクタ・デストラクタで自動的に描画をするための登録をする
	DeferredRenderer();
	virtual ~DeferredRenderer();

	//描画(自動的に呼ばれる)
	virtual void Render() = 0;
};

typedef DeferredRenderer* LpDeferredRenderer;

//フォワード描画用クラス(透明オブジェクト用)
class ForwardRenderer
{
public:
	//コンストラクタ・デストラクタで自動的に描画をするための登録をする
	ForwardRenderer();
	virtual ~ForwardRenderer();
	
	//この変数をもとに値が低い順番にSortする
	float m_SortZ;
	
	//Z値計算をこの関数で実行することができる
	virtual void CalcZ() = 0;

	//描画(自動的に呼ばれる)
	virtual void Render() = 0;
};
typedef ForwardRenderer* LpForwardRenderer;



//*************************************************
//	描画マネージャ
//*************************************************
class RendererManager
{
public:
	static RendererManager& GetInstance();
	static void Release();

	//ディファード描画用クラスの追加・削除
	bool AddDeferredRenderer(LpDeferredRenderer pDef);
	bool EraceDeferredRenderer(LpDeferredRenderer pDef);

	//フォワード描画用クラスの追加・削除
	bool AddForwardRenderer(LpForwardRenderer pFor);
	bool EraceForwardRenderer(LpForwardRenderer pFor);

    //描画
    void Render();
private:

	RendererManager();
	~RendererManager();

    //ディファード描画
    void DeferredRender();

    //フォワード描画
    void ForwardRender();

    //カラーバッファの内容を引数のサーフェイスに移す
    void RenderToBackBuffer(Surface* pSurface);

    //Gバッファ描画
    void CreateGbuf();

    //ソフトパーティクル描画
    void RenderSoftParticle();

    //テクスチャをブラーさせる
    void BlurTexture(iex2DObj* pTex);

    //HDR部分をカラーカラーバッファに加算
    void RenderAddHDR();

    //ポストエフェクト描画
    void RenderPostEffect();

	typedef std::map<LpDeferredRenderer, LpDeferredRenderer> DeferredRendererMap;
	typedef std::map<LpForwardRenderer, LpForwardRenderer> ForwardRendererMap;

	static RendererManager* m_pInstance;

	DeferredRendererMap m_DeferredRendererMap;
	ForwardRendererMap  m_ForwardRendererMap;

    enum TextureType
    {
        _ColorTexture,
        _HdrDepthTexture,
        _WorkTexture,

        __MaxTexture
    };

    iex2DObj* m_pTextures[__MaxTexture];

    const int m_WorkTextureSizeX;
    const int m_WorkTextureSizeY;
};

#define DefRendererMgr (RendererManager::GetInstance())

#endif
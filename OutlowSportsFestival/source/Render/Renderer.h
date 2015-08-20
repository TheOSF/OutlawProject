#ifndef __RENDERER_H__
#define __RENDERER_H__

//*************************************************
//	描画クラスベース
//*************************************************
#include <map>
#include "System\DeferredLightManager.h"
#include "System\BlurEffectRenderer.h"
#include "../IexSystem/System.h"

//ディファード描画用クラス
class DeferredRenderer
{
public:
	//コンストラクタ・デストラクタで自動的に描画をするための登録をする
	DeferredRenderer();
	virtual ~DeferredRenderer();

	//Gバッファ描画(自動的に呼ばれる)
	virtual void GbufRender(
        iexShader*        pShader,                       //シェーダークラス
        DeferredGbufRenderer::TechniqueSetter*  pSetter  //テクニック管理クラス
        ) = 0;

    //本番描画(自動的に呼ばれる、RT０＝色　RT1＝高輝度)
    virtual void MasterRender() = 0;

    virtual void DepthRender(iexShader* pShader, const char* pTec) = 0;
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


//ライト描画クラス
class LightObject
{
public:
    typedef DeferredLightBufRenderer::LightRenderer LightRenderer;
    
    LightObject();
    virtual~LightObject();
    
    virtual void Render(LightRenderer* pLightRenderer) = 0;
};
typedef LightObject* LpLightObject;

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

    //フォワード描画用クラスの追加・削除
    bool AddLightObject(LpLightObject pL);
    bool EraceLightObject(LpLightObject pL);

    //描画
    void Render();

    //Z値描画オブジェクトへのポインタを得る
    DeferredLightBufRenderer::IDepthRenderer* GetDepthRenderer();
private:

	RendererManager();
	~RendererManager();

	typedef std::map<LpDeferredRenderer, LpDeferredRenderer> DeferredRendererMap;
	typedef std::map<LpForwardRenderer, LpForwardRenderer> ForwardRendererMap;
    typedef std::map<LpLightObject, LpLightObject> LightObjectMap;


	static RendererManager* m_pInstance;

    class IRenderer
    {
    protected:
        RendererManager* m_pMgr;
    public:
        void SetMgr(RendererManager* pMgr);
    };

    class GbufRenderer :public DeferredGbufRenderer::IRenderer, public IRenderer
    {
    public:
        void Render(
            iexShader*        pShader, //シェーダークラス
            DeferredGbufRenderer::TechniqueSetter*  pSetter  //テクニック管理クラス
            );
    };

    class LightbufRenderer :public DeferredLightBufRenderer::IRenderer, public IRenderer
    {
    public:
        void Render(
            DeferredLightBufRenderer::LightRenderer* pLightRenderer //ライト描画クラス
            );
    };

    class MasterRenderer :public DeferredLightManager::IMasterRenderer, public IRenderer
    {
    public:
        void Render(
            iex2DObj* pInDiffuseTexture,   //ライティング処理後の拡散反射光テクスチャ
            iex2DObj* pInSpecularTexture,  //ライティング処理後の鏡面反射光テクスチャ
            iex2DObj* pOutColorTexture,    //色情報を出力するテクスチャ
            iex2DObj* pOutHighRangeTexture //高輝度部分を出力するテクスチャ
             );
    };

    class ForwardRenderer :public DeferredLightManager::IForwardRenderer, public IRenderer
    {
    public:
        void Render();
    };

    class DepthRenderer :public DeferredLightBufRenderer::IDepthRenderer, public IRenderer
    {
    public:
        void Render(iexShader* pShader, const char* technique);
    };


	DeferredRendererMap   m_DeferredRendererMap;
	ForwardRendererMap    m_ForwardRendererMap;
    LightObjectMap        m_LightObjectMap;

    DeferredLightManager  m_DeferredLightManager;

    BlurEffectRenderer    m_BlurEffectRenderer;
    DepthRenderer         m_DepthRenderer;
};

#define DefRendererMgr (RendererManager::GetInstance())

#endif
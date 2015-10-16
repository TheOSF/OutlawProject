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
    enum class DepthRenderType
    {
        DirLight,
        SpotLight,
    };

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

    virtual void DepthRender(iexShader* pShader, const char* pTec, DepthRenderType type) = 0;
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



//UI描画用クラス
class UserInterfaceRenderer
{
public:
    //コンストラクタ・デストラクタで自動的に描画をするための登録をする
    UserInterfaceRenderer();
    virtual ~UserInterfaceRenderer();

    //この変数をもとに値が低い順番にSortする
    float m_SortZ;

    //Z値計算をこの関数で実行することができる
    virtual void CalcZ() = 0;

    //描画(自動的に呼ばれる)
    virtual void Render() = 0;
};
typedef UserInterfaceRenderer* LpUserInterfaceRenderer;



//ライト描画クラス
class LightObject
{
public:
    typedef DeferredLightBufRenderer::LightRenderer LightRenderer;

    bool Visible;   //表示フラグ
    
    LightObject();
    virtual~LightObject();
    
    virtual void Render(LightRenderer* pLightRenderer) = 0;
};
typedef LightObject* LpLightObject;


//ディファード後のHDR描画クラス
class ForwardHDRRenderer
{
public:
    ForwardHDRRenderer();
    virtual ~ForwardHDRRenderer();

    //この変数をもとに値が低い順番にSortする
	float m_SortZ;
	
	//Z値計算をこの関数で実行することができる
	virtual void CalcZ() = 0;

	//描画(自動的に呼ばれる)
	virtual void Render() = 0;
};
typedef ForwardHDRRenderer* LpForwardHDRRenderer;


//ブラーポストエフェクト描画クラス
class PostEffectRenderer
{
public:
    typedef std::list<BlurEffectRenderer::SphereBlur> SphereBlurList;
    typedef std::list<BlurEffectRenderer::ConeBlur> ConeBlurList;

    PostEffectRenderer();
    virtual ~PostEffectRenderer();

    virtual void Render(
        SphereBlurList& SphereList,
        ConeBlurList&   ConeList
        ) = 0;
};
typedef PostEffectRenderer* LpPostEffectRenderer;

//*************************************************
//	描画マネージャ
//*************************************************
class RendererManager
{
public:
    class IRenderer
    {
    protected:
        RendererManager* m_pMgr;
    public:
        void SetMgr(RendererManager* pMgr);
    };

    //深度描画クラス
    class DepthRenderer :public DeferredLightBufRenderer::IDepthRenderer, public IRenderer
    {
    public:
        DeferredRenderer::DepthRenderType m_Type;

        DepthRenderer();

        void Render(iexShader* pShader, const char* technique);
    };

	static RendererManager& GetInstance();
	static void Release();

	//ディファード描画用クラスの追加・削除
	bool AddDeferredRenderer(LpDeferredRenderer pDef);
	bool EraceDeferredRenderer(LpDeferredRenderer pDef);

	//フォワード描画用クラスの追加・削除
	bool AddForwardRenderer(LpForwardRenderer pFor);
	bool EraceForwardRenderer(LpForwardRenderer pFor);

    //UI描画用クラスの追加・削除
    bool AddUIRenderer(LpUserInterfaceRenderer p);
    bool EraceUIRenderer(LpUserInterfaceRenderer p);

    //ライト描画用クラスの追加・削除
    bool AddLightObject(LpLightObject pL);
    bool EraceLightObject(LpLightObject pL);

    //フォワードHDR描画用クラスの追加・削除
    bool AddForwardHDRRenderer(LpForwardHDRRenderer pForHDR);
    bool EraceForwardHDRRenderer(LpForwardHDRRenderer pForHDR);

    //ブラーポストエフェクト描画クラスの追加・削除
    bool AddPostEffectRenderer(LpPostEffectRenderer p);
    bool EracePostEffectRenderer(LpPostEffectRenderer p);


    //描画
    void Render();

    //Z値描画オブジェクトへのポインタを得る
    DepthRenderer* GetDepthRenderer();

    iex2DObj* GetNormalDepthTexture();

private:

	RendererManager();
	~RendererManager();

    void SetBlurParameters();

	typedef std::map<LpDeferredRenderer, LpDeferredRenderer> DeferredRendererMap;
    typedef std::map<LpForwardHDRRenderer, LpForwardHDRRenderer> ForwardHDRRendererMap;
    typedef std::map<LpForwardRenderer, LpForwardRenderer> ForwardRendererMap;
    typedef std::map<LpUserInterfaceRenderer, LpUserInterfaceRenderer> UIRendererMap;
    typedef std::map<LpLightObject, LpLightObject> LightObjectMap;
    typedef std::map<LpPostEffectRenderer, LpPostEffectRenderer> BlurObjectMap;

	static RendererManager* m_pInstance;

    
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

    class UIRenderer :public DeferredLightManager::IForwardRenderer, public IRenderer
    {
    public:
        void Render();
    };

	DeferredRendererMap      m_DeferredRendererMap;
    ForwardHDRRendererMap    m_ForwardHDRRendererMap;
	ForwardRendererMap       m_ForwardRendererMap;
    UIRendererMap            m_UIRendererMap;
    LightObjectMap           m_LightObjectMap;
    BlurObjectMap            m_BlurObjectMap;

    DeferredLightManager  m_DeferredLightManager;

    BlurEffectRenderer    m_BlurEffectRenderer;
    DepthRenderer         m_DepthRenderer;
};

#define DefRendererMgr (RendererManager::GetInstance())

#endif
#include "Renderer.h"
#include "../debug/DebugFunction.h"
#include "../IexSystem/System.h"


RendererManager* RendererManager::m_pInstance = nullptr;

//*************************************************
//	描画クラスベース
//*************************************************

DeferredRenderer::DeferredRenderer()
{
#ifdef _DEBUG
	MyAssert(
		DefRendererMgr.AddDeferredRenderer(this),
		"DeferredRendererの追加に失敗しました");
#else
    DefRendererMgr.AddDeferredRenderer(this);
#endif
}



DeferredRenderer::~DeferredRenderer()
{
#ifdef _DEBUG
	MyAssert(
		DefRendererMgr.EraceDeferredRenderer(this),
		"DeferredRendererの削除に失敗しました");
#else 
    DefRendererMgr.EraceDeferredRenderer(this);
#endif
}



ForwardRenderer::ForwardRenderer() :
m_SortZ(1000)
{
#ifdef _DEBUG
	MyAssert(
		DefRendererMgr.AddForwardRenderer(this),
		"ForwardRendererの追加に失敗しました");
#else
    DefRendererMgr.AddForwardRenderer(this);
#endif
}

ForwardRenderer::~ForwardRenderer()
{
#ifdef _DEBUG
	MyAssert(
		DefRendererMgr.EraceForwardRenderer(this),
		"ForwardRendererの削除に失敗しました");

#else
    DefRendererMgr.EraceForwardRenderer(this);
#endif
}


UserInterfaceRenderer::UserInterfaceRenderer():
m_SortZ(0)
{
#ifdef _DEBUG
    MyAssert(
        DefRendererMgr.AddUIRenderer(this),
        "UserInterfaceRendererの追加に失敗しました");
#else
    DefRendererMgr.AddUIRenderer(this);
#endif
}

UserInterfaceRenderer::~UserInterfaceRenderer()
{
#ifdef _DEBUG
    MyAssert(
        DefRendererMgr.EraceUIRenderer(this),
        "UserInterfaceRendererの削除に失敗しました");
#else
    DefRendererMgr.EraceUIRenderer(this);
#endif
}

LightObject::LightObject() :
Visible(true)
{
#ifdef _DEBUG
    MyAssert(
    DefRendererMgr.AddLightObject(this),
        "LightObjectの追加に失敗しました");

#else
    DefRendererMgr.AddLightObject(this);
#endif
}

LightObject::~LightObject()
{
#ifdef _DEBUG
    MyAssert(
        DefRendererMgr.EraceLightObject(this),
        "LightObjectの削除に失敗しました");

#else
    DefRendererMgr.EraceLightObject(this);
#endif
}


ForwardHDRRenderer::ForwardHDRRenderer()
{
#ifdef _DEBUG
    MyAssert(
        DefRendererMgr.AddForwardHDRRenderer(this),
        "ForwardHDRRendererの追加に失敗しました");

#else
    DefRendererMgr.AddForwardHDRRenderer(this);
#endif
}

ForwardHDRRenderer::~ForwardHDRRenderer()
{
#ifdef _DEBUG
    MyAssert(
        DefRendererMgr.EraceForwardHDRRenderer(this),
        "ForwardHDRRendererの削除に失敗しました");
#else
    DefRendererMgr.EraceForwardHDRRenderer(this);
#endif
}


PostEffectRenderer::PostEffectRenderer()
{
#ifdef _DEBUG
    MyAssert(
        DefRendererMgr.AddPostEffectRenderer(this),
        "PostEffectRendererの追加に失敗しました");
#else
    DefRendererMgr.AddPostEffectRenderer(this);
#endif
}

PostEffectRenderer::~PostEffectRenderer()
{
#ifdef _DEBUG
    MyAssert(
        DefRendererMgr.EracePostEffectRenderer(this),
        "PostEffectRendererの削除に失敗しました");

#else
    DefRendererMgr.EracePostEffectRenderer(this);
#endif
}

//*************************************************
//	描画マネージャ
//*************************************************

RendererManager& RendererManager::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new RendererManager();
	}
	return *m_pInstance;
}

void RendererManager::Release()
{
	delete m_pInstance;
	m_pInstance = nullptr;
}


//ディファード描画用クラスの追加・削除
bool RendererManager::AddDeferredRenderer(LpDeferredRenderer pDef)
{
	if (m_DeferredRendererMap.find(pDef) != m_DeferredRendererMap.end())
	{
		return false;
	}

	m_DeferredRendererMap.insert(
		DeferredRendererMap::value_type(pDef, pDef)
		);

	return true;
}

bool RendererManager::EraceDeferredRenderer(LpDeferredRenderer pDef)
{
	auto it = m_DeferredRendererMap.find(pDef);

	if (it == m_DeferredRendererMap.end())
	{
		return false;
	}

	m_DeferredRendererMap.erase(it);
	return true;
}


//フォワード描画用クラスの追加・削除
bool RendererManager::AddForwardRenderer(LpForwardRenderer pDef)
{
	if (m_ForwardRendererMap.find(pDef) != m_ForwardRendererMap.end())
	{
		return false;
	}

	m_ForwardRendererMap.insert(
		ForwardRendererMap::value_type(pDef, pDef)
		);

	return true;
}

bool RendererManager::EraceForwardRenderer(LpForwardRenderer pDef)
{
	auto it = m_ForwardRendererMap.find(pDef);

	if (it == m_ForwardRendererMap.end())
	{
		return false;
	}

	m_ForwardRendererMap.erase(it);
	return true;
}


//フォワード描画用クラスの追加・削除
bool RendererManager::AddUIRenderer(LpUserInterfaceRenderer p)
{
    if (m_UIRendererMap.find(p) != m_UIRendererMap.end())
    {
        return false;
    }

    m_UIRendererMap.insert(
        UIRendererMap::value_type(p, p)
        );

    return true;
}

bool RendererManager::EraceUIRenderer(LpUserInterfaceRenderer p)
{
    auto it = m_UIRendererMap.find(p);

    if (it == m_UIRendererMap.end())
    {
        return false;
    }

    m_UIRendererMap.erase(it);
    return true;
}

//ライト描画用クラスの追加・削除
bool RendererManager::AddLightObject(LpLightObject pL)
{
    if (m_LightObjectMap.find(pL) != m_LightObjectMap.end())
    {
        return false;
    }

    m_LightObjectMap.insert(
        LightObjectMap::value_type(pL, pL)
        );

    return true;
}

bool RendererManager::EraceLightObject(LpLightObject pL)
{
    auto it = m_LightObjectMap.find(pL);

    if (it == m_LightObjectMap.end())
    {
        return false;
    }

    m_LightObjectMap.erase(it);
    return true;
}


//フォワードHDR描画用クラスの追加・削除
bool RendererManager::AddForwardHDRRenderer(LpForwardHDRRenderer pForHDR)
{
    if (m_ForwardHDRRendererMap.find(pForHDR) != m_ForwardHDRRendererMap.end())
    {
        return false;
    }

    m_ForwardHDRRendererMap.insert(
        ForwardHDRRendererMap::value_type(pForHDR, pForHDR)
        );

    return true;
}

bool RendererManager::EraceForwardHDRRenderer(LpForwardHDRRenderer pForHDR)
{
    auto it = m_ForwardHDRRendererMap.find(pForHDR);

    if (it == m_ForwardHDRRendererMap.end())
    {
        return false;
    }

    m_ForwardHDRRendererMap.erase(it);
    return true;
}



//ブラーポストエフェクト描画クラスの追加・削除
bool RendererManager::AddPostEffectRenderer(LpPostEffectRenderer p)
{
    if (m_BlurObjectMap.find(p) != m_BlurObjectMap.end())
    {
        return false;
    }

    m_BlurObjectMap.insert(
        BlurObjectMap::value_type(p,p)
        );

    return true;
}

bool RendererManager::EracePostEffectRenderer(LpPostEffectRenderer p)
{
    auto it = m_BlurObjectMap.find(p);

    if (it == m_BlurObjectMap.end())
    {
        return false;
    }

    m_BlurObjectMap.erase(it);
    return true;
}


//描画
void RendererManager::Render()
{
    Matrix VP;
    VP = matView;
    VP *= matProjection;

    shader->SetValue("g_V_mat", matView);
    shader->SetValue("g_VP_mat", VP);

    m_DeferredLightManager.SetViewParam(matView, matProjection, Vector3Zero);


    GbufRenderer Gr;
    LightbufRenderer Lr;
    MasterRenderer Mr;
    ForwardRenderer Fr;
    UIRenderer Ur;

    Gr.SetMgr(this);
    Lr.SetMgr(this);
    Mr.SetMgr(this);
    Fr.SetMgr(this);
    Ur.SetMgr(this);

    //ブラーパラメータの更新
    SetBlurParameters();

    m_DeferredLightManager.Render(
        &Gr,
        &Lr,
        &Mr,
        &Fr,
        &m_BlurEffectRenderer,
        &Ur
        );

    if (GetKeyState('G'))
    m_DeferredLightManager.TextureRender();
}


//---------------------------------------------------------------------
//   レンダラー
//---------------------------------------------------------------------

//共通クラス
void RendererManager::IRenderer::SetMgr(RendererManager* pMgr)
{
    m_pMgr = pMgr;
}


//Gバッファ描画
void RendererManager::GbufRenderer::Render(
    iexShader*        pShader, //シェーダークラス
    DeferredGbufRenderer::TechniqueSetter*  pSetter  //テクニック管理クラス
    )
{
	for (auto it = m_pMgr->m_DeferredRendererMap.begin();
        it != m_pMgr->m_DeferredRendererMap.end();
		++it)
	{
        it->second->GbufRender(pShader, pSetter);
	}
}


//ライトバッファ描画
void RendererManager::LightbufRenderer::Render(
    DeferredLightBufRenderer::LightRenderer* pLightRenderer //ライト描画クラス
    )
{
    for (auto it = m_pMgr->m_LightObjectMap.begin();
        it != m_pMgr->m_LightObjectMap.end();
        ++it)
    {
        if (it->second->Visible)
        {
            it->second->Render(pLightRenderer);
        }
    }
}


//Z値比較用関数
static int CompareForwardHDRRendererFunc(const void*p1, const void* p2)
{
    return ((**(LpForwardRenderer*)p1).m_SortZ > (**(LpForwardRenderer*)p2).m_SortZ) ? (-1) : (1);
}


//マスター描画
void RendererManager::MasterRenderer::Render(
    iex2DObj* pInDiffuseTexture,   //ライティング処理後の拡散反射光テクスチャ
    iex2DObj* pInSpecularTexture,  //ライティング処理後の鏡面反射光テクスチャ
    iex2DObj* pOutColorTexture,    //色情報を出力するテクスチャ
    iex2DObj* pOutHighRangeTexture //高輝度部分を出力するテクスチャ
    )
{
    shader->SetValue("DiffuseLightMap", pInDiffuseTexture->GetTexture());
    shader->SetValue("SpecuarLightMap", pInSpecularTexture->GetTexture());

    pOutColorTexture->RenderTarget(0);
    pOutHighRangeTexture->RenderTarget(1);

    
    for (auto it = m_pMgr->m_DeferredRendererMap.begin();
        it != m_pMgr->m_DeferredRendererMap.end();
        ++it)
    {
        it->second->MasterRender();
    }


    
    if (m_pMgr->m_ForwardHDRRendererMap.empty())
    {
        return;
    }

    //フォワードHDR描画を実行する

    LpForwardHDRRenderer* SortData = new LpForwardHDRRenderer[m_pMgr->m_ForwardHDRRendererMap.size()];

    try
    {
        int count = 0;

        for (auto it = m_pMgr->m_ForwardHDRRendererMap.begin();
            it != m_pMgr->m_ForwardHDRRendererMap.end();
            ++it)
        {
            SortData[count] = it->second;
            ++count;

            //Z値を計算する
            it->second->CalcZ();
        }

        //遠い順番にソート
        std::qsort(
            &SortData[0],
            count,
            sizeof(LpForwardHDRRenderer),
            CompareForwardHDRRendererFunc
            );

        //描画
        for (int i = 0; i < count; ++i)
        {
            SortData[i]->Render();
        }

    }
    catch (...)
    {
        delete[]SortData;
        throw;
    }

    delete[]SortData;

}

//Z値比較用関数
static int CompareForwardRendererFunc(const void*p1, const void* p2)
{
	return ((**(LpForwardRenderer*)p1).m_SortZ > (**(LpForwardRenderer*)p2).m_SortZ) ? (-1) : (1);
}

//フォワード描画
void RendererManager::ForwardRenderer::Render()
{
	if (m_pMgr->m_ForwardRendererMap.empty())
	{
		return;
	}

	//ソート結果用配列を生成
    LpForwardRenderer* SortData = new LpForwardRenderer[m_pMgr->m_ForwardRendererMap.size()];

	try
	{
		int count = 0;

        for (auto it = m_pMgr->m_ForwardRendererMap.begin();
            it != m_pMgr->m_ForwardRendererMap.end();
			++it)
		{
			SortData[count] = it->second;
            ++count;

			//Z値を計算する
			it->second->CalcZ();
		}

		//遠い順番にソート
		std::qsort(
			&SortData[0],
			count,
			sizeof(LpForwardRenderer),
            CompareForwardRendererFunc
			);

		//描画
		for (int i = 0; i < count; ++i)
		{
			SortData[i]->Render();
		}

	}
	catch (...)
	{
		delete[]SortData;
		throw;
	}

	delete[]SortData;

}


//Z値比較用関数
static int CompareUIRendererFunc(const void*p1, const void* p2)
{
    return ((**(LpUserInterfaceRenderer*)p1).m_SortZ > (**(LpUserInterfaceRenderer*)p2).m_SortZ) ? (-1) : (1);
}

void RendererManager::UIRenderer::Render()
{
    if (m_pMgr->m_UIRendererMap.empty())
    {
        return;
    }

    //ソート結果用配列を生成
    LpUserInterfaceRenderer* SortData = new LpUserInterfaceRenderer[m_pMgr->m_UIRendererMap.size()];

    try
    {
        int count = 0;

        for (auto it = m_pMgr->m_UIRendererMap.begin();
            it != m_pMgr->m_UIRendererMap.end();
            ++it)
        {
            SortData[count] = it->second;
            ++count;

            //Z値を計算する
            it->second->CalcZ();
        }

        //遠い順番にソート
        std::qsort(
            &SortData[0],
            count,
            sizeof(LpUserInterfaceRenderer),
            CompareUIRendererFunc
            );

        //描画
        for (int i = 0; i < count; ++i)
        {
            SortData[i]->Render();
        }

    }
    catch (...)
    {
        delete[]SortData;
        throw;
    }

    delete[]SortData;
}

RendererManager::DepthRenderer::DepthRenderer() :
m_Type(DeferredRenderer::DepthRenderType::DirLight)
{

}

void RendererManager::DepthRenderer::Render(iexShader* pShader, const char* technique)
{
    for (auto it = m_pMgr->m_DeferredRendererMap.begin();
        it != m_pMgr->m_DeferredRendererMap.end();
        ++it)
    {
        it->second->DepthRender(pShader, technique, m_Type);
    }
}

RendererManager::RendererManager():
m_DeferredLightManager(iexSystem::ScreenWidth, iexSystem::ScreenHeight, "DATA\\Shader\\Shader"),
m_BlurEffectRenderer("DATA\\Shader\\Shader")
{
    m_DepthRenderer.SetMgr(this);
}

RendererManager::~RendererManager()
{

}

void RendererManager::SetBlurParameters()
{
    //ブラーリストの更新
    m_BlurEffectRenderer.m_BlurSphere.clear();
    m_BlurEffectRenderer.m_BlurCone.clear();

    for (auto& it : m_BlurObjectMap)
    {
        it.first->Render(
            m_BlurEffectRenderer.m_BlurSphere,
            m_BlurEffectRenderer.m_BlurCone
            );
    }
}

RendererManager::DepthRenderer* RendererManager::GetDepthRenderer()
{
    return &m_DepthRenderer;
}

iex2DObj* RendererManager::GetNormalDepthTexture()
{
    return m_DeferredLightManager.GetNormalDepthTexture();
}
#include "Renderer.h"
#include "../debug/DebugFunction.h"
#include "../IexSystem/System.h"


RendererManager* RendererManager::m_pInstance = nullptr;

//*************************************************
//	描画クラスベース
//*************************************************

DeferredRenderer::DeferredRenderer()
{
	MyAssert(
		DefRendererMgr.AddDeferredRenderer(this),
		"DeferredRendererの追加に失敗しました");
}



DeferredRenderer::~DeferredRenderer()
{
	MyAssert(
		DefRendererMgr.EraceDeferredRenderer(this),
		"DeferredRendererの削除に失敗しました");
}



ForwardRenderer::ForwardRenderer() :
m_SortZ(1000)
{
	MyAssert(
		DefRendererMgr.AddForwardRenderer(this),
		"ForwardRendererの追加に失敗しました");
}

ForwardRenderer::~ForwardRenderer()
{
	MyAssert(
		DefRendererMgr.EraceForwardRenderer(this),
		"ForwardRendererの削除に失敗しました");
}


LightObject::LightObject() :
Visible(true)
{

    MyAssert(
    DefRendererMgr.AddLightObject(this),
        "LightObjectの追加に失敗しました");
}

LightObject::~LightObject()
{

    MyAssert(
        DefRendererMgr.EraceLightObject(this),
        "LightObjectの削除に失敗しました");
}


ForwardHDRRenderer::ForwardHDRRenderer()
{
    MyAssert(
        DefRendererMgr.AddForwardHDRRenderer(this),
        "ForwardHDRRendererの追加に失敗しました");
}

ForwardHDRRenderer::~ForwardHDRRenderer()
{

    MyAssert(
        DefRendererMgr.EraceForwardHDRRenderer(this),
        "ForwardHDRRendererの削除に失敗しました");
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


//描画
void RendererManager::Render()
{
    Matrix VP;
    VP = matView;
    VP *= matProjection;

    shader->SetValue("g_VP_mat", VP);

    m_DeferredLightManager.SetViewParam(matView, matProjection, Vector3Zero);


    GbufRenderer Gr;
    LightbufRenderer Lr;
    MasterRenderer Mr;
    ForwardRenderer Fr;

    Gr.SetMgr(this);
    Lr.SetMgr(this);
    Mr.SetMgr(this);
    Fr.SetMgr(this);

    m_DeferredLightManager.Render(
        &Gr,
        &Lr,
        &Mr,
        &Fr,
        &m_BlurEffectRenderer
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


void RendererManager::DepthRenderer::Render(iexShader* pShader, const char* technique)
{
    for (auto it = m_pMgr->m_DeferredRendererMap.begin();
        it != m_pMgr->m_DeferredRendererMap.end();
        ++it)
    {
        it->second->DepthRender(pShader, technique);
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

DeferredLightBufRenderer::IDepthRenderer* RendererManager::GetDepthRenderer()
{
    return &m_DepthRenderer;
}
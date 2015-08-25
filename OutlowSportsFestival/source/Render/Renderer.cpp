#include "Renderer.h"
#include "../debug/DebugFunction.h"
#include "../IexSystem/System.h"


RendererManager* RendererManager::m_pInstance = nullptr;

//*************************************************
//	�`��N���X�x�[�X
//*************************************************

DeferredRenderer::DeferredRenderer()
{
	MyAssert(
		DefRendererMgr.AddDeferredRenderer(this),
		"DeferredRenderer�̒ǉ��Ɏ��s���܂���");
}



DeferredRenderer::~DeferredRenderer()
{
	MyAssert(
		DefRendererMgr.EraceDeferredRenderer(this),
		"DeferredRenderer�̍폜�Ɏ��s���܂���");
}



ForwardRenderer::ForwardRenderer() :
m_SortZ(1000)
{
	MyAssert(
		DefRendererMgr.AddForwardRenderer(this),
		"ForwardRenderer�̒ǉ��Ɏ��s���܂���");
}

ForwardRenderer::~ForwardRenderer()
{
	MyAssert(
		DefRendererMgr.EraceForwardRenderer(this),
		"ForwardRenderer�̍폜�Ɏ��s���܂���");
}


LightObject::LightObject() :
Visible(true)
{

    MyAssert(
    DefRendererMgr.AddLightObject(this),
        "LightObject�̒ǉ��Ɏ��s���܂���");
}

LightObject::~LightObject()
{

    MyAssert(
        DefRendererMgr.EraceLightObject(this),
        "LightObject�̍폜�Ɏ��s���܂���");
}


ForwardHDRRenderer::ForwardHDRRenderer()
{
    MyAssert(
        DefRendererMgr.AddForwardHDRRenderer(this),
        "ForwardHDRRenderer�̒ǉ��Ɏ��s���܂���");
}

ForwardHDRRenderer::~ForwardHDRRenderer()
{

    MyAssert(
        DefRendererMgr.EraceForwardHDRRenderer(this),
        "ForwardHDRRenderer�̍폜�Ɏ��s���܂���");
}

//*************************************************
//	�`��}�l�[�W��
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


//�f�B�t�@�[�h�`��p�N���X�̒ǉ��E�폜
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


//�t�H���[�h�`��p�N���X�̒ǉ��E�폜
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


//���C�g�`��p�N���X�̒ǉ��E�폜
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


//�t�H���[�hHDR�`��p�N���X�̒ǉ��E�폜
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


//�`��
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
//   �����_���[
//---------------------------------------------------------------------

//���ʃN���X
void RendererManager::IRenderer::SetMgr(RendererManager* pMgr)
{
    m_pMgr = pMgr;
}


//G�o�b�t�@�`��
void RendererManager::GbufRenderer::Render(
    iexShader*        pShader, //�V�F�[�_�[�N���X
    DeferredGbufRenderer::TechniqueSetter*  pSetter  //�e�N�j�b�N�Ǘ��N���X
    )
{
	for (auto it = m_pMgr->m_DeferredRendererMap.begin();
        it != m_pMgr->m_DeferredRendererMap.end();
		++it)
	{
        it->second->GbufRender(pShader, pSetter);
	}
}


//���C�g�o�b�t�@�`��
void RendererManager::LightbufRenderer::Render(
    DeferredLightBufRenderer::LightRenderer* pLightRenderer //���C�g�`��N���X
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


//Z�l��r�p�֐�
static int CompareForwardHDRRendererFunc(const void*p1, const void* p2)
{
    return ((**(LpForwardRenderer*)p1).m_SortZ > (**(LpForwardRenderer*)p2).m_SortZ) ? (-1) : (1);
}


//�}�X�^�[�`��
void RendererManager::MasterRenderer::Render(
    iex2DObj* pInDiffuseTexture,   //���C�e�B���O������̊g�U���ˌ��e�N�X�`��
    iex2DObj* pInSpecularTexture,  //���C�e�B���O������̋��ʔ��ˌ��e�N�X�`��
    iex2DObj* pOutColorTexture,    //�F�����o�͂���e�N�X�`��
    iex2DObj* pOutHighRangeTexture //���P�x�������o�͂���e�N�X�`��
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

    //�t�H���[�hHDR�`������s����

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

            //Z�l���v�Z����
            it->second->CalcZ();
        }

        //�������ԂɃ\�[�g
        std::qsort(
            &SortData[0],
            count,
            sizeof(LpForwardHDRRenderer),
            CompareForwardHDRRendererFunc
            );

        //�`��
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

//Z�l��r�p�֐�
static int CompareForwardRendererFunc(const void*p1, const void* p2)
{
	return ((**(LpForwardRenderer*)p1).m_SortZ > (**(LpForwardRenderer*)p2).m_SortZ) ? (-1) : (1);
}

//�t�H���[�h�`��
void RendererManager::ForwardRenderer::Render()
{
	if (m_pMgr->m_ForwardRendererMap.empty())
	{
		return;
	}

	//�\�[�g���ʗp�z��𐶐�
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

			//Z�l���v�Z����
			it->second->CalcZ();
		}

		//�������ԂɃ\�[�g
		std::qsort(
			&SortData[0],
			count,
			sizeof(LpForwardRenderer),
            CompareForwardRendererFunc
			);

		//�`��
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
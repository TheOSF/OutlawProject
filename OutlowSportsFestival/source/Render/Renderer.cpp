#include "Renderer.h"
#include "../debug/DebugFunction.h"
#include "../IexSystem/System.h"


RendererManager* RendererManager::m_pInstance = nullptr;

//*************************************************
//	�`��N���X�x�[�X
//*************************************************

DeferredRenderer::DeferredRenderer()
{
#ifdef _DEBUG
	MyAssert(
		DefRendererMgr.AddDeferredRenderer(this),
		"DeferredRenderer�̒ǉ��Ɏ��s���܂���");
#else
    DefRendererMgr.AddDeferredRenderer(this);
#endif
}



DeferredRenderer::~DeferredRenderer()
{
#ifdef _DEBUG
	MyAssert(
		DefRendererMgr.EraceDeferredRenderer(this),
		"DeferredRenderer�̍폜�Ɏ��s���܂���");
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
		"ForwardRenderer�̒ǉ��Ɏ��s���܂���");
#else
    DefRendererMgr.AddForwardRenderer(this);
#endif
}

ForwardRenderer::~ForwardRenderer()
{
#ifdef _DEBUG
	MyAssert(
		DefRendererMgr.EraceForwardRenderer(this),
		"ForwardRenderer�̍폜�Ɏ��s���܂���");

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
        "UserInterfaceRenderer�̒ǉ��Ɏ��s���܂���");
#else
    DefRendererMgr.AddUIRenderer(this);
#endif
}

UserInterfaceRenderer::~UserInterfaceRenderer()
{
#ifdef _DEBUG
    MyAssert(
        DefRendererMgr.EraceUIRenderer(this),
        "UserInterfaceRenderer�̍폜�Ɏ��s���܂���");
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
        "LightObject�̒ǉ��Ɏ��s���܂���");

#else
    DefRendererMgr.AddLightObject(this);
#endif
}

LightObject::~LightObject()
{
#ifdef _DEBUG
    MyAssert(
        DefRendererMgr.EraceLightObject(this),
        "LightObject�̍폜�Ɏ��s���܂���");

#else
    DefRendererMgr.EraceLightObject(this);
#endif
}


ForwardHDRRenderer::ForwardHDRRenderer()
{
#ifdef _DEBUG
    MyAssert(
        DefRendererMgr.AddForwardHDRRenderer(this),
        "ForwardHDRRenderer�̒ǉ��Ɏ��s���܂���");

#else
    DefRendererMgr.AddForwardHDRRenderer(this);
#endif
}

ForwardHDRRenderer::~ForwardHDRRenderer()
{
#ifdef _DEBUG
    MyAssert(
        DefRendererMgr.EraceForwardHDRRenderer(this),
        "ForwardHDRRenderer�̍폜�Ɏ��s���܂���");
#else
    DefRendererMgr.EraceForwardHDRRenderer(this);
#endif
}


PostEffectRenderer::PostEffectRenderer()
{
#ifdef _DEBUG
    MyAssert(
        DefRendererMgr.AddPostEffectRenderer(this),
        "PostEffectRenderer�̒ǉ��Ɏ��s���܂���");
#else
    DefRendererMgr.AddPostEffectRenderer(this);
#endif
}

PostEffectRenderer::~PostEffectRenderer()
{
#ifdef _DEBUG
    MyAssert(
        DefRendererMgr.EracePostEffectRenderer(this),
        "PostEffectRenderer�̍폜�Ɏ��s���܂���");

#else
    DefRendererMgr.EracePostEffectRenderer(this);
#endif
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


//�t�H���[�h�`��p�N���X�̒ǉ��E�폜
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



//�u���[�|�X�g�G�t�F�N�g�`��N���X�̒ǉ��E�폜
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


//�`��
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

    //�u���[�p�����[�^�̍X�V
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


//Z�l��r�p�֐�
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

    //�\�[�g���ʗp�z��𐶐�
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

            //Z�l���v�Z����
            it->second->CalcZ();
        }

        //�������ԂɃ\�[�g
        std::qsort(
            &SortData[0],
            count,
            sizeof(LpUserInterfaceRenderer),
            CompareUIRendererFunc
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
    //�u���[���X�g�̍X�V
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
#include "Renderer.h"
#include "../debug/DebugFunction.h"

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


//�`��
void RendererManager::Render()
{

    //�X�N���[���T�[�t�F�C�X��ۑ�
    Surface* pScreen;
    iexSystem::Device->GetRenderTarget(0, &pScreen);

    //MRT�`��
    CreateGbuf();

    //�\�t�g�p�[�e�B�N���`��
    RenderSoftParticle();

    //HDR�������u���[����
    BlurTexture(m_pTextures[_HdrDepthTexture]);

    //HDR�������J���[�J���[�o�b�t�@�ɉ��Z
    RenderAddHDR();

    //�J���[�J���[�o�b�t�@�ɒʏ�`��
    ForwardRender();

    //�J���[�o�b�t�@����o�b�N�o�b�t�@�Ɉڂ�
    RenderToBackBuffer(pScreen);

    //�J���[�o�b�t�@�����Ƀo�b�N�o�b�t�@�Ƀ|�X�g�G�t�F�N�g�`��
    RenderPostEffect();

}

//�f�B�t�@�[�h�`��
void RendererManager::DeferredRender()
{

	for (auto it = m_DeferredRendererMap.begin();
		it != m_DeferredRendererMap.end();
		++it)
	{
		it->second->Render();
	}
}

//Z�l��r�p�֐�
static int CompareFunc(const void*p1, const void* p2)
{
	return ((**(LpForwardRenderer*)p1).m_SortZ > (**(LpForwardRenderer*)p2).m_SortZ) ? (-1) : (1);
}

//�t�H���[�h�`��
void RendererManager::ForwardRender()
{
	if (m_ForwardRendererMap.empty())
	{
		return;
	}

    //�����_�[�^�[�Q�b�g�ݒ�
    m_pTextures[_ColorTexture]->RenderTarget(0);

	//�\�[�g���ʗp�z��𐶐�
	LpForwardRenderer* SortData = new LpForwardRenderer[m_ForwardRendererMap.size()];

	try
	{
		int count = 0;

		for (auto it = m_ForwardRendererMap.begin();
			it != m_ForwardRendererMap.end();
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
			CompareFunc
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
        iexSystem::Device->SetRenderTarget(0, 0);
		throw;
	}

	delete[]SortData;

    iexSystem::Device->SetRenderTarget(0, 0);
}

//�J���[�o�b�t�@�̓��e�������̃T�[�t�F�C�X�Ɉڂ�
void RendererManager::RenderToBackBuffer(Surface* pSurface)
{

}

void RendererManager::CreateGbuf()
{
    //�e�N�X�`����RT�ɃZ�b�g
    m_pTextures[_ColorTexture]->RenderTarget(0);
    m_pTextures[_HdrDepthTexture]->RenderTarget(1);
    
    //�`��
    DeferredRender();

    //null��
    iexSystem::Device->SetRenderTarget(0, 0);
    iexSystem::Device->SetRenderTarget(1, 0);
}

//�\�t�g�p�[�e�B�N���`��
void RendererManager::RenderSoftParticle()
{

}

//�e�N�X�`�����u���[������
void RendererManager::BlurTexture(iex2DObj* pTex)
{
    


}

//HDR�������J���[�J���[�o�b�t�@�ɉ��Z
void RendererManager::RenderAddHDR()
{

    m_pTextures[_ColorTexture]->RenderTarget(0);

    m_pTextures[_WorkTexture]->Render(
        0,
        0,
        (int)iexSystem::ScreenWidth,
        (int)iexSystem::ScreenHeight,
        0,
        0,
        (int)iexSystem::ScreenWidth,
        (int)iexSystem::ScreenHeight,
        RS_ADD
        );

    iexSystem::Device->SetRenderTarget(0, 0);
}


//�|�X�g�G�t�F�N�g�`��
void RendererManager::RenderPostEffect()
{

}

RendererManager::RendererManager() :
m_WorkTextureSizeX((int)(iexSystem::ScreenWidth / 2)),
m_WorkTextureSizeY((int)(iexSystem::ScreenHeight / 2))
{
    for (int i = 0; i < (int)__MaxTexture; ++i)
    {
        m_pTextures[i] = new iex2DObj(
            (i != (int)_WorkTexture) ? (iexSystem::ScreenWidth) : (iexSystem::ScreenWidth / 2), 
            (i != (int)_WorkTexture) ? (iexSystem::ScreenHeight) : (iexSystem::ScreenHeight / 2),
            IEX2D_RENDERTARGET
            );
    }
}

RendererManager::~RendererManager()
{
    for (int i = 0; i < (int)__MaxTexture; ++i)
    {
        delete  m_pTextures[i];
        m_pTextures[i] = nullptr;
    }
}
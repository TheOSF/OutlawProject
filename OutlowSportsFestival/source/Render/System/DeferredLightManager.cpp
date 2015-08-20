#include "DeferredLightManager.h"
#include <string>

DeferredLightManager::DeferredLightManager(
    UINT ScreenX,
    UINT ScreenY,
    const char* pData_FilePath
    ):
    m_ScreenX(ScreenX),
    m_ScreenY(ScreenY)
{

    char work_path[MAX_PATH];

    //�J���[�o�͐�e�N�X�`���𐶐�
    m_pColorTexture = new iex2DObj(
        (DWORD)m_ScreenX,
        (DWORD)m_ScreenY,
        IEX2D_RENDERTARGET
        );


    //���P�x�o�͐�e�N�X�`���𐶐�
    m_pHRTexture = new iex2DObj(
        (DWORD)m_ScreenX,
        (DWORD)m_ScreenY,
        IEX2D_RENDERTARGET
        );


    //�p�[�e�B�N���e�N�X�`���ǂݍ���
    sprintf_s<MAX_PATH>(work_path, "%s%s", pData_FilePath, "\\particle.png");
    m_pParticleTexture = new iex2DObj(work_path);

    //�W�I���g���o�b�t�@�Ǘ��N���X����
    sprintf_s<MAX_PATH>(work_path, "%s%s", pData_FilePath, "\\DeferredGbuf.fx");
   
    m_pGbufRenderer = new DeferredGbufRenderer(
        m_ScreenX,
        m_ScreenY,
        work_path
        );

    //���C�g�o�b�t�@�Ǘ��N���X����
    m_pLightRenderer = new DeferredLightBufRenderer(
        m_ScreenX,
        m_ScreenY,
        pData_FilePath
        );

    //���C�g�o�b�t�@�Ǘ��N���X��G�o�b�t�@�e�N�X�`�����Z�b�g
    m_pLightRenderer->SetGbufTextures(
        &m_pGbufRenderer->NormalDepth,
        &m_pGbufRenderer->RoughnessLightMap
        );

    //HDR�`��Ǘ��N���X����
    sprintf_s<MAX_PATH>(work_path, "%s%s", pData_FilePath, "\\Blur.fx");
    m_pSmalBufBlurRenderer = new SmalBufBlurRenderer(
        8,
        work_path
        );

    m_pSmalBufBlurRenderer->SetSourceTexture(m_pHRTexture);
}


DeferredLightManager::~DeferredLightManager()
{
    delete m_pLightRenderer;
    delete m_pGbufRenderer;
    delete m_pColorTexture;
    delete m_pHRTexture;
    delete m_pParticleTexture;
    delete m_pSmalBufBlurRenderer;
}


//�r���[�֘A�̃p�����[�^��ݒ�
void DeferredLightManager::SetViewParam(
    const Matrix& View,
    const Matrix& Proj,
    const Vector3& CameraPos
    )
{
    Matrix VP_mat = View;
    VP_mat *= Proj;

    m_pGbufRenderer->UpdateView(VP_mat);
    m_pLightRenderer->UpdateViewParam(VP_mat, CameraPos);

}


void DeferredLightManager::Render(
    DeferredGbufRenderer::IRenderer*        pGbufRenderer,
    DeferredLightBufRenderer::IRenderer*    pLightRenderer,
    IMasterRenderer*                        pMasterRenderer,
    IForwardRenderer*                       pForwardRenderer,
    IPostEffectRenderer*                    pPostEffectRenderer
    )
{
    //�X�N���[���T�[�t�F�C�X��ۑ�
    Surface* pScreen;
    iexSystem::Device->GetRenderTarget(0, &pScreen);

    //�e�N�X�`��������
    ClearTexture();

    //�W�I���g���o�b�t�@�`��
    m_pGbufRenderer->Clear();
    m_pGbufRenderer->Render(pGbufRenderer);

    //�W�I���g���o�b�t�@�����Ƀ��C�g�o�b�t�@�`��
    m_pLightRenderer->Clear();
    m_pLightRenderer->Render(pLightRenderer);

    //���C�g�o�b�t�@�����Ƀ��C�e�B���O��̃e�N�X�`����`��
    pMasterRenderer->Render(
        m_pLightRenderer->m_pDiffuseTexture,
        m_pLightRenderer->m_pSpecularTexture,
        m_pColorTexture,
        m_pHRTexture
        );

    //�F�e�N�X�`���������_�[�^�[�Q�b�g�ɐݒ�
    m_pColorTexture->RenderTarget(0);
    for (DWORD i = 1; i < 4; ++i)
    {
        iexSystem::Device->SetRenderTarget(1, 0);
    }

    //�t�H���[�h�����_�����O�����s
    pForwardRenderer->Render();


    //���P�x�������u���[�������A�J���[�e�N�X�`���ɏo��
    m_pSmalBufBlurRenderer->Render();

    //�|�X�g�G�t�F�N�g�������A�o�b�N�o�b�t�@�ɏo��
    pPostEffectRenderer->Render(m_pColorTexture, pScreen);

}



//�f�o�b�O�p�e�N�X�`���`��
void DeferredLightManager::TextureRender()
{
  
    LPIEX2DOBJ tex[] =
    {
        &m_pGbufRenderer->NormalDepth,
        &m_pGbufRenderer->RoughnessLightMap,

        m_pLightRenderer->m_pDiffuseTexture,
        m_pLightRenderer->m_pSpecularTexture,

        m_pLightRenderer->m_pShadowDepthTexture,

        m_pHRTexture
    };

    const int sizeX = (int)m_ScreenX / 6;
    const int sizeY = (int)m_ScreenY / 6;
    int x = 0;
    int y = 0;

    for (int i = 0; i < ARRAYSIZE(tex); ++i)
    {
        if (i % 3 == 0 && i>0)
        {
            x = 0;
            y += sizeY;
        }

        //�`��
        tex[i]->Render(
            x, y, sizeX, sizeY,
            0, 0,
            1280,
            720
            );

        x += sizeX;
    }
}


//�e�N�X�`���N���A
void DeferredLightManager::ClearTexture()
{
    //�F�e�N�X�`���������_�[�^�[�Q�b�g�ɐݒ�

    m_pColorTexture->RenderTarget(0);

    iexSystem::Device->Clear(0, 0, D3DCLEAR_TARGET, 0, 1, 0);

    m_pHRTexture->RenderTarget(0);

    iexSystem::Device->Clear(0, 0, D3DCLEAR_TARGET, 0, 1, 0);

}
#ifndef __DEFERRED_MANAGER_H__
#define __DEFERRED_MANAGER_H__

#include "DeferredGbufRenderer.h"
#include "DeferredLightBufRenderer.h"
#include "SmalBufBlurRenderer.h"


//--------------------------------------------------
//  �f�B�t�@�[�h�`����Ǘ�����N���X
//--------------------------------------------------

class DeferredLightManager
{
public:
    //�t�H���[�h�`��N���X
    class IForwardRenderer
    {
    public:
        virtual ~IForwardRenderer(){}
        virtual void Render() = 0;
    };

    //���C�g���ʂ�����MRT�`����s���N���X(RT0=�F,RT1=���P�x)
    class IMasterRenderer
    {
    public:
        virtual ~IMasterRenderer(){}
        virtual void Render(
            iex2DObj* pInDiffuseTexture,   //���C�e�B���O������̊g�U���ˌ��e�N�X�`��
            iex2DObj* pInSpecularTexture,  //���C�e�B���O������̋��ʔ��ˌ��e�N�X�`��
            iex2DObj* pOutColorTexture,    //�F�����o�͂���e�N�X�`��
            iex2DObj* pOutHighRangeTexture //���P�x�������o�͂���e�N�X�`��
            ) = 0;
    };

    //�|�X�g�G�t�F�N�g�������s���C���^�[�t�F�C�X�N���X
    class IPostEffectRenderer
    {
    public:
        virtual ~IPostEffectRenderer(){}
        virtual void Render(
            iex2DObj* pIn,
            Surface*  pOut
            ) = 0;
    };

    DeferredLightManager(
        UINT ScreenX,              //�X�N���[����
        UINT ScreenY,              //�X�N���[����
        const char* pData_FilePath //�V�F�[�_�[�t�H���_�ւ̃p�X
        );

    ~DeferredLightManager();

    //�r���[�֘A�̃p�����[�^��ݒ�
    void SetViewParam(
        const Matrix& View,
        const Matrix& Proj,
        const Vector3& CameraPos
        );

    //�`��
    void Render(
        DeferredGbufRenderer::IRenderer*        pGbufRenderer,
        DeferredLightBufRenderer::IRenderer*    pLightRenderer,
        IMasterRenderer*                        pMasterRenderer,
        IForwardRenderer*                       pForwardRenderer,
        IPostEffectRenderer*                    pPostEffectRenderer,
        IForwardRenderer*                       pUIRenderer
        );

    //�f�o�b�O�p�e�N�X�`���`��
    void TextureRender();

    iex2DObj* GetNormalDepthTexture();

private:

    DeferredLightManager(const DeferredLightManager&);

    DeferredGbufRenderer*       m_pGbufRenderer;
    DeferredLightBufRenderer*   m_pLightRenderer;
    SmalBufBlurRenderer*        m_pSmalBufBlurRenderer;

    const UINT                  m_ScreenX;
    const UINT                  m_ScreenY;

    iex2DObj*                   m_pColorTexture;
    iex2DObj*                   m_pHRTexture;
 

    //�e�N�X�`���N���A
    void ClearTexture();
};

#endif
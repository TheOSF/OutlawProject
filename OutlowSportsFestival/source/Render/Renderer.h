#ifndef __RENDERER_H__
#define __RENDERER_H__

//*************************************************
//	�`��N���X�x�[�X
//*************************************************
#include <map>
#include "System\DeferredLightManager.h"
#include "System\BlurEffectRenderer.h"
#include "../IexSystem/System.h"

//�f�B�t�@�[�h�`��p�N���X
class DeferredRenderer
{
public:
	//�R���X�g���N�^�E�f�X�g���N�^�Ŏ����I�ɕ`������邽�߂̓o�^������
	DeferredRenderer();
	virtual ~DeferredRenderer();

	//G�o�b�t�@�`��(�����I�ɌĂ΂��)
	virtual void GbufRender(
        iexShader*        pShader,                       //�V�F�[�_�[�N���X
        DeferredGbufRenderer::TechniqueSetter*  pSetter  //�e�N�j�b�N�Ǘ��N���X
        ) = 0;

    //�{�ԕ`��(�����I�ɌĂ΂��ART�O���F�@RT1�����P�x)
    virtual void MasterRender() = 0;

    virtual void DepthRender(iexShader* pShader, const char* pTec) = 0;
};

typedef DeferredRenderer* LpDeferredRenderer;

//�t�H���[�h�`��p�N���X(�����I�u�W�F�N�g�p)
class ForwardRenderer
{
public:
	//�R���X�g���N�^�E�f�X�g���N�^�Ŏ����I�ɕ`������邽�߂̓o�^������
	ForwardRenderer();
	virtual ~ForwardRenderer();
	
	//���̕ϐ������Ƃɒl���Ⴂ���Ԃ�Sort����
	float m_SortZ;
	
	//Z�l�v�Z�����̊֐��Ŏ��s���邱�Ƃ��ł���
	virtual void CalcZ() = 0;

	//�`��(�����I�ɌĂ΂��)
	virtual void Render() = 0;
};
typedef ForwardRenderer* LpForwardRenderer;


//���C�g�`��N���X
class LightObject
{
public:
    typedef DeferredLightBufRenderer::LightRenderer LightRenderer;

    bool Visible;   //�\���t���O
    
    LightObject();
    virtual~LightObject();
    
    virtual void Render(LightRenderer* pLightRenderer) = 0;
};
typedef LightObject* LpLightObject;


//�f�B�t�@�[�h���HDR�`��N���X
class ForwardHDRRenderer
{
public:
    ForwardHDRRenderer();
    virtual ~ForwardHDRRenderer();

    //���̕ϐ������Ƃɒl���Ⴂ���Ԃ�Sort����
	float m_SortZ;
	
	//Z�l�v�Z�����̊֐��Ŏ��s���邱�Ƃ��ł���
	virtual void CalcZ() = 0;

	//�`��(�����I�ɌĂ΂��)
	virtual void Render() = 0;
};
typedef ForwardHDRRenderer* LpForwardHDRRenderer;

//*************************************************
//	�`��}�l�[�W��
//*************************************************
class RendererManager
{
public:
	static RendererManager& GetInstance();
	static void Release();

	//�f�B�t�@�[�h�`��p�N���X�̒ǉ��E�폜
	bool AddDeferredRenderer(LpDeferredRenderer pDef);
	bool EraceDeferredRenderer(LpDeferredRenderer pDef);

	//�t�H���[�h�`��p�N���X�̒ǉ��E�폜
	bool AddForwardRenderer(LpForwardRenderer pFor);
	bool EraceForwardRenderer(LpForwardRenderer pFor);

    //���C�g�`��p�N���X�̒ǉ��E�폜
    bool AddLightObject(LpLightObject pL);
    bool EraceLightObject(LpLightObject pL);

    //�t�H���[�hHDR�`��p�N���X�̒ǉ��E�폜
    bool AddForwardHDRRenderer(LpForwardHDRRenderer pForHDR);
    bool EraceForwardHDRRenderer(LpForwardHDRRenderer pForHDR);

    //�`��
    void Render();

    //Z�l�`��I�u�W�F�N�g�ւ̃|�C���^�𓾂�
    DeferredLightBufRenderer::IDepthRenderer* GetDepthRenderer();
private:

	RendererManager();
	~RendererManager();

	typedef std::map<LpDeferredRenderer, LpDeferredRenderer> DeferredRendererMap;
    typedef std::map<LpForwardHDRRenderer, LpForwardHDRRenderer> ForwardHDRRendererMap;
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
            iexShader*        pShader, //�V�F�[�_�[�N���X
            DeferredGbufRenderer::TechniqueSetter*  pSetter  //�e�N�j�b�N�Ǘ��N���X
            );
    };

    class LightbufRenderer :public DeferredLightBufRenderer::IRenderer, public IRenderer
    {
    public:
        void Render(
            DeferredLightBufRenderer::LightRenderer* pLightRenderer //���C�g�`��N���X
            );
    };

    class MasterRenderer :public DeferredLightManager::IMasterRenderer, public IRenderer
    {
    public:
        void Render(
            iex2DObj* pInDiffuseTexture,   //���C�e�B���O������̊g�U���ˌ��e�N�X�`��
            iex2DObj* pInSpecularTexture,  //���C�e�B���O������̋��ʔ��ˌ��e�N�X�`��
            iex2DObj* pOutColorTexture,    //�F�����o�͂���e�N�X�`��
            iex2DObj* pOutHighRangeTexture //���P�x�������o�͂���e�N�X�`��
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


	DeferredRendererMap      m_DeferredRendererMap;
    ForwardHDRRendererMap    m_ForwardHDRRendererMap;
	ForwardRendererMap       m_ForwardRendererMap;
    LightObjectMap           m_LightObjectMap;

    DeferredLightManager  m_DeferredLightManager;

    BlurEffectRenderer    m_BlurEffectRenderer;
    DepthRenderer         m_DepthRenderer;
};

#define DefRendererMgr (RendererManager::GetInstance())

#endif
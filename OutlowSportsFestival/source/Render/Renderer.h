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
    enum class DepthRenderType
    {
        DirLight,
        SpotLight,
    };

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

    virtual void DepthRender(iexShader* pShader, const char* pTec, DepthRenderType type) = 0;
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



//UI�`��p�N���X
class UserInterfaceRenderer
{
public:
    //�R���X�g���N�^�E�f�X�g���N�^�Ŏ����I�ɕ`������邽�߂̓o�^������
    UserInterfaceRenderer();
    virtual ~UserInterfaceRenderer();

    //���̕ϐ������Ƃɒl���Ⴂ���Ԃ�Sort����
    float m_SortZ;

    //Z�l�v�Z�����̊֐��Ŏ��s���邱�Ƃ��ł���
    virtual void CalcZ() = 0;

    //�`��(�����I�ɌĂ΂��)
    virtual void Render() = 0;
};
typedef UserInterfaceRenderer* LpUserInterfaceRenderer;



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


//�u���[�|�X�g�G�t�F�N�g�`��N���X
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
//	�`��}�l�[�W��
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

    //�[�x�`��N���X
    class DepthRenderer :public DeferredLightBufRenderer::IDepthRenderer, public IRenderer
    {
    public:
        DeferredRenderer::DepthRenderType m_Type;

        DepthRenderer();

        void Render(iexShader* pShader, const char* technique);
    };

	static RendererManager& GetInstance();
	static void Release();

	//�f�B�t�@�[�h�`��p�N���X�̒ǉ��E�폜
	bool AddDeferredRenderer(LpDeferredRenderer pDef);
	bool EraceDeferredRenderer(LpDeferredRenderer pDef);

	//�t�H���[�h�`��p�N���X�̒ǉ��E�폜
	bool AddForwardRenderer(LpForwardRenderer pFor);
	bool EraceForwardRenderer(LpForwardRenderer pFor);

    //UI�`��p�N���X�̒ǉ��E�폜
    bool AddUIRenderer(LpUserInterfaceRenderer p);
    bool EraceUIRenderer(LpUserInterfaceRenderer p);

    //���C�g�`��p�N���X�̒ǉ��E�폜
    bool AddLightObject(LpLightObject pL);
    bool EraceLightObject(LpLightObject pL);

    //�t�H���[�hHDR�`��p�N���X�̒ǉ��E�폜
    bool AddForwardHDRRenderer(LpForwardHDRRenderer pForHDR);
    bool EraceForwardHDRRenderer(LpForwardHDRRenderer pForHDR);

    //�u���[�|�X�g�G�t�F�N�g�`��N���X�̒ǉ��E�폜
    bool AddPostEffectRenderer(LpPostEffectRenderer p);
    bool EracePostEffectRenderer(LpPostEffectRenderer p);


    //�`��
    void Render();

    //Z�l�`��I�u�W�F�N�g�ւ̃|�C���^�𓾂�
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
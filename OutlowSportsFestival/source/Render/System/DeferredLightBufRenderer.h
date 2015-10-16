#ifndef __DEFERRED_LIGHT_BUF_RENDERER_H__
#define __DEFERRED_LIGHT_BUF_RENDERER_H__

#include "iextreme.h"
//-----------------------------------------------------------
//  G�o�b�t�@�����Ƀ��C�e�B���O�e�N�X�`�����쐬����N���X
//-----------------------------------------------------------

class DeferredLightBufRenderer
{
public:

    class IDepthRenderer
    {
    public:
        virtual ~IDepthRenderer(){}
        virtual void Render(iexShader* pShader, const char* technique) = 0;
    };

    //���s���p�����[�^�[
    struct DirLightParam
    {
        enum class Type
        {
            Ortho,
            Perspective
        };

        Vector3 vec;
        Vector3 color;

        struct
        {
            
            bool              visible;
            IDepthRenderer*   pDepthRenderer;
            Vector3           origin;
            float             Near;
            float             Far;
            Type              type;
            float             Size;
        }
        Shadow;
    };

    //�_�����p�����[�^�[
    struct PointLightParam
    {
        Vector3 pos;
        float   size;
        Vector3 color;
    };

    //�X�|�b�g���C�g
    struct SpotLightParam
    {
        Vector3 origin;
        Vector3 target;
        Vector3 color;
        float   size;

        struct
        {
            bool              visible;
            IDepthRenderer*   pDepthRenderer;
        }
        Shadow;
    };

    //�������C�e�B���O
    struct HemiLightParam
    {
        Vector3 Up;
        Vector3 SkyColor;
        Vector3 GroundColor;
    };

    //�����C�e�B���O
    struct AmbientParam
    {
        Vector3 color;
        
        struct
        {
            bool  Enable;
            float SamplingSize;
            float CalcDiffZ;
        }
        Occlusion;
    };

    //���C�g�`��N���X
    class LightRenderer
    {
    public:
        LightRenderer(DeferredLightBufRenderer* pMgr);
        
        void DirLight(DirLightParam& param);     //�f�B���N�V���i�����C�g�`��
        void PointLight(PointLightParam& param); //�|�C���g���C�g�`��
        void SpotLight(SpotLightParam& param);   //�X�|�b�g���C�g�`��
        void HemiLight(HemiLightParam& param);   //�������C�g�`��
        void AmbientLight(AmbientParam& param);  //�����`��
    private:
        void DirLightUseVarianceShadow(DirLightParam& param);
        void DirLightUseShadow(DirLightParam& param);
        void SpotLightUseShadow(SpotLightParam& param);

        DeferredLightBufRenderer* m_pMgr;
    };

    //���C�g�`��Ăяo�����s���C���^�[�t�F�[�X
    class IRenderer
    {
    public:
        virtual ~IRenderer(){}
        virtual void Render(
            LightRenderer* pLightRenderer //���C�g�`��N���X
            ) = 0;
    };



    DeferredLightBufRenderer(
        UINT X,                     //�X�N���[����
        UINT Y,                     //�X�N���[������
        const char* pData_FilePath  //���C�g�p FX�EMESH�t�@�C���ւ̃p�X
        );

    ~DeferredLightBufRenderer();
    
    //���C�g�o�b�t�@�N���A
    void Clear();

    //���C�g�o�b�t�@�ɕ`����s
    void Render(IRenderer* pRenderer);

    //�v�Z�̌��ƂȂ�G�o�b�t�@�e�N�X�`�����Z�b�g
    void SetGbufTextures(
        iex2DObj* NormalDepth,           //�@���E�[�x
        iex2DObj* RoughnessLightMap      //�e���E���C�g�}�b�v�l
    );

    //�r���[�֘A�̍s���ݒ�E�X�V����
    void UpdateViewParam(
        const Matrix& VP_mat,     //�r���[�v���W�F�N�V�����s��
        const Vector3& CameraPos
        );

    iex2DObj*   m_pDiffuseTexture;           //�f�B�t���[�Y�e�N�X�`��
    iex2DObj*   m_pSpecularTexture;          //�X�y�L�����e�N�X�`��
    iex2DObj*   m_pShadowDepthTexture;       //�e�pZ�l�e�N�X�`��
private:

    const UINT     m_X, m_Y;              //�X�N���[���T�C�YXY
    Matrix         m_ViewMatrix;          //�r���[�s��ۊ�
    Matrix         m_ViewInvMatrix;       //�r���[�t�s��ۊ�
    iexShader*     m_pShader;             //�V�F�[�_�[�N���X
    LPIEXMESH      m_pShpereMesh;         //�_�����p�����b�V��
    LPIEXMESH      m_pSpotMesh;           //�~�������p�����b�V��
                   
    Surface*       m_pDepthStencil;       //�e�p�f�v�X�X�e���V���o�b�t�@
    D3DVIEWPORT9   m_ShadowViewport;     //�e�p�r���[�|�[�g
  
};

#endif
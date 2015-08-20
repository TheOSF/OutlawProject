#ifndef __DEFERRED_GBUF_RENDERER_H__
#define __DEFERRED_GBUF_RENDERER_H__

#include "iextreme.h"

//-----------------------------------------------------------
//   G�o�b�t�@�̐����E�Ǘ����s���N���X
//-----------------------------------------------------------

class DeferredGbufRenderer
{
public:

    //�`��e�N�j�b�N���Z�b�g����N���X
    class TechniqueSetter
    {
    public:
        TechniqueSetter(DeferredGbufRenderer* pMgr);
        ~TechniqueSetter();

        //�e�N�X�`�����g�p���Ȃ��e�N�j�b�N�𓾂�
        void NoTexture(
            char* out,
            size_t out_size
            );

        //�@���@�e�N�X�`�����g�p����e�N�j�b�N�𓾂�
        void UseNormal(
            char* out,
            size_t out_size
            );

        //�@���@�����@�e�N�X�`�����g�p����e�N�j�b�N�𓾂�
        void UseNormalHeight(
            char* out,
            size_t out_size
            );
    private:
        DeferredGbufRenderer*  m_pMgr;
    };

    //�W�I���g���������s���N���X
    class IRenderer
    {
    public:
        virtual ~IRenderer(){}

        //�`��
        virtual void Render(
            iexShader*        pShader, //�V�F�[�_�[�N���X
            TechniqueSetter*  pSetter  //�e�N�j�b�N�Ǘ��N���X
            ) = 0;
    };

    
    DeferredGbufRenderer(
        UINT  X,                   //�X�N���[���T�C�YX
        UINT  Y,                   //�X�N���[���T�C�YY
        const char* shader_file    //�V�F�[�_�[�t�@�C���ւ̃p�X
        );

    ~DeferredGbufRenderer();

    //G�o�b�t�@���N���A
    void Clear();

    //G�o�b�t�@�`��
    void Render(IRenderer* pRenderer);

    //�r���[�֘A�̃p�����[�^�̐ݒ�E�X�V
    void UpdateView(
        const Matrix& VP_mat  //�r���[�v���W�F�N�V�����s��
        );

    iex2DObj NormalDepth;                   //�@��xy �v���W�F�N�V����zw
    iex2DObj RoughnessLightMap;             //�e��r ���C�g�}�b�vg
private:

    iexShader*         m_pShader;     //�V�F�[�_�[�N���X
    const UINT         m_X;           //�X�N���[���T�C�YX
    const UINT         m_Y;           //�X�N���[���T�C�YY

    DeferredGbufRenderer(const DeferredGbufRenderer&);

    void SetRenderTarget();
};

#endif
#include "DeferredGbufRenderer.h"


//---------------------------------------------
//  DeferredGbufRenderer::TechniqueSetter
//---------------------------------------------

DeferredGbufRenderer::TechniqueSetter::TechniqueSetter(DeferredGbufRenderer* pMgr) :
m_pMgr(pMgr)
{

}

DeferredGbufRenderer::TechniqueSetter::~TechniqueSetter()
{

}

//�e�N�X�`�����g�p���Ȃ��e�N�j�b�N�𓾂�
void DeferredGbufRenderer::TechniqueSetter::NoTexture(
    char*  out,
    size_t out_size)
{
    static const char* Technique = "NoTexture";

    //���ʊi�[����e�ʃ`�F�b�N
    assert(strlen(Technique) + 1 < out_size && "�i�[������̃T�C�Y������܂���");

    //�߂�l�Ƃ��Ċi�[
    strcpy(out, Technique);
}

//�@���@�e�N�X�`�����g�p����e�N�j�b�N�𓾂�
void DeferredGbufRenderer::TechniqueSetter::UseNormal(
    char*  out,
    size_t out_size)
{
    static const char* Technique = "UseNormal";

    //���ʊi�[����e�ʃ`�F�b�N
    assert(strlen(Technique) + 1 < out_size && "�i�[������̃T�C�Y������܂���");


    //�߂�l�Ƃ��Ċi�[
    strcpy(out, Technique);
}

//�@���@�����@�e�N�X�`�����g�p����e�N�j�b�N�𓾂�
void DeferredGbufRenderer::TechniqueSetter::UseNormalHeight(
    char*  out,
    size_t out_size)
{
    static const char* Technique = "UseNormalHeight";

    //���ʊi�[����e�ʃ`�F�b�N
    assert(strlen(Technique) + 1 < out_size && "�i�[������̃T�C�Y������܂���");

    //�߂�l�Ƃ��Ċi�[
    strcpy(out, Technique);
}

//---------------------------------------------
//  public 
//---------------------------------------------

DeferredGbufRenderer::DeferredGbufRenderer(
    UINT  X,
    UINT  Y,
    const char* shader_file
    ) :
    m_X(X),
    m_Y(Y),
    NormalDepth         ((ULONG)X, (ULONG)Y, IEX2D_USEALPHA2),
    RoughnessLightMap   ((ULONG)X, (ULONG)Y, IEX2D_RENDERTARGET)
{
    //�V�F�[�_�[�N���X�쐬
    char temp_fileName[MAX_PATH];
    strcpy_s<MAX_PATH>(temp_fileName, shader_file);

    m_pShader = new iexShader(temp_fileName);

}

DeferredGbufRenderer::~DeferredGbufRenderer()
{
    delete m_pShader;
}

//G�o�b�t�@���N���A
void DeferredGbufRenderer::Clear()
{
    //�X�N���[���̃T�[�t�F�C�X�|�C���^��ۑ�
    Surface*  pScreen;
    iexSystem::Device->GetRenderTarget(0, &pScreen);

    //�����_�[�^�[�Q�b�g�ݒ�
    SetRenderTarget();

    //�`��
    iexPolygon::RectPlus(
        0,
        0,
        (int)m_X,
        (int)m_Y,
        m_pShader,
        "Clear",
        0xFFFFFFFF
        );


    //���ɖ߂�
    iexSystem::Device->SetRenderTarget(0, pScreen);
    iexSystem::Device->SetRenderTarget(1, 0);

    pScreen->Release();
}

//G�o�b�t�@�`��
void DeferredGbufRenderer::Render(IRenderer* pRenderer)
{
    TechniqueSetter setter(this);

    //�����_�[�^�[�Q�b�g�ݒ�
    SetRenderTarget();

    //�`��
    pRenderer->Render(m_pShader, &setter);
   
    //���ɖ߂�
    iexSystem::Device->SetRenderTarget(1, 0);
}

//�r���[�֘A�̃p�����[�^�̐ݒ�E�X�V
void DeferredGbufRenderer::UpdateView(
    const Matrix& VP_mat  //�r���[�s��
    )
{
    Matrix m = VP_mat;
    m_pShader->SetValue("g_VP_mat", m);
    m = matView;
    m_pShader->SetValue("g_V_mat", m);
}

//---------------------------------------------
//  private
//---------------------------------------------

void DeferredGbufRenderer::SetRenderTarget()
{
    //�����_�[�^�[�Q�b�g�ݒ�
    NormalDepth.RenderTarget(0);
    RoughnessLightMap.RenderTarget(1);
}
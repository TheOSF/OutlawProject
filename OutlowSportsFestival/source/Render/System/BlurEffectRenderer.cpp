#include "BlurEffectRenderer.h"

BlurEffectRenderer::BlurEffectRenderer(const char* pDataFile)
{
    char path[MAX_PATH];

    //�����b�V���ǂݍ���
    sprintf(path, "%s%s", pDataFile, "\\sphere.IMO");
    m_pSphereMesh = new iexMesh(path);
    D3DXMatrixIdentity(&m_pSphereMesh->TransMatrix);


    //�R�[�����b�V���ǂݍ���
    sprintf(path, "%s%s", pDataFile, "\\spot.IMO");
    m_pConeMesh = new iexMesh(path);
    D3DXMatrixIdentity(&m_pConeMesh->TransMatrix);
    

    //�V�F�[�_�[�t�@�C���ǂݍ���
    sprintf(path, "%s%s", pDataFile, "\\BlurEffect.fx");
    m_pShader = new iexShader(path);

    //�s�N�Z���T�C�Y�ݒ�
    m_pShader->SetValue("g_offset_X", 1 / (float)(iexSystem::ScreenWidth));
    m_pShader->SetValue("g_offset_Y", 1 / (float)(iexSystem::ScreenHeight));

}

BlurEffectRenderer::~BlurEffectRenderer()
{
    delete m_pConeMesh;
    delete m_pSphereMesh;
    delete m_pShader;
}

void BlurEffectRenderer::Render(
    iex2DObj* pIn,
    Surface*  pOut
    )
{
    //���ʂ������_�[�^�[�Q�b�g�ɐݒ�
    iexSystem::Device->SetRenderTarget(0, pOut);
    for (int i = 1; i < 4; ++i)
    {
        iexSystem::Device->SetRenderTarget(i, 0);
    }

    //���摜��`��
    pIn->Render();

    Matrix VP_mat = matView*matProjection;

    //�`��p�����[�^�ݒ�
    m_pShader->SetValue("SourceTexture", pIn->GetTexture());
    m_pShader->SetValue("g_VP_mat", VP_mat);

    RenderConeBlur(VP_mat);

    RenderSphereBlur(VP_mat);
  
}


void BlurEffectRenderer::RenderSphereBlur(const Matrix& VP_mat)
{

    //���u���[��`��
    Matrix& m = m_pSphereMesh->TransMatrix;
    Vector4 tex_pos, temp;
    float Size;

    for (auto& it : m_BlurSphere)
    {
        //�X�N���[���e�N�X�`�����W���Z�o
        tex_pos.x = it.pos.x;
        tex_pos.y = it.pos.y;
        tex_pos.z = it.pos.z;
        tex_pos.w = 1;


        D3DXVec4Transform(&tex_pos, &tex_pos, &VP_mat);
        tex_pos.x = tex_pos.x / tex_pos.w*0.5f + 0.5f;
        tex_pos.y = tex_pos.y / tex_pos.w*-0.5f + 0.5f;


        //�X�N���[���e�N�X�`����ł̑傫�����Z�o
        temp.x = it.pos.x + it.scale;
        temp.y = it.pos.y;
        temp.z = it.pos.z;
        temp.w = 1;

        D3DXVec4Transform(&temp, &temp, &VP_mat);
        temp.x = temp.x / temp.w*0.5f + 0.5f;
        temp.y = temp.y / temp.w*-0.5f + 0.5f;

        temp.x -= tex_pos.x;
        temp.y -= tex_pos.y;

        Size = sqrtf(temp.x*temp.x + temp.y*temp.y);

        //�p�����[�^�ݒ�

        m_pShader->SetValue(
            "g_center_pos_x",
            tex_pos.x
            );

        m_pShader->SetValue(
            "g_center_pos_y",
            tex_pos.y
            );

        m_pShader->SetValue(
            "g_blur_power",
            it.power
            );

        m_pShader->SetValue(
            "g_blur_size",
            Size
            );

        m._41 = it.pos.x;
        m._42 = it.pos.y;
        m._43 = it.pos.z;

        m._11 = m._22 = m._33 = it.scale;

        //�`��
        m_pSphereMesh->Render(m_pShader, "radiation_blur");
    }



}



void BlurEffectRenderer::RenderConeBlur(const Matrix& VP_mat)
{

}
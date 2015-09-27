#include "DeferredLightBufRenderer.h"

//---------------------------------------------
//   DeferredLightBufRenderer::LightRenderer
//---------------------------------------------

DeferredLightBufRenderer::LightRenderer::LightRenderer(DeferredLightBufRenderer* pMgr) :
m_pMgr(pMgr)
{

}


void DeferredLightBufRenderer::LightRenderer::DirLight(DirLightParam& param)
{

    m_pMgr->m_pShader->SetValue("g_LightVec", Vector3MulMatrix3x3(param.vec, m_pMgr->m_ViewMatrix));
    m_pMgr->m_pShader->SetValue("g_LightColor", param.color);


    if (param.Shadow.visible)
    {
        DirLightUseVarianceShadow(param);
        return;
    }

    //���s���`��
    iexPolygon::RectPlus(
        0,
        0,
        (int)m_pMgr->m_X,
        (int)m_pMgr->m_Y,
        m_pMgr->m_pShader,
        "DirLight",
        0xFFFFFFFF
        );

}

void DeferredLightBufRenderer::LightRenderer::DirLightUseShadow(DirLightParam& param)
{
    Matrix View;
    Matrix Proj;

    //�s��v�Z
    switch (param.Shadow.type)
    {
    case DirLightParam::Type::Ortho:

        //�r���[�s��v�Z
        D3DXMatrixLookAtLH(
            &View,
            (D3DXVECTOR3*)&param.Shadow.origin,
            (D3DXVECTOR3*)&(param.Shadow.origin + param.vec),
            &D3DXVECTOR3(0, 1, 0)
            );

        //�v���W�F�N�V�����s��v�Z
        D3DXMatrixOrthoLH(&Proj, param.Shadow.Size, param.Shadow.Size, param.Shadow.Near, param.Shadow.Far);
        View *= Proj;
        break;

    case DirLightParam::Type::Perspective:
        Proj = matView * matProjection;
        D3DXMatrixInverse(&View, 0, &matView);
        Proj *= View;

        //�r���[�s��v�Z
        D3DXMatrixLookAtLH(
            &View,
            (D3DXVECTOR3*)&param.Shadow.origin,
            (D3DXVECTOR3*)&(param.Shadow.origin + param.vec),
            &D3DXVECTOR3(0, 1, 0)
            );

        Proj *= View;

        D3DXMatrixPerspectiveFovLH(&View, param.Shadow.Size, 1, param.Shadow.Near, param.Shadow.Far);
      //  D3DXMatrixOrthoLH(&Proj, param.Shadow.Size, param.Shadow.Size, param.Shadow.Near, param.Shadow.Far);
        Proj *= View;
        break;

    default:
        assert(0 && "���s���̉e�^�C�v���ݒ肳��Ă��܂���");
        return;
    }

    //�r���[�v���W�F�N�V�����s����Z�o���A�G�t�F�N�g�ɃZ�b�g
    
    m_pMgr->m_pShader->SetValue("g_Shadow_VP_mat", View);


    //���݂̃r���[�|�[�g��ۑ����A�e�p�̃r���[�|�[�g���Z�b�g
    D3DVIEWPORT9 SaveViewport;
    iexSystem::Device->GetViewport(&SaveViewport);

    iexSystem::Device->SetViewport(&m_pMgr->m_ShadowViewport);

    //���݂�RT��ۑ����A�e�p�e�N�X�`����RT��
    Surface* pDiffuseTex, *pSpecularTex;
    iexSystem::Device->GetRenderTarget(0, &pDiffuseTex);
    iexSystem::Device->GetRenderTarget(1, &pSpecularTex);

    m_pMgr->m_pShadowDepthTexture->RenderTarget(0);
    iexSystem::Device->SetRenderTarget(1, 0);

    //���݂̃f�v�X�X�e���V���o�b�t�@��ۑ����e�p�̂��̂�
    Surface* pSaveDepthStencil;
    iexSystem::Device->GetDepthStencilSurface(&pSaveDepthStencil);

    iexSystem::Device->SetDepthStencilSurface(m_pMgr->m_pDepthStencil);

    //�N���A
    iexPolygon::RectPlus(
        0,
        0,
        m_pMgr->m_ShadowViewport.Width,
        m_pMgr->m_ShadowViewport.Height,
        m_pMgr->m_pShader,
        "ClearZ",
        0xFFFFFFFF);

    iexSystem::Device->Clear(
        0,
        0,
        D3DCLEAR_ZBUFFER,
        0,
        1,
        0
        );

    //Z�l��`��
    param.Shadow.pDepthRenderer->Render(
        m_pMgr->m_pShader,
        "WriteZ"
        );

    //�r���[�|�[�g�����ɖ߂�
    iexSystem::Device->SetViewport(&SaveViewport);

    //RT�����Ƃɖ߂�
    iexSystem::Device->SetRenderTarget(0, pDiffuseTex);
    iexSystem::Device->SetRenderTarget(1, pSpecularTex);

    pDiffuseTex->Release();
    pSpecularTex->Release();

    //�f�v�X�X�e���V���o�b�t�@�����Ƃɖ߂�
    iexSystem::Device->SetDepthStencilSurface(pSaveDepthStencil);
    pSaveDepthStencil->Release();

    //���s���`��
    iexPolygon::RectPlus(
        0,
        0,
        (int)m_pMgr->m_X,
        (int)m_pMgr->m_Y,
        m_pMgr->m_pShader,
        "DirLightShadow",
        0xFFFFFFFF
        );
}

void DeferredLightBufRenderer::LightRenderer::DirLightUseVarianceShadow(DirLightParam& param)
{
    Matrix View;
    Matrix Proj;

    //�s��v�Z
    switch (param.Shadow.type)
    {
    case DirLightParam::Type::Ortho:

        //�r���[�s��v�Z
        D3DXMatrixLookAtLH(
            &View,
            (D3DXVECTOR3*)&param.Shadow.origin,
            (D3DXVECTOR3*)&(param.Shadow.origin + param.vec),
            &D3DXVECTOR3(0, 1, 0)
            );

        //�v���W�F�N�V�����s��v�Z
        D3DXMatrixOrthoLH(&Proj, param.Shadow.Size, param.Shadow.Size, param.Shadow.Near, param.Shadow.Far);
        View *= Proj;
        break;

    case DirLightParam::Type::Perspective:
        Proj = matView * matProjection;
        D3DXMatrixInverse(&View, 0, &matView);
        Proj *= View;

        //�r���[�s��v�Z
        D3DXMatrixLookAtLH(
            &View,
            (D3DXVECTOR3*)&param.Shadow.origin,
            (D3DXVECTOR3*)&(param.Shadow.origin + param.vec),
            &D3DXVECTOR3(0, 1, 0)
            );

        Proj *= View;

        D3DXMatrixPerspectiveFovLH(&View, param.Shadow.Size, 1, param.Shadow.Near, param.Shadow.Far);
        //  D3DXMatrixOrthoLH(&Proj, param.Shadow.Size, param.Shadow.Size, param.Shadow.Near, param.Shadow.Far);
        Proj *= View;
        break;

    default:
        assert(0 && "���s���̉e�^�C�v���ݒ肳��Ă��܂���");
        return;
    }

    //�r���[�v���W�F�N�V�����s����Z�o���A�G�t�F�N�g�ɃZ�b�g

    m_pMgr->m_pShader->SetValue("g_Shadow_VP_mat", View);


    //���݂̃r���[�|�[�g��ۑ����A�e�p�̃r���[�|�[�g���Z�b�g
    D3DVIEWPORT9 SaveViewport;
    iexSystem::Device->GetViewport(&SaveViewport);

    iexSystem::Device->SetViewport(&m_pMgr->m_ShadowViewport);

    //���݂�RT��ۑ����A�e�p�e�N�X�`����RT��
    Surface* pDiffuseTex, *pSpecularTex;
    iexSystem::Device->GetRenderTarget(0, &pDiffuseTex);
    iexSystem::Device->GetRenderTarget(1, &pSpecularTex);

    m_pMgr->m_pShadowDepthTexture2->RenderTarget(0);
    iexSystem::Device->SetRenderTarget(1, 0);

    //���݂̃f�v�X�X�e���V���o�b�t�@��ۑ����e�p�̂��̂�
    Surface* pSaveDepthStencil;
    iexSystem::Device->GetDepthStencilSurface(&pSaveDepthStencil);

    iexSystem::Device->SetDepthStencilSurface(m_pMgr->m_pDepthStencil);

    //�N���A
    iexPolygon::RectPlus(
        0,
        0,
        m_pMgr->m_ShadowViewport.Width,
        m_pMgr->m_ShadowViewport.Height,
        m_pMgr->m_pShader,
        "ClearZ",
        0xFFFFFFFF);

    iexSystem::Device->Clear(
        0,
        0,
        D3DCLEAR_ZBUFFER,
        0,
        1,
        0
        );

    //Z�l��`��
    param.Shadow.pDepthRenderer->Render(
        m_pMgr->m_pShader,
        "WriteZf2"
        );

    //�r���[�|�[�g�����ɖ߂�
    iexSystem::Device->SetViewport(&SaveViewport);

    //RT�����Ƃɖ߂�
    iexSystem::Device->SetRenderTarget(0, pDiffuseTex);
    iexSystem::Device->SetRenderTarget(1, pSpecularTex);

    pDiffuseTex->Release();
    pSpecularTex->Release();

    //�f�v�X�X�e���V���o�b�t�@�����Ƃɖ߂�
    iexSystem::Device->SetDepthStencilSurface(pSaveDepthStencil);
    pSaveDepthStencil->Release();

    m_pMgr->m_pShader->SetValue("g_TexOffsetX", 1.f / (float)m_pMgr->m_ShadowViewport.Width);
    m_pMgr->m_pShader->SetValue("g_TexOffsetY", 1.f / (float)m_pMgr->m_ShadowViewport.Height);

    //���s���`��
    iexPolygon::RectPlus(
        0,
        0,
        (int)m_pMgr->m_X,
        (int)m_pMgr->m_Y,
        m_pMgr->m_pShader,
        "DirLightShadowVariance",
        0xFFFFFFFF
        );
}

void DeferredLightBufRenderer::LightRenderer::SpotLightUseShadow(SpotLightParam& param)
{
    Matrix View;
    Matrix Proj;

    //�r���[�s��v�Z
    D3DXMatrixLookAtLH(
        &View,
        (D3DXVECTOR3*)&param.origin,
        (D3DXVECTOR3*)&(param.target),
        &D3DXVECTOR3(0, 1, 0)
        );

    //�v���W�F�N�V�����s��v�Z
    D3DXMatrixOrthoLH(&Proj, param.size * 4, param.size * 4, 1, Vector3Length(param.target - param.origin) * 2);
    View *= Proj;
    //�r���[�v���W�F�N�V�����s����Z�o���A�G�t�F�N�g�ɃZ�b�g

    m_pMgr->m_pShader->SetValue("g_Shadow_VP_mat", View);


    //���݂̃r���[�|�[�g��ۑ����A�e�p�̃r���[�|�[�g���Z�b�g
    D3DVIEWPORT9 SaveViewport;
    iexSystem::Device->GetViewport(&SaveViewport);

    iexSystem::Device->SetViewport(&m_pMgr->m_ShadowViewport);

    //���݂�RT��ۑ����A�e�p�e�N�X�`����RT��
    Surface* pDiffuseTex, *pSpecularTex;
    iexSystem::Device->GetRenderTarget(0, &pDiffuseTex);
    iexSystem::Device->GetRenderTarget(1, &pSpecularTex);

    m_pMgr->m_pShadowDepthTexture->RenderTarget(0);
    iexSystem::Device->SetRenderTarget(1, 0);

    //���݂̃f�v�X�X�e���V���o�b�t�@��ۑ����e�p�̂��̂�
    Surface* pSaveDepthStencil;
    iexSystem::Device->GetDepthStencilSurface(&pSaveDepthStencil);

    iexSystem::Device->SetDepthStencilSurface(m_pMgr->m_pDepthStencil);

    //�N���A
    iexPolygon::RectPlus(
        0,
        0,
        m_pMgr->m_ShadowViewport.Width,
        m_pMgr->m_ShadowViewport.Height,
        m_pMgr->m_pShader,
        "ClearZ",
        0xFFFFFFFF);

    iexSystem::Device->Clear(
        0,
        0,
        D3DCLEAR_ZBUFFER,
        0,
        1,
        0
        );

    //Z�l��`��
    param.Shadow.pDepthRenderer->Render(
        m_pMgr->m_pShader,
        "WriteZ"
        );

    //�r���[�|�[�g�����ɖ߂�
    iexSystem::Device->SetViewport(&SaveViewport);

    //RT�����Ƃɖ߂�
    iexSystem::Device->SetRenderTarget(0, pDiffuseTex);
    iexSystem::Device->SetRenderTarget(1, pSpecularTex);

    pDiffuseTex->Release();
    pSpecularTex->Release();

    //�f�v�X�X�e���V���o�b�t�@�����Ƃɖ߂�
    iexSystem::Device->SetDepthStencilSurface(pSaveDepthStencil);
    pSaveDepthStencil->Release();

    //�`��
    m_pMgr->m_pSpotMesh->Render(
        m_pMgr->m_pShader,
        "SpotLightShadow"
        );
}


//�_�����`��
void DeferredLightBufRenderer::LightRenderer::PointLight(PointLightParam& param)
{
    
    //���[���h�ϊ��s���ݒ�
    Matrix& m = m_pMgr->m_pShpereMesh->TransMatrix;

    D3DXMatrixTranslation(&m, param.pos.x, param.pos.y, param.pos.z);
    m._11 = m._22 = m._33 = param.size;

    //�p�����[�^���Z�b�g
    m_pMgr->m_pShader->SetValue("g_LightPos", Vector3MulMatrix(param.pos, m_pMgr->m_ViewMatrix));
    m_pMgr->m_pShader->SetValue("g_LightColor", param.color);
    m_pMgr->m_pShader->SetValue("g_LightSize", param.size);


    //�_�����`��
    m_pMgr->m_pShpereMesh->Render(
        m_pMgr->m_pShader,
        "PointLight"
        );

}

//�X�|�b�g���C�g�`��
void DeferredLightBufRenderer::LightRenderer::SpotLight(SpotLightParam& param)
{
    //���[���h�ϊ��s���ݒ�
    Matrix& m = m_pMgr->m_pSpotMesh->TransMatrix;

    Vector3 up = param.target - param.origin;
    Vector3 r,f;

    const float Length = up.Length();

    Vector3Cross(r, up, Vector3(1, 0, 0)); 

    if (r.x == 0 && r.y == 0 && r.z == 0)
    {
        Vector3Cross(r, up, Vector3(0, 0, 1));
    }

    r.Normalize();
    r *= param.size;

    Vector3Cross(f, r, up);
    f.Normalize();
    f *= param.size;

    m._11 = r.x;
    m._12 = r.y;
    m._13 = r.z;

    m._21 = up.x;
    m._22 = up.y;
    m._23 = up.z;
    
    m._31 = f.x;
    m._32 = f.y;
    m._33 = f.z;

    m._41 = param.origin.x;
    m._42 = param.origin.y;
    m._43 = param.origin.z;

    //�p�����[�^���Z�b�g
    m_pMgr->m_pShader->SetValue("g_LightPos", Vector3MulMatrix(param.origin, m_pMgr->m_ViewMatrix));
    m_pMgr->m_pShader->SetValue("g_LightColor", param.color);
    m_pMgr->m_pShader->SetValue("g_LightSize", param.size);
    m_pMgr->m_pShader->SetValue("g_LightVec", Vector3MulMatrix3x3(Vector3Normalize(param.target - param.origin), m_pMgr->m_ViewMatrix));
    m_pMgr->m_pShader->SetValue("g_LightMaxAngle", 1 / (1 - (Vector3Normalize(Vector3(param.size, Length, 0)).y)));

    if (param.Shadow.visible)
    {
        SpotLightUseShadow(param);
        return;
    }
    
    //�`��
    m_pMgr->m_pSpotMesh->Render(
        m_pMgr->m_pShader,
        "SpotLight"
        );
}

//�������C�g�`��
void DeferredLightBufRenderer::LightRenderer::HemiLight(HemiLightParam& param)
{
    m_pMgr->m_pShader->SetValue("g_LightVec", Vector3MulMatrix3x3(param.Up, m_pMgr->m_ViewMatrix));
    m_pMgr->m_pShader->SetValue("g_LightColor", param.SkyColor);
    m_pMgr->m_pShader->SetValue("g_LightGroundColor", param.GroundColor);
    

    //���s���`��
    iexPolygon::RectPlus(
        0,
        0,
        (int)m_pMgr->m_X,
        (int)m_pMgr->m_Y,
        m_pMgr->m_pShader,
        "HemiLight",
        0xFFFFFFFF
        );
}

//�����`��
void DeferredLightBufRenderer::LightRenderer::AmbientLight(AmbientParam& param)  
{
    m_pMgr->m_pShader->SetValue("g_LightColor", param.color);

    if (!param.Occlusion.Enable)
    {
        //���s���`��
        iexPolygon::RectPlus(
            0,
            0,
            (int)m_pMgr->m_X,
            (int)m_pMgr->m_Y,
            m_pMgr->m_pShader,
            "AmbientLight",
            0xFFFFFFFF
            );

        return;
    }

    m_pMgr->m_pShader->SetValue("g_OcclusionSampSize", param.Occlusion.SamplingSize);
    m_pMgr->m_pShader->SetValue("g_OcclusionCalcDiffZ", param.Occlusion.CalcDiffZ);

    
    //���s���`��
    iexPolygon::RectPlus(
        0,
        0,
        (int)m_pMgr->m_X,
        (int)m_pMgr->m_Y,
        m_pMgr->m_pShader,
        "AmbientOcclusionLight",
        0xFFFFFFFF
        );
}

//---------------------------------------------
//  ���C�g�o�b�t�@�����_���[
//---------------------------------------------


DeferredLightBufRenderer::DeferredLightBufRenderer(
    UINT X,
    UINT Y,
    const char* pData_FilePath
    ) :
    m_X(X),
    m_Y(Y)
{

    char path[MAX_PATH];

    //�V�F�[�_�[�\�[�X�ǂݍ���
    sprintf(path, "%s%s", pData_FilePath, "\\DeferredLight.fx");
    m_pShader = new iexShader(path);

    //���C�g�p���b�V���̓ǂݍ���
    sprintf(path, "%s%s", pData_FilePath, "\\sphere.IMO");
    m_pShpereMesh = new iexMesh(path);


    sprintf(path, "%s%s", pData_FilePath, "\\spot.IMO");
    m_pSpotMesh = new iexMesh(path);
    

    //�e�N�X�`������
    m_pDiffuseTexture  = new iex2DObj((ULONG)X, (ULONG)Y, IEX2D_RENDERTARGET);
    m_pSpecularTexture = new iex2DObj((ULONG)X, (ULONG)Y, IEX2D_RENDERTARGET);

    //�e�p�r���[�|�[�g��ݒ�
    m_ShadowViewport.X = 0;
    m_ShadowViewport.Y = 0;
    m_ShadowViewport.MaxZ = 1;
    m_ShadowViewport.MinZ = 0;
    m_ShadowViewport.Width = 1024;
    m_ShadowViewport.Height = 1024;

    //�e�p�f�v�X�X�e���V���o�b�t�@����
    iexSystem::Device->CreateDepthStencilSurface(
        (UINT)m_ShadowViewport.Width,
        (UINT)m_ShadowViewport.Height,
        D3DFMT_D16,
        D3DMULTISAMPLE_NONE,
        0,
        FALSE,
        &m_pDepthStencil,
        NULL
        );

    //�e�pZ�l�e�N�X�`������
    m_pShadowDepthTexture = new iex2DObj(
        (ULONG)m_ShadowViewport.Width,
        (ULONG)m_ShadowViewport.Height,
        IEX2D_FLOAT
        );

    m_pShadowDepthTexture2 = new iex2DObj(
        (ULONG)m_ShadowViewport.Width,
        (ULONG)m_ShadowViewport.Height,
        IEX2D_FLOAT2
        );

    m_pShader->SetValue("ShadowDepthTex", m_pShadowDepthTexture->GetTexture());
    m_pShader->SetValue("ShadowDepthTex2", m_pShadowDepthTexture2->GetTexture());

}


DeferredLightBufRenderer::~DeferredLightBufRenderer()
{
    delete m_pShader;
    delete m_pShpereMesh;
    delete m_pSpotMesh;
    delete m_pDiffuseTexture;
    delete m_pSpecularTexture;
    delete m_pShadowDepthTexture;
    delete m_pShadowDepthTexture2;
}

//���C�g�o�b�t�@�N���A
void DeferredLightBufRenderer::Clear()
{
    //�X�N���[���T�[�t�F�C�X��ۑ�
    Surface*  pScreen;
    iexSystem::Device->GetRenderTarget(0, &pScreen);


    //�����_�[�^�[�Q�b�g�ɐݒ�
    m_pDiffuseTexture->RenderTarget(0);
    m_pSpecularTexture->RenderTarget(1);


    //�N���A
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
    iexSystem::Device->SetRenderTarget(2, 0);
    iexSystem::Device->SetRenderTarget(3, 0);

    pScreen->Release();
}

//���C�g�o�b�t�@�ɕ`����s
void DeferredLightBufRenderer::Render(IRenderer* pRenderer)
{
    //���C�g�`��N���X
    LightRenderer Renderer(this);

    //�����_�[�^�[�Q�b�g�ɐݒ�
    m_pDiffuseTexture->RenderTarget(0);
    m_pSpecularTexture->RenderTarget(1);

    //���C�g�`��
    pRenderer->Render(&Renderer);

    //���ɖ߂�
    iexSystem::Device->SetRenderTarget(1, 0);
}


//�v�Z�̌��ƂȂ�G�o�b�t�@�e�N�X�`�����Z�b�g
void DeferredLightBufRenderer::SetGbufTextures(
    iex2DObj* NormalDepth,           //�@���E�[�x
    iex2DObj* RoughnessLightMap      //�e���E���C�g�}�b�v�l
    )
{
    m_pShader->SetValue("NormalDepthTex", NormalDepth->GetTexture());
    m_pShader->SetValue("RoughnessLightMapTex", RoughnessLightMap->GetTexture());
}

//�r���[�֘A�̍s���ݒ�E�X�V����
void DeferredLightBufRenderer::UpdateViewParam(
    const Matrix&  VP_mat,     //�r���[�v���W�F�N�V�����s��
    const Vector3& CameraPos
    )
{
    Matrix m = VP_mat;

    m_ViewMatrix = matView;

    D3DXMatrixInverse(&m_ViewInvMatrix, 0, &m_ViewMatrix);

    m_pShader->SetValue("g_V_inv_mat", m_ViewInvMatrix);

    m_pShader->SetValue("g_VP_mat", m);

    m_pShader->SetValue("g_P_mat", matProjection);

    D3DXMatrixInverse(&m, 0, &matProjection);
    m_pShader->SetValue("g_P_inv_mat", m);
    
}


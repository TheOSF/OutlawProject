#include "BlurEffectRenderer.h"
#include "../../Camera/Camera.h"

BlurEffectRenderer::BlurEffectRenderer(const char* pDataFile)
{
    char path[MAX_PATH];

    //球メッシュ読み込み
    sprintf(path, "%s%s", pDataFile, "\\sphere.IMO");
    m_pSphereMesh = new iexMesh(path);
    D3DXMatrixIdentity(&m_pSphereMesh->TransMatrix);


    //コーンメッシュ読み込み
    sprintf(path, "%s%s", pDataFile, "\\spot.IMO");
    m_pConeMesh = new iexMesh(path);
    D3DXMatrixIdentity(&m_pConeMesh->TransMatrix);
    

    //シェーダーファイル読み込み
    sprintf(path, "%s%s", pDataFile, "\\BlurEffect.fx");
    m_pShader = new iexShader(path);

    //ピクセルサイズ設定
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
    //結果をレンダーターゲットに設定
    iexSystem::Device->SetRenderTarget(0, pOut);
    for (int i = 1; i < 4; ++i)
    {
        iexSystem::Device->SetRenderTarget(i, 0);
    }

    //元画像を描画
    pIn->Render();

    Matrix VP_mat = matView*matProjection;

    //描画パラメータ設定
    m_pShader->SetValue("SourceTexture", pIn->GetTexture());
    m_pShader->SetValue("g_VP_mat", VP_mat);

    RenderConeBlur(VP_mat);

    RenderSphereBlur(VP_mat);
  
}


void BlurEffectRenderer::RenderSphereBlur(const Matrix& VP_mat)
{

    //球ブラーを描画
    Matrix& m = m_pSphereMesh->TransMatrix;
    Vector4 tex_pos, temp;
    float Size;

    for (auto& it : m_BlurSphere)
    {
        //スクリーンテクスチャ座標を算出
        tex_pos.x = it.pos.x;
        tex_pos.y = it.pos.y;
        tex_pos.z = it.pos.z;
        tex_pos.w = 1;


        D3DXVec4Transform(&tex_pos, &tex_pos, &VP_mat);
        tex_pos.x = tex_pos.x / tex_pos.w*0.5f + 0.5f;
        tex_pos.y = tex_pos.y / tex_pos.w*-0.5f + 0.5f;


        //スクリーンテクスチャ上での大きさを算出
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

        //パラメータ設定

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

        //描画
        m_pSphereMesh->Render(m_pShader, "radiation_blur");
    }



}



void BlurEffectRenderer::RenderConeBlur(const Matrix& VP_mat)
{


    //コーンブラーを描画
    Matrix& m = m_pConeMesh->TransMatrix;
    Vector4 tex_pos, temp, tr;
    float Size, l;

    Vector3 up, r, f;

    for (auto& it : m_BlurCone)
    {
        //スクリーンテクスチャ座標を算出
        tex_pos.x = it.origin.x;
        tex_pos.y = it.origin.y;
        tex_pos.z = it.origin.z;
        tex_pos.w = 1;


        D3DXVec4Transform(&tex_pos, &tex_pos, &VP_mat);
        tex_pos.x = tex_pos.x / tex_pos.w*0.5f + 0.5f;
        tex_pos.y = tex_pos.y / tex_pos.w*-0.5f + 0.5f;


        //スクリーンテクスチャ上での大きさを算出
        temp.x = it.target.x;
        temp.y = it.target.y;
        temp.z = it.target.z;
        temp.w = 1;

        D3DXVec4Transform(&temp, &temp, &VP_mat);
        temp.x = temp.x / temp.w*0.5f + 0.5f;
        temp.y = temp.y / temp.w*-0.5f + 0.5f;

        temp.x -= tex_pos.x;
        temp.y -= tex_pos.y;

        Size = sqrtf(temp.x*temp.x + temp.y*temp.y);

        {
            temp.x /= Size;
            temp.y /= Size;
        }


        //パラメータ設定

        m_pShader->SetValue(
            "g_center_pos_x",
            tex_pos.x
            );

        m_pShader->SetValue(
            "g_center_pos_y",
            tex_pos.y
            );

        m_pShader->SetValue(
            "g_vec_x",
            temp.x
            );

        m_pShader->SetValue(
            "g_vec_y",
            temp.y
            );


        m_pShader->SetValue(
            "g_blur_power",
            it.power
            );

        m_pShader->SetValue(
            "g_blur_size",
            Size
            );




        up = it.target - it.origin;

        Vector3Cross(r, up, DefCamera.GetForward());

        if (r.x == 0 && r.y == 0 && r.z == 0)
        {
            Vector3Cross(r, up, Vector3(0, 0, 1));
        }

        r.Normalize();
        r *= it.width;

        Vector3Cross(f, r, up);
        f.Normalize();
        f *= it.width;

        m._11 = r.x;
        m._12 = r.y;
        m._13 = r.z;

        m._21 = up.x;
        m._22 = up.y;
        m._23 = up.z;

        m._31 = f.x;
        m._32 = f.y;
        m._33 = f.z;

        m._41 = it.origin.x;
        m._42 = it.origin.y;
        m._43 = it.origin.z;


        //角度計算
        tr.x = it.target.x + r.x;
        tr.y = it.target.y + r.y;
        tr.z = it.target.z + r.z;
        tr.w = 1;

        D3DXVec4Transform(&tr, &tr, &VP_mat);
        tr.x = tr.x / tr.w*0.5f + 0.5f;
        tr.y = tr.y / tr.w*-0.5f + 0.5f;

        tr.x -= tex_pos.x;
        tr.y -= tex_pos.y;

        l = sqrtf(tr.x*tr.x + tr.y*tr.y);
        tr.x /= l;
        tr.y /= l;

        float d = tr.x*temp.x + tr.y*temp.y;

        //d = 1 / (1 - d);

        d = PI / 4.0f;

        m_pShader->SetValue(
            "g_angle",
            d
            );

        //描画
        m_pConeMesh->Render(m_pShader, "radiation_blur_cone");
    }



}
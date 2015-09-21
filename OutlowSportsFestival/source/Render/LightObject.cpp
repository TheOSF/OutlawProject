#include "LightObject.h"
#include "../Render/Renderer.h"

//---------------------------------------------------------------
//   ポイントライト
//---------------------------------------------------------------


PointLight::PointLight()
{
    param.color = Vector3(1, 1, 1);
    param.pos = Vector3(0, 0, 0);
    param.size = 10;
}


void PointLight::Render(LightRenderer* pLightRenderer)
{
    DeferredLightBufRenderer::PointLightParam temp = param;
    pLightRenderer->PointLight(temp);
}


//---------------------------------------------------------------
//   スポットライト
//---------------------------------------------------------------


SpotLight::SpotLight()
{
    param.color = Vector3(1, 1, 1);
    param.origin = Vector3(0, 0, 0);
    param.target = Vector3(0, 0, 100);
    param.size = 30;

    param.Shadow.visible = false;
    param.Shadow.pDepthRenderer = nullptr;
}


void SpotLight::Render(LightRenderer* pLightRenderer)
{
    DeferredLightBufRenderer::SpotLightParam temp = param;

    if (temp.Shadow.visible)
    {
        RendererManager::DepthRenderer* p=DefRendererMgr.GetDepthRenderer();
        p->m_Type = DeferredRenderer::DepthRenderType::SpotLight;
        temp.Shadow.pDepthRenderer = p;
    }

    pLightRenderer->SpotLight(temp);
}



//---------------------------------------------------------------
//   ディレクショナルライト
//---------------------------------------------------------------


DirLight::DirLight()
{
    param.color = Vector3(1, 1, 1);
    param.vec = Vector3Normalize(Vector3(1, -1.5f, 1));

    param.Shadow.visible = false;
    param.Shadow.pDepthRenderer = nullptr;
    param.Shadow.Far = 300;
    param.Shadow.Near = 5;
    param.Shadow.origin = Vector3(0, -10, 0);
    param.Shadow.Size = 150;
    param.Shadow.type = DeferredLightBufRenderer::DirLightParam::Type::Ortho;
    
}


void DirLight::Render(LightRenderer* pLightRenderer)
{
    DeferredLightBufRenderer::DirLightParam temp = param;
    if (temp.Shadow.visible)
    {
        RendererManager::DepthRenderer* p = DefRendererMgr.GetDepthRenderer();
        p->m_Type = DeferredRenderer::DepthRenderType::DirLight;
        temp.Shadow.pDepthRenderer = p;
        temp.Shadow.pDepthRenderer = DefRendererMgr.GetDepthRenderer();
    }
    pLightRenderer->DirLight(temp);
}

//---------------------------------------------------------------
//   半球ライト
//---------------------------------------------------------------


HemiLight::HemiLight()
{
    param.GroundColor = Vector3(1, 0, 0);
    param.SkyColor = Vector3(0, 0, 1);
    param.Up = Vector3AxisY;
}


void HemiLight::Render(LightRenderer* pLightRenderer)
{
    DeferredLightBufRenderer::HemiLightParam temp = param;
    pLightRenderer->HemiLight(temp);
}



//---------------------------------------------------------------
//   アンビエントライト
//---------------------------------------------------------------


AmbientLight::AmbientLight()
{
    param.color = Vector3(0, 0.2f, 0);
    param.Occlusion.Enable = false;
    param.Occlusion.SamplingSize = 2.5f;

}


void AmbientLight::Render(LightRenderer* pLightRenderer)
{
    DeferredLightBufRenderer::AmbientParam temp = param;
   // temp.Occlusion.Enable = GetKeyState('A') != 0;

    pLightRenderer->AmbientLight(temp);
}


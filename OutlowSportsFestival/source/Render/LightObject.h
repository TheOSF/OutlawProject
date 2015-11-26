#ifndef __LIGHT_OBJECT_H__
#define __LIGHT_OBJECT_H__

#include "../GameSystem/ForwardDecl.h"
#include "Renderer.h"

//---------------------------------------------------------------
//   ポイントライト
//---------------------------------------------------------------

class PointLight :public LightObject
{
public:
    DeferredLightBufRenderer::PointLightParam param;

    PointLight();

private:

    void Render(LightRenderer* pLightRenderer);
};


//---------------------------------------------------------------
//   スポットライト
//---------------------------------------------------------------
class SpotLight :public LightObject
{
public:
    DeferredLightBufRenderer::SpotLightParam param;

    SpotLight();

    void AddCharacterDepthRenderer(CharacterBase* pChr);
    void SetCharacterDepthRender();
private:

    class MyDepthRenderer :public DeferredLightBufRenderer::IDepthRenderer
    {
    public:
        void Render(iexShader* pShader, const char* technique)override;
        std::list<CharacterBase*> DrawChrList;
    };

    MyDepthRenderer m_MyDepthRenderer;

    void Render(LightRenderer* pLightRenderer);
};


//---------------------------------------------------------------
//   ディレクショナルライト
//---------------------------------------------------------------
class DirLight :public LightObject
{
public:
    DeferredLightBufRenderer::DirLightParam param;

    DirLight();

private:

    void Render(LightRenderer* pLightRenderer);
};


//---------------------------------------------------------------
//   半球ライト
//---------------------------------------------------------------
class HemiLight :public LightObject
{
public:
    DeferredLightBufRenderer::HemiLightParam param;

    HemiLight();

private:

    void Render(LightRenderer* pLightRenderer);
};

//---------------------------------------------------------------
//   環境ライト
//---------------------------------------------------------------
class AmbientLight :public LightObject
{
public:
    DeferredLightBufRenderer::AmbientParam param;

    AmbientLight();

private:

    void Render(LightRenderer* pLightRenderer);
};

#endif
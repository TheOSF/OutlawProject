#ifndef __LIGHT_OBJECT_H__
#define __LIGHT_OBJECT_H__

#include "../GameSystem/ForwardDecl.h"
#include "Renderer.h"

//---------------------------------------------------------------
//   �|�C���g���C�g
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
//   �X�|�b�g���C�g
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
//   �f�B���N�V���i�����C�g
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
//   �������C�g
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
//   �����C�g
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
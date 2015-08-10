#ifndef __POST_EFFET_RENDERER_H__
#define __POST_EFFET_RENDERER_H__

#include "iextreme.h"

class PostEffectRenderer
{
public:

    PostEffectRenderer(const char* pDataFile);
    ~PostEffectRenderer();

    void SetInputTexture(iex2DObj*  pInputTexture);
    void SetOutputTexture(iex2DObj*  pOutputTexture);

    //���u���[��`�悷��
    void RenderSphereBlur(
        CrVector3 pos,
        float size,
        float power
        );

    //�~���u���[��`�悷��
    void RenderSpotBlur(
        CrVector3 origin,
        CrVector3 vec,
        float length,
        float width
        );

private:

    iexShader* m_pShader;
    iex2DObj*  m_pInputTexture;
    iex2DObj*  m_pOutputTexture;

};

#endif
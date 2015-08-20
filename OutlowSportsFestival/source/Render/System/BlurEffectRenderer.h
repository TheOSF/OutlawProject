#ifndef __POST_EFFECT_RENDERER_H__
#define __POST_EFFECT_RENDERER_H__


#include "DeferredLightManager.h"
#include <list>


class BlurEffectRenderer :public DeferredLightManager::IPostEffectRenderer
{
public:

    struct SphereBlur
    {
        Vector3 pos;
        float   scale;
        float   power;
    };

    BlurEffectRenderer(const char* pDataFile);
    ~BlurEffectRenderer();

    void Render(
        iex2DObj* pIn,
        Surface*  pOut
        )override;

    std::list<SphereBlur> m_BlurSphere;
    
private:

    iexMesh*    m_pSphereMesh;
    iexShader*  m_pShader;
};

#endif
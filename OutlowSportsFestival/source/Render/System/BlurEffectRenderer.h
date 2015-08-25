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

    struct ConeBlur
    {
        Vector3 origin;
        Vector3 target;
        float   power;
        float   width;
    };

    BlurEffectRenderer(const char* pDataFile);
    ~BlurEffectRenderer();

    void Render(
        iex2DObj* pIn,
        Surface*  pOut
        )override;

    std::list<SphereBlur> m_BlurSphere;
    std::list<ConeBlur>   m_BlurCone;
    
private:

    iexMesh*    m_pSphereMesh;
    iexMesh*    m_pConeMesh;
    iexShader*  m_pShader;
};

#endif
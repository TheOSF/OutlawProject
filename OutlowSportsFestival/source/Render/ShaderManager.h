#ifndef __RENDER_MANAGER_H__
#define __RENDER_MANAGER_H__

#include "iextreme.h"
#include <list>


class ShaderManager
{
public:

    class PointLight
    {
    public:
        PointLight(CrVector3 pos, float size, Vector3 color);
        ~PointLight();
        
        Vector3 pos;
        Vector3 color;
        float   size;
    };

    class ShadowCamera
    {
    public:
        ShadowCamera();
        ~ShadowCamera();

        Matrix View;
        Matrix Proj;

        void SetViewParam(
            CrVector3 pos,
            CrVector3 target
            );

        void SetProjParam(
            float Size,
            float Near,
            float Far
            );
    };

    static ShaderManager& GetInstance();
    static void Release();

    void SetDirLight(Vector3 vec, Vector3 color);
    void SetCamera(Vector3 pos, Vector3 vec);

    void Update();

private:
    typedef std::list<PointLight*> PointLightList;

    static const int      m_NumPointLight;
    static const int      m_NumShadowCamera;
    static ShaderManager* m_pInstance;

    iex2DObj*             m_pToonSpecularTex;
    PointLightList        m_PointLightList;
    iex2DObj**            m_pShadowTex;

    ShaderManager();
    ~ShaderManager();

    void RegistLight(PointLight* pLight);
    void EraceLight(PointLight* pLight);

    void RegistShadowCamera();
    void EraceShadowCamera();
};


#define DefShaderMgr (ShaderManager::GetInstance())

#endif
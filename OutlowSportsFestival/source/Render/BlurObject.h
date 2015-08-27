#ifndef __BLUR_OBJECT_H__
#define __BLUR_OBJECT_H__

#include "Renderer.h"

//-------------------------------------------------------//
//    球ブラークラス
//-------------------------------------------------------//

class BlurObjectSphere :public PostEffectRenderer
{
public:
    BlurObjectSphere();
    ~BlurObjectSphere();

    bool Enable;

    Vector3   m_Pos;
    float     m_Size;
    float     m_Power;

private:

    void Render(
        SphereBlurList& SphereList,
        ConeBlurList&   ConeList
        );
};


//-------------------------------------------------------//
//    コーンブラークラス
//-------------------------------------------------------//

class BlurObjectCone :public PostEffectRenderer
{
public:
    BlurObjectCone();
    ~BlurObjectCone();

    bool Enable;

    Vector3   m_Origin;
    Vector3   m_Target;
    float     m_Size;
    float     m_Power;

private:

    void Render(
        SphereBlurList& SphereList,
        ConeBlurList&   ConeList
        );
};


#endif
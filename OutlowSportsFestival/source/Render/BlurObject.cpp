#include "BlurObject.h"


//-------------------------------------------------------//
//    球ブラークラス
//-------------------------------------------------------//

BlurObjectSphere::BlurObjectSphere() :
m_Pos(0, 0, 0),
m_Size(10),
m_Power(1),
Enable(true)
{

}

BlurObjectSphere::~BlurObjectSphere()
{

}

void BlurObjectSphere::Render(
    SphereBlurList& SphereList,
    ConeBlurList&   ConeList
    )
{
    if (!Enable)
    {
        return;
    }

    BlurEffectRenderer::SphereBlur p;

    p.pos = m_Pos;
    p.power = m_Power;
    p.scale = m_Size;

    SphereList.push_back(
        p
        );
}



//-------------------------------------------------------//
//    コーンブラークラス
//-------------------------------------------------------//

BlurObjectCone::BlurObjectCone():
m_Origin(0, 0, 0),
m_Target(0, 0, 0),
m_Size(10),
m_Power(1),
Enable(true)
{

}

BlurObjectCone::~BlurObjectCone()
{

}

void BlurObjectCone::Render(
    SphereBlurList& SphereList,
    ConeBlurList&   ConeList
    )
{
    if (!Enable)
    {
        return;
    }

    BlurEffectRenderer::ConeBlur p;

    p.origin = m_Origin;
    p.power = m_Power;
    p.target = m_Target;
    p.width = m_Size;

    ConeList.push_back(
        p
        );
}
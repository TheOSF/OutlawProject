#include "PointLightObject.h"


PointLightObject::PointLightObject(
    CrVector3 pos,
    CrVector3 color,
    float     size,
    RATIO     DeleteSpeed
    ) :
    m_PointLight(pos, size, color),
    m_DeleteSpeed(1 - DeleteSpeed)
{

}

PointLightObject::~PointLightObject()
{

}


bool PointLightObject::Update()
{
    //サイズ更新
    m_PointLight.size *= m_DeleteSpeed;

    return m_PointLight.size >= 0.1f;
}

bool PointLightObject::Msg(MsgType mt)
{
    return false;
}
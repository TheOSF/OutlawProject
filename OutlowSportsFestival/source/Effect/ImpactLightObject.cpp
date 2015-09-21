#include "ImpactLightObject.h"

ImpactLightObject::ImpactLightObject(
    CrVector3 pos,
    float     size,
    CrVector3 color,
    RATIO     PowerSubValue
    ) :
    m_PowerSubValue(1 - min(PowerSubValue, 1))
{
    m_Light.param.pos = pos;
    m_Light.param.size = size;
    m_Light.param.color = color;
}

ImpactLightObject::~ImpactLightObject()
{

}


bool ImpactLightObject::Update()
{
    m_Light.param.size *= m_PowerSubValue;

    return m_Light.param.size > 1.0f;
}

bool ImpactLightObject::Msg(MsgType mt)
{
    return false;
}
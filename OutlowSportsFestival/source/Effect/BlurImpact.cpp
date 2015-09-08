#include "BlurImpact.h"


//----------------------------------------------------------
//  球のブラーエフェクトオブジェクト
//----------------------------------------------------------

BlurImpactSphere::BlurImpactSphere(
    CrVector3 pos,
    float     size,
    float     power,
    UINT      timer
    ):
    m_MaxTime(timer),
    m_Timer(0),
    m_InitPower(power)
{
    m_BlurObj.m_Size = size;
    m_BlurObj.m_Pos = pos;
    m_BlurObj.m_Power = power;
    m_BlurObj.Enable = true;
}

BlurImpactSphere::~BlurImpactSphere()
{

}


bool BlurImpactSphere::Update()
{

    m_BlurObj.m_Power = m_InitPower * pow((1.0f - ((float)m_Timer / (float)m_MaxTime)), 2.0f);

    return ++m_Timer < m_MaxTime;
}


bool BlurImpactSphere::Msg(MsgType mt)
{
    return false;
}
#include "DamageObject.h"

DamageObject::DamageObject(DamageBase* pDamage, UINT Frame)
{
    m_pDamage = pDamage;
    m_Frame = (int)Frame;
}


DamageObject::~DamageObject()
{
    delete m_pDamage;
}


bool DamageObject::Update()
{

    return --m_Frame > 0;
}

bool DamageObject::Msg(MsgType mt)
{
    return false;
}
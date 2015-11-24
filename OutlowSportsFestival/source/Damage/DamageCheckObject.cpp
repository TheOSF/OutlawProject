#include "DamageCheckObject.h"

DamageCheckObject::DamageCheckObject(
    DamageManager::HitEventBase*   pHitEvent,
    const SphereParam&	           Param
    ):
    m_pHitEvent(pHitEvent)
{
    m_Param = Param;
}

DamageCheckObject::~DamageCheckObject()
{

}

bool DamageCheckObject::Update()
{
    class MyHitEvent :public DamageManager::HitEventBase
    {
    private:
        DamageManager::HitEventBase* pEvent;
    public:
        MyHitEvent(DamageManager::HitEventBase* pEvent) :
            pEvent(pEvent),
            hit(false){}

        bool hit;

        bool Hit(DamageBase* pDmg)
        {
            bool h = pEvent->Hit(pDmg);

            if (hit == false)
            {
                hit = h;
            }

            return h;
        }
    };

    MyHitEvent h(m_pHitEvent);

    DefDamageMgr.HitCheckSphere(m_Param, h);

    return !h.hit;
}

bool DamageCheckObject::Msg(GameObjectBase::MsgType type)
{
    return false;
}
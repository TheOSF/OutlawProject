#pragma once

#include "../GameSystem/GameObject.h"
#include "Damage.h"

//----------------------------------------------------------//
// �_���[�W��������o����Q�[���I�u�W�F�N�g
//----------------------------------------------------------//

class DamageCheckObject :public GameObjectBase
{
public:
    DamageCheckObject(
        DamageManager::HitEventBase*   pHitEvent,
        const SphereParam&	           Param
        );

    ~DamageCheckObject();

private:
    SphereParam                    m_Param;
    DamageManager::HitEventBase*   m_pHitEvent;

    bool Update();
    bool Msg(GameObjectBase::MsgType type);
};
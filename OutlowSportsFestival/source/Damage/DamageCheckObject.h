#pragma once

#include "../GameSystem/GameObject.h"
#include "Damage.h"

//----------------------------------------------------------//
// ダメージ判定を検出するゲームオブジェクト
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
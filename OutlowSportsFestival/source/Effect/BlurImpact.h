#pragma once

#include "../Render/BlurObject.h"
#include "../GameSystem/GameObject.h"
#include "iextreme.h"

//----------------------------------------------------------
//  球のブラーエフェクトオブジェクト
//----------------------------------------------------------
class BlurImpactSphere :public GameObjectBase
{
public:

    BlurImpactSphere(
        CrVector3 pos,
        float     size,
        float     power,
        UINT      timer
        );

    ~BlurImpactSphere();

private:

    BlurObjectSphere   m_BlurObj;
    const float        m_InitPower;
    const UINT         m_MaxTime;
    UINT               m_Timer;

    bool Update()override;
    bool Msg(MsgType mt)override;

};
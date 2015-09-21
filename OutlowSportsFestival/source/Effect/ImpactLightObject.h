#pragma once


#include "../utillity/ColorUtility.h"
#include "../Render/LightObject.h"
#include "../GameSystem/GameObject.h"

//------------------------------------------------
// 　一瞬だけ光るポイントライトオブジェクト
//------------------------------------------------

class ImpactLightObject :public GameObjectBase
{
public:
  
    ImpactLightObject(
        CrVector3 pos,
        float     size,
        CrVector3 color,
        RATIO     PowerSubValue
        );

    ~ImpactLightObject();

private:
    PointLight  m_Light;
    const RATIO m_PowerSubValue;

    bool Update();	//falseを返すと消去する
    bool Msg(MsgType mt);
};
#ifndef __POINT_LIGHT_OBJECT_H__
#define __POINT_LIGHT_OBJECT_H__

#include "../GameSystem/GameObject.h"
#include "../Render/ShaderManager.h"

//---------------------------------------------------
//　だんだん消えていくポイントライト
//---------------------------------------------------

class PointLightObject :public GameObjectBase
{
public:

    PointLightObject(
        CrVector3 pos,
        CrVector3 color,
        float     size,
        RATIO     DeleteSpeed
        );

    ~PointLightObject();

    bool Update();
    bool Msg(MsgType mt);

private:
    ShaderManager::PointLight  m_PointLight;
    const float                m_DeleteSpeed;
};

#endif
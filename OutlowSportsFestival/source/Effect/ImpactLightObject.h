#pragma once


#include "../utillity/ColorUtility.h"
#include "../Render/LightObject.h"
#include "../GameSystem/GameObject.h"

//------------------------------------------------
// �@��u��������|�C���g���C�g�I�u�W�F�N�g
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

    bool Update();	//false��Ԃ��Ə�������
    bool Msg(MsgType mt);
};
#ifndef __HIT_EFFECT_OBJECT_H__
#define __HIT_EFFECT_OBJECT_H__

#include "../GameSystem/GameObject.h"
#include "../Render/ForwardHDR_MeshRenderer.h"
#include "../utillity/ColorUtility.h"
#include "../Render/LightObject.h"

//------------------------------------------------//
//   ヒットエフェクトクラス
//------------------------------------------------//

class HitEffectObject :public GameObjectBase
{
public:

    HitEffectObject(
        CrVector3 pos,
        CrVector3 vec,
        float     length,
        float     width,
        Vector3   color
        );

    ~HitEffectObject();

private:

    ForwardHDR_MeshRenderer m_Renderer;
    float                   m_T;
    Vector3                 m_Color;
    PointLight              m_Light;

    void InitMatrix(
        CrVector3 pos,
        CrVector3 vec,
        float     length,
        float     width
        );

    void SetParticle(
        CrVector3 pos,
        CrVector3 vec,
        float     length,
        float     width,
        Vector3   color
        );

    void UpdateMatrix();
    bool Update();
    bool Msg(MsgType mt);

};
#endif
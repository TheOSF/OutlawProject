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
        Vector3   color,
        UINT      particle_level = 0,
        float     LightSize = 20.0f,
        UINT      frame = 8
        );

    ~HitEffectObject();

private:

    ForwardHDR_MeshRenderer m_Renderer;
    float                   m_T;
    Vector3                 m_Color;
    PointLight              m_Light;
    const UINT              m_Particle_level;
    const UINT              m_Frame;

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
#pragma once

#include "../GameSystem/GameObject.h"
#include "../Render/ForwardHDR_MeshRenderer.h"
#include "../utillity/ColorUtility.h"
#include "../Render/LightObject.h"

//------------------------------------------------//
//   ヒットエフェクトクラス
//------------------------------------------------//

class SoccerSpecialHitEffect :public GameObjectBase
{
public:

    SoccerSpecialHitEffect(
        CrVector3 pos,
        CrVector3 vec,
        Vector3   color,
        UINT      particle_level = 0
        );

    ~SoccerSpecialHitEffect();

private:

    ForwardHDR_MeshRenderer m_Renderer;
    float                   m_T;
    Vector3                 m_Color, m_Pos, m_Vec;
    PointLight              m_Light;
    const UINT              m_Particle_level;
    UINT                    m_Timer;

    void InitMatrix(
        CrVector3 pos,
        CrVector3 vec,
        float     length,
        float     width
        );

    void SetParticle(
        CrVector3 pos,
        CrVector3 vec,
        Vector3   color
        );

    void UpdateMatrix();
    bool Update();
    bool Msg(MsgType mt);

};

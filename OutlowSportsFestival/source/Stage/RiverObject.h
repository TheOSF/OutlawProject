#pragma once

#include "../GameSystem/GameObject.h"
#include "../Render/Renderer.h"

//--------------------------------------------------//
//  川　オブジェクト
//--------------------------------------------------//

class RiverObject :public GameObjectBase, public ForwardHDRRenderer
{
public:
    RiverObject(
        CrVector3   pos,
        CrVector3   scale,
        CrVector3   angle,
        CrVector2   speed
        );

    ~RiverObject();

private:

    Matrix         m_Mat;
    iexMesh*       m_pMesh;
    const Vector2  m_Speed;
    Vector2        m_DiffTex;
    iex2DObj*      m_pTexture;

    void CalcZ();
    void Render();

    bool Update();
    bool Msg(MsgType mt);

};
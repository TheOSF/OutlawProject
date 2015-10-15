#ifndef __DEBUG_DRAW_H__
#define __DEBUG_DRAW_H__

#include "iextreme.h"
#include "../utillity/ColorUtility.h"
#include "../GameSystem/GameObject.h"
#include "../Render/Renderer.h"

//--------------------------------------------------------
//        デバッグ描画用　球
//--------------------------------------------------------

class DebugDrawSphere :
    public GameObjectBase, public ForwardRenderer
{
public:
    static const UINT LIVETIME_LIMITLESS;

    DebugDrawSphere(
        CrVector3   pos,
        float       size,
        COLORf      color = COLORf(1, 1, 1, 1), 
        UINT        live_time = 1,     //出現時間(LIVETIME_LIMITLESS を指定すると消えない)
        CrVector3   move = Vector3Zero //移動ベクトル
        );

    ~DebugDrawSphere();

private:
    LPIEXMESH           m_pMesh;
    Matrix              m_TransMat;
    const Vector3       m_Move;
    Vector4             m_Color;
    UINT                m_Livetime;
    bool                m_FirstDraw;

    bool Update();
    bool Msg(MsgType mt);

    void CalcZ();
    void Render();
};


//--------------------------------------------------------
//        デバッグ描画用　ポール
//--------------------------------------------------------

class DebugDrawPole :
    public GameObjectBase, public ForwardRenderer
{
public:
    static const UINT LIVETIME_LIMITLESS;

    DebugDrawPole(
        CrVector3   pos1,
        CrVector3   pos2,
        float       width,
        COLORf      color = COLORf(1, 1, 1, 1),
        UINT        live_time = 1,     //出現時間(LIVETIME_LIMITLESS を指定すると消えない)
        CrVector3   move = Vector3Zero //移動ベクトル
        );

    ~DebugDrawPole();

private:
    LPIEXMESH           m_pMesh;
    Matrix              m_TransMat;
    const Vector3       m_Move;
    Vector4             m_Color;
    UINT                m_Livetime;
    bool                m_FirstDraw;

    bool Update();
    bool Msg(MsgType mt);

    void CalcZ();
    void Render();
};

//--------------------------------------------------------
//        デバッグ描画用　ライン
//--------------------------------------------------------

class DebugDrawLine :
    public GameObjectBase, public ForwardRenderer
{
public:
    static const UINT LIVETIME_LIMITLESS;

    DebugDrawLine(
        CrVector3   pos1,
        CrVector3   pos2,
        float       width,
        COLORf      color = COLORf(1, 1, 1, 1),
        UINT        live_time = 1     //出現時間(LIVETIME_LIMITLESS を指定すると消えない)
        );

    ~DebugDrawLine();

private:
    DWORD               m_Color;
    UINT                m_Livetime;
    bool                m_FirstDraw;
    const Vector3       m_Pos1, m_Pos2;
    const float         m_Width;

    bool Update();
    bool Msg(MsgType mt);

    void CalcZ();
    void Render();
};


#endif
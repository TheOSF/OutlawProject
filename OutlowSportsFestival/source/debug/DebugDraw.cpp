#include "DebugDraw.h"
#include "../GameSystem/ResourceManager.h"
#include "../Camera/Camera.h"
#include "../IexSystem/System.h"

const UINT DebugDrawSphere::LIVETIME_LIMITLESS = UINT_MAX;

DebugDrawSphere::DebugDrawSphere(
    CrVector3   pos,
    float       size,
    COLORf      color,
    UINT        live_time,  //出現時間(LIVETIME_LIMITLESS を指定すると消えない)
    CrVector3   move //移動ベクトル
    ) :
    m_Color(color.r, color.g, color.b, color.a), 
    m_Livetime(live_time),
    m_Move(move),
    m_FirstDraw(false)
{
    m_pMesh = DefResource.Get(Resource::MeshType::Sphere);

    D3DXMatrixScaling(&m_TransMat, size, size, size);
    m_TransMat._41 = pos.x;
    m_TransMat._42 = pos.y;
    m_TransMat._43 = pos.z;

}

DebugDrawSphere::~DebugDrawSphere()
{

}


bool DebugDrawSphere::Update()
{
    //変換行列の位置情報に移動ベクトルを加算
    m_TransMat._41 += m_Move.x;
    m_TransMat._42 += m_Move.y;
    m_TransMat._43 += m_Move.z;

    //消滅時間カウントを更新
    if (m_Livetime != LIVETIME_LIMITLESS &&
        m_Livetime > 0)
    {
        --m_Livetime;
    }

    //生存時間が０以上でtrue
    return !(m_FirstDraw&&m_Livetime == 0);
}

bool DebugDrawSphere::Msg(MsgType mt)
{
    return false;
}

void DebugDrawSphere::CalcZ()
{
    m_SortZ = DefCamera.GetCameraZ(Vector3(m_TransMat._41, m_TransMat._42, m_TransMat._43));
}

void DebugDrawSphere::Render()
{
    m_FirstDraw = true;

    m_pMesh->TransMatrix = m_TransMat;

    shader->SetValue("g_Color", m_Color);

    m_pMesh->Render(shader,"ColorOnly");
}




//------------------------------------------------------------------------------


const UINT DebugDrawPole::LIVETIME_LIMITLESS = UINT_MAX;

DebugDrawPole::DebugDrawPole(
    CrVector3   pos1,
    CrVector3   pos2,
    float       width,
    COLORf      color,
    UINT        live_time,  //出現時間(LIVETIME_LIMITLESS を指定すると消えない)
    CrVector3   move //移動ベクトル
    ) :
    m_Color(color.r, color.g, color.b, color.a),
    m_Livetime(live_time),
    m_Move(move),
    m_FirstDraw(false)
{
    m_pMesh = DefResource.Get(Resource::MeshType::Pole);

    Vector3 f, r, u;

    u = pos2 - pos1;

    Vector3Cross(r, u, Vector3(0, 1, 0));

    if (r == Vector3Zero)
    {
        Vector3Cross(r, u, Vector3(0, 0, 1));
    }

    Vector3Cross(f, r, u);

    f.Normalize();
    r.Normalize();

    r *= width;
    f *= width;

    D3DXMatrixIdentity(&m_TransMat);

    m_TransMat._11 = r.x;
    m_TransMat._12 = r.y;
    m_TransMat._13 = r.z;

    m_TransMat._21 = u.x;
    m_TransMat._22 = u.y;
    m_TransMat._23 = u.z;

    m_TransMat._31 = f.x;
    m_TransMat._32 = f.y;
    m_TransMat._33 = f.z;

    m_TransMat._41 = pos1.x;
    m_TransMat._42 = pos1.y;
    m_TransMat._43 = pos1.z;
}

DebugDrawPole::~DebugDrawPole()
{

}


bool DebugDrawPole::Update()
{
    //変換行列の位置情報に移動ベクトルを加算
    m_TransMat._41 += m_Move.x;
    m_TransMat._42 += m_Move.y;
    m_TransMat._43 += m_Move.z;

    //消滅時間カウントを更新
    if (m_Livetime != LIVETIME_LIMITLESS &&
        m_Livetime > 0)
    {
        --m_Livetime;
    }

    //生存時間が０以上でtrue
    return !(m_FirstDraw&&m_Livetime == 0);
}

bool DebugDrawPole::Msg(MsgType mt)
{
    return false;
}

void DebugDrawPole::CalcZ()
{
    m_SortZ = DefCamera.GetCameraZ(Vector3(m_TransMat._41, m_TransMat._42, m_TransMat._43));
}

void DebugDrawPole::Render()
{
    m_FirstDraw = true;

    m_pMesh->TransMatrix = m_TransMat;

    shader->SetValue("g_Color", m_Color);

    m_pMesh->Render(shader, "ColorOnly");
}




//------------------------------------------------------------------------------


const UINT DebugDrawLine::LIVETIME_LIMITLESS = UINT_MAX;

DebugDrawLine::DebugDrawLine(
    CrVector3   pos1,
    CrVector3   pos2,
    float       width,
    COLORf      color,
    UINT        live_time  //出現時間(LIVETIME_LIMITLESS を指定すると消えない)
    ) :
    m_Color(color.toDWORD()),
    m_Livetime(live_time),
    m_FirstDraw(false),
    m_Pos1(pos1),
    m_Pos2(pos2),
    m_Width(width)
{

}

DebugDrawLine::~DebugDrawLine()
{

}


bool DebugDrawLine::Update()
{
    //消滅時間カウントを更新
    if (m_Livetime != LIVETIME_LIMITLESS &&
        m_Livetime > 0)
    {
        --m_Livetime;
    }

    //生存時間が０以上でtrue
    return !(m_FirstDraw&&m_Livetime == 0);
}

bool DebugDrawLine::Msg(MsgType mt)
{
    return false;
}

void DebugDrawLine::CalcZ()
{
    m_SortZ = DefCamera.GetCameraZ(m_Pos1);
}

void DebugDrawLine::Render()
{
    m_FirstDraw = true;

    LVERTEX v[4];

    for (int i = 0; i < 4; ++i)
    {
        v[i].tu = 0;
        v[i].tv = 0;
        v[i].color = m_Color;
    }


    {
        Vector3 cr;

        Vector3Cross(cr, DefCamera.GetForward(), m_Pos1 - m_Pos2);
        cr.Normalize();
        cr *= m_Width;

        v[0].pos = m_Pos1 + cr;
        v[1].pos = m_Pos1 - cr;

        v[2].pos = m_Pos2 + cr;
        v[3].pos = m_Pos2 - cr;

    }

    iexSystem::Device->SetRenderState(D3DRS_ZENABLE, FALSE);

    iexPolygon::Render3D(
        v,
        2,
        null,
        RS_COPY
        );

    iexSystem::Device->SetRenderState(D3DRS_ZENABLE, TRUE);
}
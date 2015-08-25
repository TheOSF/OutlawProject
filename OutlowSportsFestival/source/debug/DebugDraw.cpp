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
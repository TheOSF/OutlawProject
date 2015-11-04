#include "RiverObject.h"
#include "../GameSystem/ResourceManager.h"



RiverObject::RiverObject(
    CrVector3   pos,
    CrVector3   scale,
    CrVector3   angle,
    CrVector2   speed
    ):
    m_Speed(speed),
    m_DiffTex(0,0)
{
    Matrix T;

    m_pMesh = DefResource.Get(Resource::MeshType::River);

    D3DXMatrixIdentity(&m_Mat);

    D3DXMatrixRotationYawPitchRoll(&m_Mat, angle.x, angle.y, angle.z);
    D3DXMatrixScaling(&T, scale.x, scale.y, scale.z);

    m_Mat *= T;

    m_Mat._41 = pos.x;
    m_Mat._42 = pos.y;
    m_Mat._43 = pos.z;

    m_pTexture = new iex2DObj("DATA\\Stages\\Stage1\\kasenziki.png");
}

RiverObject::~RiverObject()
{
    delete m_pTexture;
}


void RiverObject::CalcZ()
{
    m_SortZ = 0;
}

void RiverObject::Render()
{
    m_pMesh->TransMatrix = m_Mat;

    float DiffTex[2] = { m_DiffTex.x, m_DiffTex.y };

    shader->SetValue("g_Color", Vector4(0.125f, 0.25f, 0.5f, 0.6f));
    shader->SetValue("g_DiffTex", DiffTex, 2);
    

    
    m_pMesh->Render(
        shader,
        "Water"
        );

}

bool RiverObject::Update()
{
    m_DiffTex += m_Speed;

    if (m_DiffTex.x > 1.0f)
    {
        m_DiffTex.x -= 1.0f;
    }

    if (m_DiffTex.y > 1.0f)
    {
        m_DiffTex.y -= 1.0f;
    }

    return true;
}

bool RiverObject::Msg(MsgType mt)
{

    return false;
}
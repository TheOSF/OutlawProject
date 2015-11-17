#include "RiverObject.h"
#include "../GameSystem/ResourceManager.h"



RiverObject::RiverObject(
    CrVector3   pos,
    CrVector3   scale,
    CrVector3   angle,
    CrVector2   speed
    ):
    m_Speed(speed),
    m_DiffTex(0,0),
    m_DiffTex2(0.2f,0.2f)
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
    m_SortZ = 1000;
}

void RiverObject::Render()
{
    m_pMesh->TransMatrix = m_Mat;

    float DiffTex[2]  = { m_DiffTex.x, m_DiffTex.y };
    float DiffTex2[2] = { m_DiffTex2.x, m_DiffTex2.y };

    shader->SetValue("g_Color", Vector4(0.05f, 0.3f, 0.4f, 0.9f));
    shader->SetValue("g_DiffTex", DiffTex, 2);
    shader->SetValue("g_DiffTex2", DiffTex2, 2);
    
    m_pMesh->Render(
        shader,
        "Water"
        );

}

bool RiverObject::Update()
{
    UpdateTex(m_DiffTex,m_Speed);
    UpdateTex(m_DiffTex2, m_Speed*0.25f);

    return true;
}

void RiverObject::UpdateTex(Vector2& val, CrVector2 Speed)
{
    const float m = 1000.0f;
    val += Speed;

    while (val.x > m)
    {
        val.x -= m;
    }

    while (val.y > m)
    {
        val.y -= m;
    }

}

bool RiverObject::Msg(MsgType mt)
{

    return false;
}
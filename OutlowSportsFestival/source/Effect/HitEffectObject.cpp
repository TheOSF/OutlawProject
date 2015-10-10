#include "HitEffectObject.h"
#include "../GameSystem/ResourceManager.h"
#include "ParticleRenderer.h"
#include "ParticleMoveObject.h"
#include "../GameSystem/ResourceManager.h"
#include "GlavityLocus.h"


HitEffectObject::HitEffectObject(
    CrVector3 pos,
    CrVector3 vec,
    float     length,
    float     width,
    Vector3   color,
    UINT      level,
    float     LightSize,
    UINT      frame 
    ) :
    m_Renderer(DefResource.Get(Resource::MeshType::HitEffect), false),
    m_Color(color),
    m_T(0),
    m_Particle_level(level),
    m_Frame(frame)
{
    InitMatrix(pos, vec, length, width);

    //SetParticle(
    //    pos, vec, length, width, color
    //    );

    SetGlLocus(
        pos, vec, color
        );

    m_Renderer.m_Color.SetColor(0, 0, 0, 0);

    //ƒ‰ƒCƒg
    m_Light.param.color = color*0.25f;
    m_Light.param.pos = pos;
    m_Light.param.size = LightSize;
}

HitEffectObject::~HitEffectObject()
{

}

void HitEffectObject::InitMatrix(
    CrVector3 pos,
    CrVector3 vec,
    float     length,
    float     width
    )
{
    Matrix m;
    const float DefaultScale = 0.025f;
    Vector3 r(1, 0, 0), u(vec), f;

    Vector3Cross(f, r, u);

    if (f.x == 0 &&
        f.y == 0 &&
        f.z == 0)
    {
        Vector3Cross(f, Vector3(0, 0, 1), u);
    }

    Vector3Cross(r, u, f);

    r.Normalize();
    u.Normalize();
    f.Normalize();

    f *= width*DefaultScale;
    r *= width*DefaultScale;
    u *= length*DefaultScale;

    D3DXMatrixTranslation(&m, pos.x, pos.y, pos.z);

    m._11 = r.x;
    m._12 = r.y;
    m._13 = r.z;

    m._21 = u.x;
    m._22 = u.y;
    m._23 = u.z;

    m._31 = f.x;
    m._32 = f.y;
    m._33 = f.z;

    m_Renderer.SetMatrix(m);

}

void HitEffectObject::SetParticle(
    CrVector3 pos,
    CrVector3 vec,
    float     length,
    float     width,
    Vector3   color
    )
{
    ParticleRenderer* Renderer;
    ParticleMoveObject* MoveObj;

    const int numParticle = 10 * (int)(1 + m_Particle_level);
    
    COLORf Colorf(1, color.x, color.y, color.z);
    LPIEX2DOBJ pTexture = DefResource.Get(Resource::TextureType::Particle);

    Vector3 moveVec = Vector3Normalize(vec)*0.6f;

    for (int i = 0; i < numParticle; ++i)
    {
        Renderer = new ParticleRenderer();
        
        Renderer->m_Param.color = Colorf;
        Renderer->m_Param.dw_Flag = RS_ADD;
        Renderer->m_Param.pos = pos;
        Renderer->m_Param.size.x = 0.125f;
        Renderer->m_Param.size.y = 0.125f;
        Renderer->SetCellUV(4, 4, 0);
        Renderer->m_pTexture = pTexture;

        MoveObj = new ParticleMoveObject(
            Renderer,
            moveVec + Vector3Rand()*0.4f,
            Vector3Zero,
            25,
            false,
            0,
            0
            );
    }
}


void HitEffectObject::SetGlLocus(
    CrVector3 pos,
    CrVector3 vec,
    Vector3   color
    )
{
    Vector3 power(0, -0.01f, 0);
    GlavityLocus* g;

    const Vector4 
        stCol(color.x, color.y, color.z, 0.3f),
        endCol(color.x, color.y, color.z, 0);

    const Vector4
        stHdCol(color.x, color.y, color.z, 5.0f),
        endHdCol(color.x, color.y, color.z, 0);

    const Vector3 move = Vector3Normalize(vec)*0.8f;


    for (int i = 0; i < (int)m_Particle_level * 5; ++i)
    {
        g = new GlavityLocus(
            pos, move + Vector3Rand() * 0.4f, power, 4, 40 + rand() % 30
            );

        g->m_BoundRatio = 0.8f;
        g->m_CheckWall = false;

        g->m_Locus.m_StartParam.Color = stCol;
        g->m_Locus.m_EndParam.Color = endCol;

        g->m_Locus.m_StartParam.HDRColor = stHdCol;
        g->m_Locus.m_EndParam.HDRColor = endHdCol;

        g->m_Locus.m_StartParam.Width = 0.05f;
        g->m_Locus.m_EndParam.Width = 0.00f;

        g->m_Locus.m_pTexture = DefResource.Get(Resource::TextureType::Locus1);
    }
    
}


void HitEffectObject::UpdateMatrix()
{
    Matrix m = m_Renderer.GetMatrix();
    const float UpScale = 0.35f;
    const float WidthUp = 0.75f;

    m._11 *= 1 + UpScale*WidthUp;
    m._12 *= 1 + UpScale*WidthUp;
    m._13 *= 1 + UpScale*WidthUp;

    m._21 *= 1 + UpScale;
    m._22 *= 1 + UpScale;
    m._23 *= 1 + UpScale;

    m._31 *= 1 + UpScale*WidthUp;
    m._32 *= 1 + UpScale*WidthUp;
    m._33 *= 1 + UpScale*WidthUp;

    m_Renderer.SetMatrix(m);
}

bool HitEffectObject::Update()
{
    const float Speed = 1 / (float)m_Frame;
    m_T += Speed;

    UpdateMatrix();

    m_Renderer.m_HDR_Color = m_Color * (1 - m_T);

    m_Light.param.color *= 0.9f;

    return m_T < 1.0f;
}

bool HitEffectObject::Msg(MsgType mt)
{
    return false;
}
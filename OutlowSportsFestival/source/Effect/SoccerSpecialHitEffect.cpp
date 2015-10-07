#include "SoccerSpecialHitEffect.h"
#include "../GameSystem/ResourceManager.h"
#include "ParticleRenderer.h"
#include "ParticleMoveObject.h"
#include "../GameSystem/ResourceManager.h"
#include "HitEffectObject.h"

SoccerSpecialHitEffect::SoccerSpecialHitEffect(
    CrVector3 pos,
    CrVector3 vec,
    Vector3   color,
    UINT      level
    ) :
    m_Renderer(DefResource.Get(Resource::MeshType::Soccer_Special_Hit), false),
    m_Color(color),
    m_T(0),
    m_Particle_level(level),
    m_Pos(pos),
    m_Vec(vec),
    m_Timer(0)
{
    InitMatrix(pos, vec, 1, 1);

    m_Renderer.m_Color.SetColor(0, 0, 0, 0);

    //ライト
    m_Light.param.color = color*0;
    m_Light.param.pos = pos;
    m_Light.param.size = 50.0f;
}

SoccerSpecialHitEffect::~SoccerSpecialHitEffect()
{

}

void SoccerSpecialHitEffect::InitMatrix(
    CrVector3 pos,
    CrVector3 vec,
    float     length,
    float     width
    )
{
    Matrix m;
    const float DefaultScale = 0.015f;
    Vector3 r, u(0,1,0), f(vec);

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

void SoccerSpecialHitEffect::SetParticle(
    CrVector3 pos,
    CrVector3 vec,
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
        Renderer->m_Param.size.x = 0.2f;
        Renderer->m_Param.size.y = 0.2f;
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

void SoccerSpecialHitEffect::UpdateMatrix()
{
    Matrix m = m_Renderer.GetMatrix();
    const float UpScale = 0;
    const float WidthUp = 0;

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

bool SoccerSpecialHitEffect::Update()
{
    const float Speed = 1 / 60.0f;
    m_T += Speed;

    UpdateMatrix();

    m_Renderer.m_HDR_Color = m_Color * (1 - m_T);

    m_Light.param.color *= 0.9f;


    if (++m_Timer == 15)
    {
     /*   SetParticle(
            m_Pos, m_Vec, m_Color
            );*/

        //ヒットエフェクト作成
        new HitEffectObject(
            m_Pos + Vector3(0, 3, 0),
            m_Vec,
            0.1f,
            0.2f,
            m_Color,
            0,
            20,
            9
            );
    }

    return m_T < 1.0f;
}

bool SoccerSpecialHitEffect::Msg(MsgType mt)
{
    return false;
}
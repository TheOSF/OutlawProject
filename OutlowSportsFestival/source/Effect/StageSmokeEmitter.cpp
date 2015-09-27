#include "StageSmokeEmitter.h"
#include "ParticleRenderer.h"
#include "ParticleMoveObject.h"
#include "../GameSystem/ResourceManager.h"


StageSmokeEmitter::StageSmokeEmitter(
    CrVector3 minPos,     
    CrVector3 maxPos,
    DWORD     Color,
    float     EmitteTime,
    float     Random
    ) :
    m_MinPos(minPos),
    m_MaxPos(maxPos),
    m_Color(Color),
    m_EmitteTime(EmitteTime),
    m_Random(Random),
    m_Count(0)
{
    
}

StageSmokeEmitter::~StageSmokeEmitter()
{

}

bool StageSmokeEmitter::Update()
{
    //出現チェック
    if (m_Count >= m_EmitteTime)
    {
        //カウント初期化
        m_Count = (frand() - 0.5f)*2.0f * m_Random;

        //出現
        {
            Vector3 pos;

            //範囲のベクトルを算出
            pos = m_MaxPos - m_MinPos;

            //ランダムな大きさに
            pos.x *= frand();
            pos.y *= frand();
            pos.z *= frand();

            //最小地点を加算
            pos += m_MinPos;

            ParticleRenderer* r = new ParticleRenderer();

            r->m_pTexture = DefResource.Get(Resource::TextureType::Particle);
            r->m_SoftEnable = true;
            r->m_SoftDepth = 0.01f;
            r->m_Param.color = m_Color & 0x00FFFFFF;
            r->m_Param.dw_Flag = RS_COPY;
            r->m_Param.pos = pos;
            r->m_Param.size = Vector2(5, 5);
            r->SetCellUV(4, 4, 15);

            ParticleMoveObject2* m = new ParticleMoveObject2(
                r,
                Vector3Normalize(Vector3(frand() - 0.5f, 0, frand() - 0.5f))*0.05f, 
                Vector3Zero,
                false,
                1,
                1,

                120,
                240,

                m_Color,
                m_Color & 0x00FFFFFF
                
                );

        }
    }

    //カウント更新
    m_Count += 1.0f;

    return  true;
}

bool StageSmokeEmitter::Msg(MsgType mt)
{
    return false;
}
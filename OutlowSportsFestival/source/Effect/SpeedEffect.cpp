#include "SpeedEffect.h"
#include "GlavityLocus.h"

SpeedEffect::SpeedEffect(RATIO level):
m_Count(0),
m_Level(level)
{

}

SpeedEffect::~SpeedEffect()
{

}

void SpeedEffect::Update(CrVector3 Pos, CrVector3 Move)
{
    
    for (int i = 0; i < 2; ++i)
    {
        m_Count = 0;

        GlavityLocus *p =
            new GlavityLocus(
            Pos + Vector3Rand()*1.25f,
            Move,
            Vector3Zero,
            8,
            11
            );

        p->m_Locus.m_StartParam.Color = Vector4(1,1,1, 0.8f);
        p->m_Locus.m_EndParam.Color   = Vector4(1,1,1, 0.8f);

        p->m_Locus.m_StartParam.HDRColor = Vector4(0, 0, 0, 0);
        p->m_Locus.m_EndParam.HDRColor = Vector4(0, 0, 0, 0);

        p->m_Locus.m_StartParam.Width = 0.04f;
        p->m_Locus.m_EndParam.Width = 0.04f;
    }
}
#include "WindBallEffect.h"



WindBallEffect::WindBallEffect(
    UINT     num_locus,
    UINT     locus_len,
    float    locus_width,
    float    set_width,
    RADIAN   rotate_speed
    ):
    m_NumLocus(num_locus),
    m_LocusPoint(locus_len),
    m_SetWidth(set_width),
    m_Angle(0),
    m_RotateSpeed(rotate_speed)
{
    m_ppLocusPtrArray = new LocusHDR*[m_NumLocus];

    for (UINT i = 0; i < m_NumLocus; ++i)
    {
        m_ppLocusPtrArray[i] = new LocusHDR(m_LocusPoint);

        m_ppLocusPtrArray[i]->m_EndParam.Width = locus_width;
        m_ppLocusPtrArray[i]->m_EndParam.Color = Vector4(1, 1, 1, 0.0f);
        m_ppLocusPtrArray[i]->m_EndParam.HDRColor = Vector4(1, 1, 1, 0.0f);

        m_ppLocusPtrArray[i]->m_StartParam.Width = 0;
        m_ppLocusPtrArray[i]->m_StartParam.Color = Vector4(1, 1, 1, 1);
        m_ppLocusPtrArray[i]->m_StartParam.HDRColor = Vector4(1, 1, 1, 1);
    }
}


WindBallEffect::~WindBallEffect()
{
    for (UINT i = 0; i < m_NumLocus; ++i)
    {
        delete m_ppLocusPtrArray[i];
    }

    delete[] m_ppLocusPtrArray;
}

void WindBallEffect::Update(CrVector3 pos, CrVector3 move)
{
    const RADIAN one_rotate_locus = PI*2.0f / (float)m_NumLocus; //‚P‹OÕ‚ ‚½‚è‚Ì‰ñ“]
    
    D3DXQUATERNION locus_rotate_qua;
    Vector3 rotate_vec, locus_pos, locus_vec;
    
    D3DXVECTOR3 axis(move.x, move.y, move.z);

    D3DXVec3Normalize(&axis, &axis);
    D3DXQuaternionRotationAxis(&locus_rotate_qua, &axis, one_rotate_locus);


    Vector3Cross(rotate_vec, move, Vector3AxisY);

    if (rotate_vec == Vector3Zero)
    {
        Vector3Cross(rotate_vec, move, Vector3AxisZ);
    }

    rotate_vec.Normalize();


    {
        //oŒ»ˆÊ’u‚ð‚ ‚ç‚©‚¶‚ß‰ñ“]
        rotate_vec = Vector3RotateAxis(Vector3Normalize(move), m_Angle, rotate_vec);
    }


    //“_‚ð’Ç‰Á
    for (UINT i = 0; i < m_NumLocus; ++i)
    {
        rotate_vec = Vector3RotateQuaternion(locus_rotate_qua, rotate_vec);

        locus_pos = rotate_vec + pos;
        Vector3Cross(locus_vec, move, rotate_vec);

        locus_vec.Normalize();

        m_ppLocusPtrArray[i]->AddPoint(locus_pos, locus_vec);
    }


    m_Angle += m_RotateSpeed;

}
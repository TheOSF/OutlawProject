#pragma once

#include "../utillity/LocusHDR.h"

//---------------------------------------------------------//
//  �@�@�{�[�����܂Ƃ����G�t�F�N�g�����N���X
//---------------------------------------------------------//

class WindBallEffect
{
public:
    WindBallEffect(
        UINT     num_locus,
        UINT     locus_len,
        float    locus_width,
        float    set_width,
        RADIAN   rotate_speed
        );

    ~WindBallEffect();

    void Update(CrVector3 pos, CrVector3 move);

private:
    const UINT    m_NumLocus;
    const UINT    m_LocusPoint;
    const float   m_SetWidth;

    LocusHDR**    m_ppLocusPtrArray;
    RADIAN        m_Angle;
    RADIAN        m_RotateSpeed;
};
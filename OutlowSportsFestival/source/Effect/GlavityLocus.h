#pragma once
#include "../GameSystem/GameObject.h"
#include "../utillity/LocusHDR.h"
#include "../utillity/Locus.h"
//------------------------------------------------//
//  �@�@�@�@�d�͂ňړ�����O�ՃN���X
//------------------------------------------------//

class GlavityLocus :public GameObjectBase
{
public:

    GlavityLocus(
        CrVector3 pos,         //�����ʒu
        CrVector3 vec,         //����
        CrVector3 power,       //�͏�
        UINT      locus_len,   //�O�Ղ̒���
        UINT      live_frame   //����
        );

    ~GlavityLocus();


    bool    m_CheckWall;    //�ǂƂ̔�����Ƃ邩�ǂ���
    RATIO   m_BoundRatio;   //�o�E���h���闦

    LocusHDR m_Locus;        //�O�ՃN���X

private:

    const UINT     m_LiveFrame;
    UINT           m_LiveCount;
    const Vector3  m_Power;
    Vector3        m_Pos, 
                   m_Move;

    float          m_InitStartAlpha;
    float          m_InitEndAlpha;

    float          m_InitStartHDRAlpha;
    float          m_InitEndHDRAlpha;

    bool Update();
    bool Msg(MsgType mt);

};
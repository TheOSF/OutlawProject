#include "GlavityLocus.h"
#include "../Collision/Collision.h"
#include "../Camera/Camera.h"


GlavityLocus::GlavityLocus(
    CrVector3 pos,         //�����ʒu
    CrVector3 vec,         //����
    CrVector3 power,       //�͏�
    UINT      locus_len,   //�O�Ղ̒���
    UINT      live_frame   //����
    ) :
    m_Locus(locus_len),
    m_CheckWall(false),
    m_BoundRatio(0.5f),
    m_LiveFrame(live_frame),
    m_LiveCount(0),
    m_Power(power),
    m_Pos(pos),
    m_Move(vec),
    m_InitStartAlpha(-10)
{
    
}

GlavityLocus::~GlavityLocus()
{

}



bool GlavityLocus::Update()
{
    //�����J���[��ϐ��ɓo�^
    if (m_InitStartAlpha < -5)
    {
        m_InitStartAlpha = m_Locus.m_StartParam.Color.w;
        m_InitEndAlpha = m_Locus.m_EndParam.Color.w;

        m_InitStartHDRAlpha = m_Locus.m_StartParam.HDRColor.w;
        m_InitEndHDRAlpha = m_Locus.m_EndParam.HDRColor.w;

    }

    {
        const float t = 1 - ((float)m_LiveCount / (float)m_LiveFrame);

        m_Locus.m_StartParam.Color.w = m_InitStartAlpha * t;
        m_Locus.m_EndParam.Color.w = m_InitEndAlpha * t;


        m_Locus.m_StartParam.HDRColor.w = m_InitStartHDRAlpha * t;
        m_Locus.m_EndParam.HDRColor.w = m_InitEndHDRAlpha * t;
    }

    //�ǃ`�F�b�N
    if (m_CheckWall)
    {
        Vector3 out, pos(m_Pos), vec(Vector3Normalize(m_Move));
        float dist = m_Move.Length()*2.0f;
        int material;

        if (DefCollisionMgr.RayPick(
            &out, &pos, &vec, &dist, &material, CollisionManager::RayType::_Usual
            ))
        {
            m_Move = Vector3Refrect(m_Move, vec)*m_BoundRatio;
        }
    }

    //���`�F�b�N
    if (m_CheckWall == false)
    {
        if (m_Pos.y < 0 )
        {
            m_Pos.y = 0;
            m_Move.y = -m_Move.y * m_BoundRatio;
        }
    }

    //�ړ�
    {
        m_Move += m_Power;
        m_Pos += m_Move;
    }

    //�O�Ղ̓_���X�V
    {
        Vector3 c;

        Vector3Cross(c, m_Move, DefCamera.GetForward());

        c.Normalize();

        m_Locus.AddPoint(m_Pos, c);

    }

    return ++m_LiveCount <= m_LiveFrame;
}

bool GlavityLocus::Msg(MsgType mt)
{
    return false;
}
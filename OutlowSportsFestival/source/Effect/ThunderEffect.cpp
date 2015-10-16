#include "ThunderEffect.h"
#include "../Camera/Camera.h"

ThunderEffect::ThunderEffect(
    CrVector3 origin,     //打つ始点
    CrVector3 target,     //終点
    float     random,     //横へのランダムな、ばらけ度
    float     width,      //太さ
    float     live_time,  //雷の寿命
    Vector4   color,      //色
    UINT      point_num   //折れ曲がれる最大数、多いと重いので20程度がオススメ
    ) :
    m_LiveTime(live_time),
    m_LiveCount(live_time),
    m_Locus(point_num),
    m_Point_num(point_num),
    m_Origin(origin),
    m_Target(target),
    m_Random(random)
{

    m_Locus.m_StartParam.Color = Vector4(1, 1, 1, 1);
    m_Locus.m_StartParam.HDRColor = color;
    m_Locus.m_StartParam.Width = width;

    m_Locus.m_EndParam.Color = Vector4(1, 1, 1, 1);
    m_Locus.m_EndParam.HDRColor = color;
    m_Locus.m_EndParam.Width = width;

    SetNewPos();

    m_MoveVec = Vector3Rand()*0.0f;
}


ThunderEffect::~ThunderEffect()
{

}

void ThunderEffect::SetNewPos()
{
    const Vector3 camera_front = DefCamera.GetForward();
    Vector3
        pre_pos = m_Origin,
        pre_vec = m_Target - m_Origin,
        pos,
        vec,
        nowToTarget;

    pre_vec.Normalize();

    m_Locus.Clear();

    //軌跡を配置
    for (UINT i = 0; i < m_Point_num; ++i)
    {
        //到達点へのベクトル
        nowToTarget = m_Target - pos;

        if (i % 2 == 0)
        {
            vec = Vector3Rand()*2.0f + Vector3Normalize(nowToTarget);

            vec.Normalize();
            vec *= frand() * m_Random;
        }
        else
        {
            vec = nowToTarget;

            if (vec.Length() < m_Random)
            {
                break;
            }

            vec.Normalize();
            vec *= frand() * m_Random;
        }

        pos = pre_pos + vec; // *(frand() * m_Size.x);

        pre_vec = vec;
        pre_pos = pos;

        Vector3Cross(vec, pre_vec, camera_front);

        vec.Normalize();

        m_Locus.AddPoint(
            pos,
            vec
            );
    }

}

void ThunderEffect::UpdatePos()
{
    const Vector3 camera_front = DefCamera.GetForward();
    Vector3
        pre_pos = m_Origin,
        pos,
        vec;

    //軌跡を配置
    for (UINT i = 0; i < m_Point_num; ++i)
    {
        vec = Vector3Normalize(m_MoveVec + Vector3Rand()*0.2f);
        
        m_Locus.GetPos(m_Point_num - 2, pos);


        pos += vec;


        Vector3Cross(vec, pos-pre_pos, camera_front);

        vec.Normalize();

        m_Locus.AddPoint(
            pos,
            vec
            );

        pre_pos = pos;
    }
}

bool ThunderEffect::Update()
{
  
    if (true)
    {
        //新しい形状を生成
        SetNewPos();

        //移動ベクトル初期化
        m_MoveVec = Vector3Rand()*0.0f;

        //見えないように
       // m_Locus.m_Visible = false;
    }
    else
    {
        //たまに隠れる用に
        m_Locus.m_Visible = true;//(rand() % 5 != 0);

        //すでにある形を変形
        UpdatePos();
    }
    
    //色調整
    {
        const float Alpha = m_LiveCount / m_LiveTime;

        m_Locus.m_StartParam.Color.w = Alpha;
        m_Locus.m_StartParam.HDRColor.w = Alpha;

        m_Locus.m_EndParam.Color.w = Alpha;
        m_Locus.m_EndParam.HDRColor.w = Alpha;

    }

    m_LiveCount -= 1.0f;



    return m_LiveCount > 0;
}

bool ThunderEffect::Msg(MsgType mt)
{
    return false;
}
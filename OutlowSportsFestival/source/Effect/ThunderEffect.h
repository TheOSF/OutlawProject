#pragma once

#include "../GameSystem/GameObject.h"
#include "../utillity/LocusHDR.h"

//---------------------------------------------------
// 　　　　雷エフェクト
//--------------------------------------------------- 

class ThunderEffect :public GameObjectBase
{
public:
    ThunderEffect(
        CrVector3 origin,     //打つ始点
        CrVector3 target,     //終点
        float     random,     //横へのランダムな、ばらけ度
        float     width,      //太さ
        float     live_time,  //雷の寿命
        Vector4   color,      //色
        UINT      point_num   //折れ曲がれる最大数、多いと重いので20程度がオススメ
        );

    ~ThunderEffect();

private:
    const float   m_LiveTime;
    float         m_LiveCount;
    LocusHDR      m_Locus;
    const UINT    m_Point_num;
    const Vector3 m_Origin, m_Target;
    const float   m_Random;
    Vector3       m_MoveVec;

    void SetNewPos();
    void UpdatePos();

    bool Update();
    bool Msg(MsgType mt);
};
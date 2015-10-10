#pragma once
#include "../GameSystem/GameObject.h"
#include "../utillity/LocusHDR.h"
#include "../utillity/Locus.h"
//------------------------------------------------//
//  　　　　重力で移動する軌跡クラス
//------------------------------------------------//

class GlavityLocus :public GameObjectBase
{
public:

    GlavityLocus(
        CrVector3 pos,         //初期位置
        CrVector3 vec,         //方向
        CrVector3 power,       //力場
        UINT      locus_len,   //軌跡の長さ
        UINT      live_frame   //寿命
        );

    ~GlavityLocus();


    bool    m_CheckWall;    //壁との判定をとるかどうか
    RATIO   m_BoundRatio;   //バウンドする率

    LocusHDR m_Locus;        //軌跡クラス

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
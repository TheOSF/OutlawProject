#pragma once

#include "../GameSystem/GameObject.h"

//---------------------------------------------------------//
// 　ステージの煙パーティクルを出現させるクラス
//---------------------------------------------------------//

class StageSmokeEmitter :public GameObjectBase
{
public:
    StageSmokeEmitter(
        CrVector3 minPos,     //ＡＡＢＢの座標がもっとも小さい地点
        CrVector3 maxPos,     //ＡＡＢＢの座標がもっとも大きい地点
        DWORD     Color,      //煙の色
        float     EmitteTime, //出現頻度
        float     Random      //出現の時間の誤差
        );

    ~StageSmokeEmitter();

    bool Update();
    bool Msg(MsgType mt);

private:
    const Vector3   m_MinPos;
    const Vector3   m_MaxPos;
    const DWORD     m_Color;
    const float     m_EmitteTime;
    const float     m_Random;

    float           m_Count;
};
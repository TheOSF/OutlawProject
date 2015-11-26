#pragma once

//------------------------------------------------------//
//  高速移動エフェクト
//------------------------------------------------------//

class SpeedEffect
{
public:
    SpeedEffect(RATIO level);
    ~SpeedEffect();

    void Update(CrVector3 Pos, CrVector3 Move);
private:
    const RATIO m_Level;
    int m_Count;
};
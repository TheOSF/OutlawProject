#pragma once

//------------------------------------------------------//
//  �����ړ��G�t�F�N�g
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
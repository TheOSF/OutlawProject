#pragma once

#include "../../GameSystem/GameObject.h"
#include "BaseballPlayer.h"
#include "../../Render/MeshRenderer.h"

//--------------------------------------------------------//
//  　　　　　　　ピッチングマシーン
//--------------------------------------------------------//

class PitchingMachine : public GameObjectBase
{
public:

    PitchingMachine(
        BaseballPlayer*     pChr,
        CrVector3           pos,
        CrVector3           vec
        );

    ~PitchingMachine();


    bool Update();
    bool Msg(MsgType mt);

private:

    BaseballPlayer* const       m_pChr;
    MeshRenderer*               m_pMachineMesh;
    MeshRenderer*               m_pArmMesh;

    Vector3                     m_Pos, m_Vec;
    RADIAN                      m_ArmAngle;

    void UpdateMesh();
};
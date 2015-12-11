#pragma once
//TennisHeartBall

#include "../../Ball/Ball.h"
#include "../../GameSystem/GameObject.h"
#include "../../GameSystem/ForwardDecl.h"
#include "../../GameSystem/GameSystem.h"
#include "../../Damage/Damage.h"
#include "../../utillity/LocusHDR.h"
#include "../../Render/LightObject.h"
#include "TennisPlayer.h"


//*****************************************************
//		テニスハートボールクラス
//*****************************************************

class TennisHeartBall :public GameObjectBase, public BallBase
{
public:
    TennisHeartBall(
        CrVector3 pos,
        CrVector3 move,
        TennisPlayer* pTennis
        );

    ~TennisHeartBall();

    bool Update();
    bool Msg(MsgType mt);

private:
    MeshRenderer*       m_pMesh;
    TennisPlayer* const m_pTennis;
    const float         m_InitMove;
    RADIAN              m_MoveCount;
    bool                m_isLive;
    int                 m_LiveCount;
    DamageShpere        m_Damage;
    float               m_ScaleCount;
    float               m_ScaleMove;
    Vector3             m_PrePos;

    bool GetTarget(CharacterBase** pChr);
    void UpdateMesh();
    void UpdateDamage();
    void MoveRotate(RADIAN angle);
    void Move();
    float CalcScale();
    void Effect(int Level);
    void SmokeEffect();

    void Counter(CharacterBase* pCounterCharacter);
};


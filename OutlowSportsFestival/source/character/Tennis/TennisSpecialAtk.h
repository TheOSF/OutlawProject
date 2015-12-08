#pragma once

#include "TennisPlayer.h"
#include "../../Ball/Ball.h"
#include "../../Damage/Damage.h"
#include "../../utillity/LocusHDR.h"
#include "../../Render/MeshRenderer.h"
#include "../../Effect/SpeedEffect.h"

//***************************************************
//		テニス＿必殺技用ボールクラス
//***************************************************

class TennisSpecialBall :public GameObjectBase
{
public:

    //コンストラクタ
    TennisSpecialBall(TennisPlayer* t, CrVector3 pos, CrVector3 move);
    ~TennisSpecialBall();

    bool Update();
    bool Msg(MsgType mt);

private:
    void(TennisSpecialBall::*m_pStatefunc)();
    TennisPlayer* const m_pTennis;
    LpMeshRenderer		m_pMeshRenderer;
    LocusHDR            m_Locus;
    int                 m_Timer;
    Vector3             m_Pos, m_Move;
    DamageShpere        m_Damage;

    void StateMove();
    void StateStop();
    void StateFinish();

    void UpdateMesh();
    void UpdateMove();
    void ApperEffect();
};

//***************************************************
//		テニス＿必殺技ステートクラス
//***************************************************

class TennisState_SpecialAtk : public TennisState
{
public:

    TennisState_SpecialAtk(TennisPlayer* t);
    ~TennisState_SpecialAtk();

    // ステート開始
    void Enter(TennisPlayer* t)override;

    // ステート実行
    void Execute(TennisPlayer* t)override;

    // ステート終了
    void Exit(TennisPlayer* t)override;

private:
    bool CalcToBestTargetVec(CrVector3 ShotPos,CrVector3 ShotVec, Vector3& out);

    TennisPlayer* const  m_pTennis;
    int                  m_Timer;
    SpeedEffect          m_SpeedEffect;

};


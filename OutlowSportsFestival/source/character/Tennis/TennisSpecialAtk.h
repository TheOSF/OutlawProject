#pragma once

#include "TennisPlayer.h"
#include "../../Ball/Ball.h"
#include "../../Damage/Damage.h"
#include "../../utillity/LocusHDR.h"
#include "../../Render/MeshRenderer.h"

//***************************************************
//		テニス＿必殺技用ボールクラス
//***************************************************

class TennisSpecialBall :public GameObjectBase, public BallBase
{
public:

    //コンストラクタ
    TennisSpecialBall(TennisPlayer* t, CrVector3 pos, CrVector3 move);
    ~TennisSpecialBall();

    bool Update();
    bool Msg(MsgType mt);

private:
    TennisPlayer* const m_pTennis;
    LpMeshRenderer		m_pMeshRenderer;
    DamageShpere		m_Damage;
    LocusHDR            m_Locus;
    RADIAN              m_Rotate;
    const float         m_FirstMoveVal;
    int                 m_NoDamageFrame;
    int                 m_Timer;

    void UpdateDamageClass();
    void UpdateMesh();
    void UpdateMove();
    void UpdateWallCheck();

    void Counter(CharacterBase* pCounterCharacter)override;
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

    TennisPlayer* const  m_pTennis;
    int                  m_Timer;

};


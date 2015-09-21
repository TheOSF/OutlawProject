#pragma once

#include "TennisPlayer.h"
#include "../../Ball/Ball.h"
#include "../../Damage/Damage.h"
#include "../../utillity/Locus.h"
#include "../../Render/MeshRenderer.h"

//***************************************************
//		テニス＿必殺技用ボールクラス
//***************************************************

class TennisSpecialBall :public GameObjectBase, public BallBase
{
public:

    //コンストラクタ
    TennisSpecialBall(TennisPlayer* t, BallBase::Params& param);
    ~TennisSpecialBall();

    bool Update();
    bool Msg(MsgType mt);

private:
    TennisPlayer* const m_pTennis;
    LpMeshRenderer		m_pMeshRenderer;
    DamageShpere		m_Damage;
    UINT                m_DeleteFrame;
    D3DXQUATERNION		m_Ballrot;
    Locus               m_Locus;
    Matrix              m_BaseMatrix;
    bool                m_DeleteFlag;

    bool isOutofField()const;

    void UpdateDamageClass();
    void UpdateMesh();
    void UpdateLocusColor();
    void UpdateMove();
    void UpdateWallCheck();
    void ToNoWork();

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

};


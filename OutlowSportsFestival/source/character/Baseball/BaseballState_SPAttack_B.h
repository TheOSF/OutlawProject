#pragma once

#include "BaseballPlayer.h"
#include "../../Render/LightObject.h"
#include "../../Render/MeshRenderer.h"
#include "../../utillity/LocusHDR.h"

//***************************************************
//		野球　バッター　必殺技ステートクラス
//***************************************************

class BaseballState_SPAttack_B : public BaseballState
{
public:

	BaseballState_SPAttack_B(BaseballPlayer* b);
	~BaseballState_SPAttack_B();

	// ステート開始
	void Enter(BaseballPlayer* b)override;

	// ステート実行
	void Execute(BaseballPlayer* b)override;

	// ステート終了
	void Exit(BaseballPlayer* b)override;

private:
	typedef void(BaseballState_SPAttack_B::*StateFunc)();

    static const float      m_sDamageValue;

    BaseballPlayer* const   m_pChr;
    StateFunc               m_pStateFunc;
    int                     m_StateTimer;
    DamageCapsure           m_Damage;

    MeshRenderer*           m_pBatMesh;
    Vector3                 m_BatScale;
    bool                    m_DoOnHit;

    UINT                    m_ChrLiveCount;

    LocusHDR                m_Locus;

    void SetState(StateFunc state);
    void UpdateBatMesh();
    void UpdateDamagePos();
    void OnHit();
    void AddLocusPoint();
    void BallCounterUpdate(CrVector3 pos, float size);

    void State_PreAtk();
    void State_Atk();

};


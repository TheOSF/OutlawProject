#pragma once

#include "BaseballPlayer.h"
#include "../../Render/LightObject.h"
#include "../../Render/MeshRenderer.h"
#include "../../utillity/LocusHDR.h"

//***************************************************
//		�싅�@�o�b�^�[�@�K�E�Z�X�e�[�g�N���X
//***************************************************

class BaseballState_SPAttack_B : public BaseballState
{
public:

	BaseballState_SPAttack_B(BaseballPlayer* b);
	~BaseballState_SPAttack_B();

	// �X�e�[�g�J�n
	void Enter(BaseballPlayer* b)override;

	// �X�e�[�g���s
	void Execute(BaseballPlayer* b)override;

	// �X�e�[�g�I��
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


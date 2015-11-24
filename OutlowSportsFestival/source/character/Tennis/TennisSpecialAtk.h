#pragma once

#include "TennisPlayer.h"
#include "../../Ball/Ball.h"
#include "../../Damage/Damage.h"
#include "../../utillity/LocusHDR.h"
#include "../../Render/MeshRenderer.h"

//***************************************************
//		�e�j�X�Q�K�E�Z�p�{�[���N���X
//***************************************************

class TennisSpecialBall :public GameObjectBase, public BallBase
{
public:

    //�R���X�g���N�^
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
//		�e�j�X�Q�K�E�Z�X�e�[�g�N���X
//***************************************************

class TennisState_SpecialAtk : public TennisState
{
public:

    TennisState_SpecialAtk(TennisPlayer* t);
    ~TennisState_SpecialAtk();

    // �X�e�[�g�J�n
    void Enter(TennisPlayer* t)override;

    // �X�e�[�g���s
    void Execute(TennisPlayer* t)override;

    // �X�e�[�g�I��
    void Exit(TennisPlayer* t)override;

private:

    TennisPlayer* const  m_pTennis;
    int                  m_Timer;

};


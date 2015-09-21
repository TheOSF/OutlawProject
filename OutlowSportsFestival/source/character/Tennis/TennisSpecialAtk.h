#pragma once

#include "TennisPlayer.h"
#include "../../Ball/Ball.h"
#include "../../Damage/Damage.h"
#include "../../utillity/Locus.h"
#include "../../Render/MeshRenderer.h"

//***************************************************
//		�e�j�X�Q�K�E�Z�p�{�[���N���X
//***************************************************

class TennisSpecialBall :public GameObjectBase, public BallBase
{
public:

    //�R���X�g���N�^
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

};


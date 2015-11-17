#pragma once


#include "../../Ball/Ball.h"
#include "../../Library/Bullet/BulletSystem.h"
#include "../../GameSystem/GameObject.h"
#include "../../Damage/Damage.h"
#include "../../Ball/PhysicallyMoveBall.h"
#include "TennisPlayer.h"
//****************************************************
//	�e�j�X_�{�[������ɏグ��N���X
//****************************************************

class TennisPlayerState_SlowUpBall :public TennisState
{
public:
    //�ł^�C�v
    enum class ShotType
    {
        Smash,   //�X�}�b�V��
        CutBall, //�J�b�g���ċA���Ă���{�[��
    };

    //�R���g���[���N���X�C���^�[�t�F�[�X
    class ControllClass
    {
    public:
        virtual ~ControllClass(){}
        virtual Vector3 GetVec() = 0;
        virtual bool    DoOtherAction() = 0;
        virtual bool    DoShotAfterAction() = 0;
        virtual bool    isShot() = 0;
    };

    TennisPlayerState_SlowUpBall(
        ControllClass*       pControllClass //�I������delete����
        );

    ~TennisPlayerState_SlowUpBall();


    void Enter(TennisPlayer* p)override;
    void Execute(TennisPlayer* p)override;
    void Exit(TennisPlayer* p)override;

    ShotType GetShotType()const;

private:

    void(TennisPlayerState_SlowUpBall::*m_pStateFunc)();
    int                  m_Timer;
    PhysicallyMoveBall*  m_pUpBall;
    ControllClass*       m_pControllClass;
    TennisPlayer*        m_pTennis;
    CharacterBase*       m_pTargetEnemy;

    const CharacterBase* GetFrontTarget(TennisPlayer* p)const;
    void SetState(void(TennisPlayerState_SlowUpBall::*pStateFunc)());

    void State_SlowUp();
    void State_Smash();
    void State_CutShot();
    void State_Finish();
};

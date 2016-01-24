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
        Weak,    //�ア
        Usual,   //����
        Smash,   //�X�}�b�V��
    };

    //�R���g���[���N���X�C���^�[�t�F�[�X
    class ControllClass
    {
    public:
        virtual ~ControllClass(){}
        virtual Vector3 GetVec() = 0;
        virtual bool    DoOtherAction() = 0;
        virtual bool    DoShotAfterAction() = 0;
        virtual bool    isShot(bool isSmashFrame) = 0;
    };

    TennisPlayerState_SlowUpBall(
        ControllClass*       pControllClass //�I������delete����
        );

    ~TennisPlayerState_SlowUpBall();


    void Enter(TennisPlayer* p)override;
    void Execute(TennisPlayer* p)override;
    void Exit(TennisPlayer* p)override;

    ShotType GetShotType(int m_Timer)const;

private:

    void(TennisPlayerState_SlowUpBall::*m_pStateFunc)();
    int                  m_Timer;
    PhysicallyMoveBall*  m_pUpBall;
    ControllClass*       m_pControllClass;
    TennisPlayer*        m_pTennis;
    CharacterBase*       m_pTargetEnemy;
    bool                 m_isDoShot;

    const CharacterBase* GetFrontTarget(TennisPlayer* p)const;
    void SetState(void(TennisPlayerState_SlowUpBall::*pStateFunc)());

    void State_SlowUp();

    void State_Weak();
    void State_Usual();
    void State_Smash();

    void State_Finish();

    void AngleControll(RADIAN Speed,float CheckLen);
};

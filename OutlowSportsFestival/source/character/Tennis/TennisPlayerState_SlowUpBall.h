#pragma once


#include "../../Ball/Ball.h"
#include "../../utillity/Locus.h"
#include "../../Library/Bullet/BulletSystem.h"
#include "../../GameSystem/GameObject.h"
#include "../../Damage/Damage.h"

//*****************************************************
//		�����グ��{�[���N���X
//*****************************************************

class TennisUpBall :public GameObjectBase, public BallBase
{
public:

    //�R���X�g���N�^
    TennisUpBall(
        BallBase::Params	params			//�{�[���p�����[�^
        );

    ~TennisUpBall();


    bool Update();
    bool Msg(MsgType mt);

private:

    bool(TennisUpBall::*m_pStateFunc)();
    LpMeshRenderer		m_pMeshRenderer;
    DamageCapsure		m_Damage;
    int                 m_DeleteFrame;
    Locus               m_Locus;
    RigidBody*          m_pRigitBody;
    Vector3             m_RotateSpeed;

    bool isOutofField()const;  //�t�B�[���h�O�ɏo�Ă��邩

    void UpdateDamageClass();  //�_���[�W����̈ʒu�����݂̈ʒu�ɍX�V
    void UpdateLocusColor();   //�O�Ղ̐F�����݂̐e�L�����N�^�̐F�ɐݒ�
    bool UpdateWallCheck(Vector3& outNewMove);    //�ǂƂ̔�������A�ڐG���Ă����Ȃ�ړ��l�𔽎˂��ăX�e�[�g�ڍs������
    void AddLocusPoint();      //�O�Ղ̃|�C���g�����݂̃p�����[�^�ň�_�ǉ�����

    void Counter(CharacterBase* pCounterCharacter)override;

    void ToNoWork();           //�U������̂Ȃ���Ԃɂ���

    bool StateFlyMove();
    bool StatePhysicMove();
};




#include "TennisPlayer.h"
#include "../../Library/Bullet/BulletSystem.h"
#include "../../Damage/Damage.h"

//****************************************************
//	�e�j�X_�{�[������ɏグ��N���X
//****************************************************

class TennisState_SlowUpBall :public TennisState
{
public:
    class ControllClass
    {
    public:
        virtual ~ControllClass(){}
        virtual void AngleControll(TennisPlayer* p) = 0;
        virtual bool SwitchState(TennisState_SlowUpBall* pState) = 0;
    };

    TennisState_SlowUpBall(
        ControllClass* pControllClass   //����R���g���[������N���X(�I������delete����)
        );

    ~TennisState_SlowUpBall();

    void Enter(TennisPlayer* t)override;
    void Execute(TennisPlayer* t)override;
    void Exit(TennisPlayer* t)override;

    RATIO           GetChargePower()const;
    TennisUpBall*   GetSlowUpBall();
    void            DeleteSlowBall();

private:
    ControllClass* const m_pControllClass;
    TennisUpBall*        m_pUpBall;
    int                  m_Timer;
    const int            m_EndFrame;
};


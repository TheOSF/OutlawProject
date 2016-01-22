#pragma once

#include "CharacterBase.h"
#include "../Damage/Damage.h"
#include "../Ball/Ball.h"

//-------------------------------------------------------//
//  �@�@�@�L�����N�^�̒ʏ�J�E���^�[�N���X
//-------------------------------------------------------//

class CharacterDefaultCounter
{
public:

    //�J�E���^�[���̃R�[���o�b�N�֐��N���X
    class Event
    {
    public:
        virtual ~Event(){}

        virtual void Pose() = 0;      //�\���J�n(�X�e�[�g�̊J�n)
        virtual void Move(BallBase* pCounterBall) = 0;      //�J�E���^�[����{�[���Ɍ������Ĉړ��J�n
 
        virtual void Catch(BallBase* pCounterBall) {} // �L���b�`��

        virtual void Shot(BallBase* pCounterBall) = 0;      //�ł��Ԃ���
        virtual void ModelUpdate(float t){}

        virtual void ShotFaild() = 0; //�ړ����Ƀ{�[�����ł��Ԃ��Ȃ���ԂɂȂ�����

        virtual Vector3 ClacLocalOffset(bool Right){ return Vector3Zero; }

        virtual void End() = 0;       //�N���X�I��
    };

    //�J�E���^�[�p�����[�^
    struct Param
    {
        Param();

        UINT     PoseFrame;       //�\�����s���t���[��
        UINT     CanCounterFrame; //�J�E���^�[�ڍs���\�ȃt���[��
        UINT     ShotFrame;       //�{�[�������m���Ă���L���b�`����܂ł̃t���[��
        UINT     AfterShotFrame;  //�����Ԃ�����̃t���[��
        UINT     FailedFrame;     //�ł��Ԃ��Ɏ��s���Ă���N���X�I���܂ł̃t���[��

        UINT     CatchFrame = 0;     // �L���b�`���Ă��瓊����܂ł̃t���[�� �� 0�Ȃ瑦�ł��Ԃ�
        UINT     CatchBoneNumber; // �L���b�`�����{�[�����ǂ̃{�[���ɂ��邩
                                  
        float    CatchAriaSize;   //�J�E���^�[���m�͈�
        float    ControllRadian;  //����ł���ł��Ԃ��p�x (PI/2�𒴂���ƌ����ړI�ɂ��������Ȃ�̂Œ���)
        float    BallSpeed;       //�ł��Ԃ����Ƃ��̃{�[���̑��x(0.8f�����肪������)
    };

    
    CharacterDefaultCounter(
        CharacterBase*               pOwner,       //�I�[�i�[�L�����N�^
        const Param&                 param,        //�J�E���^�[�p�����^
        Event*                       pEventClass,  //�C�x���g�N���X�ւ̃|�C���^(����delete����)
        DamageManager::HitEventBase* pHitEventClass//�q�b�g�C�x���g(����delete����)
        );

    ~CharacterDefaultCounter();

    //���t���[���̍X�V
    void Update();

    //�X�e�B�b�N�l�̃Z�b�g(�ł��Ԃ����̕��������߂�)
    void SetStickValue(CrVector2 stick);

private:

    //�{�[���̃_���[�W���J�b�g����N���X(�f�R���[�g�p�^�[���H)
    class HitEventClass_NoBallDamageFileter :public DamageManager::HitEventBase
    {
        DamageManager::HitEventBase*const m_pInHitEvent;
        BallBase*                         m_pCounterBall;
        CharacterBase*                    m_pOwner;
    public:
        HitEventClass_NoBallDamageFileter(
            DamageManager::HitEventBase* pInHitEvent,
            BallBase*                    pCounterBall,
            CharacterBase*               pOwner
            );
        
        bool Hit(DamageBase* pDmg)override;
    };

    void Pose();  //�\����
    void Move();  //�ړ���
    void HitStop(); //�q�b�g�X�g�b�v
    void Catch(); // �L���b�`��
    void Shot();  //�ł��Ԃ���
    void Failed();//�ł��Ԃ����s
    void End();   //�I��

    //�X�e�[�g�Z�b�g
    void SetState(void(CharacterDefaultCounter::*pNewState)());

    //�X�e�B�b�N�ɂ��p�x����
    void SetStickAngle(CrVector3 OriginVec,RADIAN controllRad);

    void EffectApper();
        

    void SetAutoCounter();

    static const float                  m_SkillUpValue; //�X�L���Q�[�W���オ���
    CharacterBase* const                m_pOwner;       //�I�[�i�[�L�����N�^
    Param  const                        m_Param;        //�J�E���^�[�p�����^
    Event* const                        m_pEventClass;  //�C�x���g�N���X�ւ̃|�C���^(����delete����)
    DamageManager::HitEventBase* const  m_pHitEventClass;//�q�b�g�C�x���g(����delete����)
    UINT                                m_HitStopFrame;

    void(CharacterDefaultCounter::*m_pNowState)();       //�X�e�[�g�֐�
    UINT        m_Count;
    BallBase*   m_pCounterBall;
    Vector3     m_MoveTargetPos;
    Vector3     m_BallFreezePos;
    Vector2     m_Stick;
    float       m_MoveValue;
    BallBase::BallID m_counterBallID;
};


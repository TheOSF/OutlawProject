#pragma once

#include "CharacterBase.h"
#include "../Damage/Damage.h"

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
        virtual void Move() = 0;      //�J�E���^�[����{�[���Ɍ������Ĉړ��J�n
 
        virtual void Shot() = 0;      //�ł��Ԃ���
        virtual void ShotFaild() = 0; //�ړ����Ƀ{�[�����ł��Ԃ��Ȃ���ԂɂȂ�����

        virtual void End() = 0;       //�N���X�I��
    };

    //�J�E���^�[�p�����[�^
    struct Param
    {
        UINT     PoseFrame;       //�\�����s���t���[��
        UINT     CanCounterFrame; //�J�E���^�[�ڍs���\�ȃt���[��
        UINT     ShotFrame;       //�{�[�������m���Ă���ł܂ł̃t���[��
        UINT     AfterShotFrame;  //�����Ԃ�����̃t���[��
        UINT     FailedFrame;     //�ł��Ԃ��Ɏ��s���Ă���N���X�I���܂ł̃t���[��
                                  
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

    void Pose();  //�\����
    void Move();  //�ړ���
    void Shot();  //�ł��Ԃ���
    void Failed();//�ł��Ԃ����s
    void End();   //�I��

    //�X�e�[�g�Z�b�g
    void SetState(void(CharacterDefaultCounter::*pNewState)());

    //�X�e�B�b�N�ɂ��p�x����
    void SetStickAngle(CrVector3 OriginVec);

    CharacterBase* const                m_pOwner;       //�I�[�i�[�L�����N�^
    Param  const                        m_Param;        //�J�E���^�[�p�����^
    Event* const                        m_pEventClass;  //�C�x���g�N���X�ւ̃|�C���^(����delete����)
    DamageManager::HitEventBase* const  m_pHitEventClass;//�q�b�g�C�x���g(����delete����)

    void(CharacterDefaultCounter::*m_pNowState)();       //�X�e�[�g�֐�
    UINT        m_Count;
    BallBase*   m_pCounterBall;
    Vector3     m_MoveTargetPos;
    Vector2     m_Stick;
    float       m_MoveValue;
};


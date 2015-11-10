#include "CharacterDefaultCounterClass.h"

#include "../Ball/Ball.h"
#include "CharacterFunction.h"

#include "../Effect/HitEffectObject.h"
#include "../Effect/EffectFactory.h"
#include "../Effect/ImpactLightObject.h"

#include "../Sound/Sound.h"

#include "CharacterManager.h"
#include "../Ball/UsualBall.h"
#include "../GameSystem/GameController.h"


CharacterDefaultCounter::CharacterDefaultCounter(
    CharacterBase*               pOwner,        //�I�[�i�[�L�����N�^
    const Param&                 param,         //�J�E���^�[�p�����^
    Event*                       pEventClass,   //�C�x���g�N���X�ւ̃|�C���^(����delete����)
    DamageManager::HitEventBase* pHitEventClass //�q�b�g�C�x���g(����delete����)
    ) :
    m_pOwner(pOwner),
    m_Param(param),
    m_pEventClass(pEventClass),
    m_pHitEventClass(pHitEventClass),
    m_pNowState(&CharacterDefaultCounter::Pose),
    m_Count(0),
    m_Stick(0, 0),
    m_pCounterBall(nullptr)
{

}

CharacterDefaultCounter::~CharacterDefaultCounter()
{
    delete m_pEventClass;
    delete m_pHitEventClass;
}


//���t���[���̍X�V
void CharacterDefaultCounter::Update()
{
    (this->*m_pNowState)();
}

//�X�e�B�b�N�l�̃Z�b�g(�ł��Ԃ����̕��������߂�)
void CharacterDefaultCounter::SetStickValue(CrVector2 stick)
{
    m_Stick = stick;

    //�J������ɕϊ�
    Vector3 temp(m_Stick.x, m_Stick.y, 0);
    
    temp = Vector3MulMatrix3x3(temp, matView);

    m_Stick.x = temp.x;
    m_Stick.y = temp.y;
}


//-----------------------------------------------//
//  �J�E���^�[���̃_���[�W����N���X
//-----------------------------------------------//

CharacterDefaultCounter::HitEventClass_NoBallDamageFileter::HitEventClass_NoBallDamageFileter(
    DamageManager::HitEventBase* pInHitEvent,
    BallBase*                    pCounterBall
    ):
m_pInHitEvent(pInHitEvent),
m_pCounterBall(pCounterBall)
{

}

bool CharacterDefaultCounter::HitEventClass_NoBallDamageFileter::Hit(DamageBase* pDmg)
{
    //�J�E���^�[����\��̃{�[���������ꍇ�͏�������false��Ԃ�
    if (pDmg->pBall != nullptr &&
        BallBase::isCanCounter(pDmg->pBall) &&
        m_pCounterBall == pDmg->pBall
        )
    {
        return false;
    }

    //�ʏ�̃_���[�W����
    return m_pInHitEvent->Hit(pDmg);
}

//-----------------------------------------------//
//  �X�e�[�g�֐�
//-----------------------------------------------// 

//�\����
void CharacterDefaultCounter::Pose()  
{
    //�J�E���^�X�V
    ++m_Count;
    
    if (m_Count == 1)
    {
        //�G�t�F�N�g
        {
            const Vector3 pos = m_pOwner->m_Params.pos + Vector3(0, 3.5f, 0);
            EffectFactory::Counter(pos, 6.5f);

            new ImpactLightObject(
                pos,
                15.0f,
                Vector3(1, 1, 1)*0.5f, 
                0.05f
                );
        }

        //�T�E���h
        Sound::Play(Sound::Counter);

        //�C�x���g�N���X�ʒm
        m_pEventClass->Pose();

        //�ړ��ʂ�������
        chr_func::XZMoveDown(m_pOwner, 1);
    }

    //�J�E���^�[�\�ȃt���[���Ȃ�
    if (m_Count < m_Param.CanCounterFrame)
    {



        //�{�[�����m
        if (DefBallMgr.GetCounterBall(
            &m_pCounterBall,
            m_pOwner->m_Params.pos,
            &m_MoveTargetPos,
            m_Param.CatchAriaSize,
            (int)m_Param.ShotFrame))
        {
            //�{�[���̏ꏊ�̏�����ɂ��炷(2�L������)
            Vector3 V = Vector3Normalize(m_pCounterBall->m_Params.move);
            V.y = 0;
            m_MoveTargetPos += V*m_pOwner->m_Params.size;

            //�J�E���^�[�ł���{�[�������������ꍇ�A�X�e�[�g���ړ��X�e�[�g�Ɉڍs����
            SetState(&CharacterDefaultCounter::Move);
        }
    }


    //���ԂŏI��
    if (m_Count > m_Param.PoseFrame)
    {
        SetState(&CharacterDefaultCounter::End);
    }


    //��{�I�ȍX�V
    if (m_Count < m_Param.CanCounterFrame)
    {
        HitEventClass_NoBallDamageFileter BallDmgFilter(m_pHitEventClass, m_pCounterBall);

        chr_func::UpdateAll(m_pOwner, &BallDmgFilter);
    }
    else
    {
        chr_func::UpdateAll(m_pOwner, m_pHitEventClass);
    }
}


//�ړ���
void CharacterDefaultCounter::Move()
{

    //�J�E���^�X�V
    ++m_Count;

    //�C�x���g�N���X�ʒm
    if (m_Count == 1)
    {
        m_pEventClass->Move(m_pCounterBall);
    }

    //�P�t���[��������̈ړ��l���Z�o
    if (m_Count == 1)
    {
        m_MoveValue = 1.0f / (float)m_Param.ShotFrame;
    }

    //�J�E���^�[����{�[�����J�E���^�[�ł��Ȃ���ԂȂ�ł��Ԃ����s�X�e�[�g�ֈڍs
    if (BallBase::isCanCounter(m_pCounterBall) == false)
    {
        SetState(&CharacterDefaultCounter::Failed);
        return;
    }

    //�ړ��l���Z�b�g
    {
        Vector3 m = m_MoveTargetPos - m_pOwner->m_Params.pos;

        m.y = 0;
        m *= m_MoveValue;

        m_pOwner->m_Params.move = m;
    }


    //�A���O��
    {   
        if (m_pCounterBall->m_Params.pParent != m_pOwner)
        {
            Vector3 ViewPos = m_pOwner->m_Params.pos + (m_pCounterBall->m_Params.pParent->m_Params.pos - m_MoveTargetPos);
            chr_func::AngleControll(m_pOwner, ViewPos, PI / (float)m_Param.ShotFrame);
        }
        else
        {
            SetStickAngle(-m_pCounterBall->m_Params.move, PI);
        }
    }


    //���Ԃőł��X�e�[�g�ֈڍs
    if (m_Count > m_Param.ShotFrame)
    {
        SetState(&CharacterDefaultCounter::Shot);
    }

    ////���łɃ{�[���Ƃ̋��������ȉ��Ȃ�ł��X�e�[�g�ֈڍs
    //if (Vector3Distance(m_pCounterBall->m_Params.pos, m_pOwner->m_Params.pos + Vector3(0, BallBase::UsualBallShotY, 0)) < 1.0f) 
    //{
    //    SetState(&CharacterDefaultCounter::Shot);
    //    return;
    //}

    //��{�I�ȍX�V
    {
        HitEventClass_NoBallDamageFileter BallDmgFilter(m_pHitEventClass, m_pCounterBall);

        chr_func::UpdateAll(m_pOwner, &BallDmgFilter);
    }
}

//�ł��Ԃ����s
void CharacterDefaultCounter::Failed()
{
    //�J�E���^�X�V
    ++m_Count;

    //�C�x���g�N���X�ʒm
    if (m_Count == 1)
    {
        m_pEventClass->ShotFaild();
    }

    //���ԂŏI���X�e�[�g��
    if (m_Count > m_Param.FailedFrame)
    {
        SetState(&CharacterDefaultCounter::End);
        return;
    }

    //��{�I�ȍX�V
    {
        chr_func::XZMoveDown(m_pOwner,0.1f);
        chr_func::UpdateAll(m_pOwner, m_pHitEventClass);
    }
}


//�ł��Ԃ���
void CharacterDefaultCounter::Shot()
{
    //�J�E���^�X�V
    ++m_Count;

    //�C�x���g�N���X�ʒm
    if (m_Count == 1)
    {
        m_pEventClass->Shot(m_pCounterBall);
    }

    //�{�[���ł��Ԃ����I
    if (m_Count == 1)
    {

        //�X�e�B�b�N�ɂ��p�x����
        if (m_pCounterBall->m_Params.pParent == m_pOwner)
        {
            Vector3 vec;
            chr_func::GetFront(m_pOwner, &vec);
            SetStickAngle(vec, m_Param.ControllRadian);
        }
        else
        {
            //�����I�ɐe�̌��ɕԂ�
            SetAutoCounter();
        }

        {
            float m = m_pCounterBall->m_Params.move.Length()*1.1f;
            m = min(m, 2.5f);

            //�{�[���̐ݒ�
            chr_func::GetFront(m_pOwner, &m_pCounterBall->m_Params.move);

            m_pCounterBall->m_Params.move *= m;
        }


        {
            m_pCounterBall->m_Params.pos = m_pOwner->m_Params.pos;
            m_pCounterBall->m_Params.pos.y = UsualBall::UsualBallShotY;
        }

        //�{�[�����̃J�E���^�[����
        m_pCounterBall->Counter(m_pOwner);

        COLORf EffectColor(CharacterBase::GetPlayerColor(m_pCounterBall->m_Params.pParent->m_PlayerInfo.number));
        
        //�G�t�F�N�g�̐ݒ�
        new HitEffectObject(
            m_pCounterBall->m_Params.pos,
            Vector3Normalize(m_pCounterBall->m_Params.move),
            0.1f,
            0.1f,
            Vector3(1, 1, 1)
            );

        //�J�E���^�[���Đ�
        Sound::Play(Sound::AtkHit2);

        //�R���g���[����U��
        controller::SetVibration(
            10000,
            0.10f,
            m_pOwner->m_PlayerInfo.number
            );

        //�኱�O�ɐi��
        chr_func::AddMoveFront(m_pOwner, 0.2f, 1.0f);
    }


    //���ԂŏI���X�e�[�g��
    if (m_Count > m_Param.AfterShotFrame)
    {
        SetState(&CharacterDefaultCounter::End);
        return;
    }

    //��{�I�ȍX�V
    {
        HitEventClass_NoBallDamageFileter BallDmgFilter(m_pHitEventClass, m_pCounterBall);

        DamageManager::HitEventBase* pHitEvent = &BallDmgFilter;

        if (m_Count > 1)
        {
            pHitEvent = m_pHitEventClass;
        }

        //�ł��I����Ă���̂ŃX�s�[�h�_�E��
    //    chr_func::XZMoveDown(m_pOwner, 0.1f);

        chr_func::UpdateAll(m_pOwner, &BallDmgFilter);
    }

}


//�I��
void CharacterDefaultCounter::End()
{
    
    if (m_Count == 0)
    {
        //��񂵂����s���Ȃ��悤��
        ++m_Count; 

        //�I���ʒm���C�x���g�N���X�ɑ���
        m_pEventClass->End();
    }

    //��{�I�ȍX�V
    {
        chr_func::UpdateAll(m_pOwner, m_pHitEventClass);
    }
}





//�X�e�[�g�Z�b�g
void CharacterDefaultCounter::SetState(void(CharacterDefaultCounter::*pNewState)())
{
    m_pNowState = pNewState;
    m_Count = 0;
}


////�X�e�B�b�N�ɂ��p�x����
//void CharacterDefaultCounter::SetStickAngle(CrVector3 OriginVec, RADIAN controllRad)
//{
//    if (m_Stick.x == 0 && m_Stick.y == 0)
//    {
//        Vector3 ViewPos = m_pOwner->m_Params.pos + OriginVec;
//        ViewPos.y = m_pOwner->m_Params.pos.y;
//
//        chr_func::AngleControll(m_pOwner, ViewPos);
//        return;
//    }
//
//    Vector3 RotateVec;
//
//    //��]����
//    float toStickAngle = Vector2Dot(m_Stick, Vector2(OriginVec.x, OriginVec.z));
//
//    m_Stick.x = -m_Stick.x;
//
//    toStickAngle /= Vector2Length(m_Stick)*Vector2Length(Vector2(OriginVec.x, OriginVec.z));
//    toStickAngle = acosf(toStickAngle);
//
//    //�p�x����
//    toStickAngle = min(toStickAngle, controllRad);
//
//    //���E����
//    float CrossY = m_Stick.y*OriginVec.x + m_Stick.x*OriginVec.z;
//    if (CrossY > 0)
//    {
//        toStickAngle = -toStickAngle;
//    }
//
//    Matrix m;
//    D3DXMatrixRotationY(&m, toStickAngle);
//
//    RotateVec = Vector3MulMatrix3x3(OriginVec, m);
//
//
//    RotateVec = m_pOwner->m_Params.pos + RotateVec;
//    RotateVec.y = m_pOwner->m_Params.pos.y;
//
//    chr_func::AngleControll(m_pOwner, RotateVec);
//}



//�X�e�B�b�N�ɂ��p�x����
void CharacterDefaultCounter::SetStickAngle(CrVector3 OriginVec, RADIAN controllRad)
{

    Vector3 RotateVec;

    //�X�e�B�b�N�̓|���������̃x�N�g�����쐬
    //if (m_Stick.x != 0 || m_Stick.y != 0)
    if (0)
    {
        //��]����
        float toStickAngle = Vector2Dot(m_Stick, Vector2(OriginVec.x, OriginVec.z));

        m_Stick.x = -m_Stick.x;

        toStickAngle /= Vector2Length(m_Stick)*Vector2Length(Vector2(OriginVec.x, OriginVec.z));
        toStickAngle = acosf(toStickAngle);

        //�p�x����
        toStickAngle = min(toStickAngle, controllRad);

        //���E����
        float CrossY = m_Stick.y*OriginVec.x + m_Stick.x*OriginVec.z;
        if (CrossY > 0)
        {
            toStickAngle = -toStickAngle;
        }

        Matrix m;
        D3DXMatrixRotationY(&m, toStickAngle);

        RotateVec = Vector3MulMatrix3x3(OriginVec, m);
        RotateVec.Normalize();
    }
    else
    {
        RotateVec = OriginVec;
    }


    //�e�I�L�����N�^�ւ̃x�N�g���̒��ň�ԁ@���̃x�N�g���Ƌ߂����̂��Z�o
    {

        RADIAN  most_min_angle = PI, temp_angle;
        Vector3 v, answer;

        const CharacterManager::CharacterMap& ChrMap = DefCharacterMgr.GetCharacterMap();

        answer = RotateVec;


        for (auto& it : ChrMap)
        {
            if (it.first == m_pOwner ||
                chr_func::isDie(it.first))
            {
                continue;
            }

            v = it.first->m_Params.pos - m_pOwner->m_Params.pos;
            v.y = 0;

 
            temp_angle = Vector3Radian(v, RotateVec);

            if (temp_angle < most_min_angle)
            {
                most_min_angle = temp_angle;
                answer = v;
            }
        }

        RotateVec = answer;
    }

    //��]
    {
        chr_func::AngleControll(m_pOwner, m_pOwner->m_Params.pos + RotateVec);
    }
}



void CharacterDefaultCounter::SetAutoCounter()
{
    chr_func::AngleControll(m_pOwner, m_pCounterBall->m_Params.pParent->m_Params.pos);
}
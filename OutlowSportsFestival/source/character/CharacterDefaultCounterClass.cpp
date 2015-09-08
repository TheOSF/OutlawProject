#include "CharacterDefaultCounterClass.h"

#include "../Ball/Ball.h"
#include "CharacterFunction.h"

#include "../Effect/HitEffectObject.h"


CharacterDefaultCounter::CharacterDefaultCounter(
    CharacterBase*               pOwner,       //�I�[�i�[�L�����N�^
    const Param&                 param,        //�J�E���^�[�p�����^
    Event*                       pEventClass,  //�C�x���g�N���X�ւ̃|�C���^(����delete����)
    DamageManager::HitEventBase* pHitEventClass//�q�b�g�C�x���g(����delete����)
    ):
    m_pOwner(pOwner),
    m_Param(param),
    m_pEventClass(pEventClass),
    m_pHitEventClass(pHitEventClass),
    m_pNowState(&CharacterDefaultCounter::Pose),
    m_Count(0),
    m_Stick(0,0)
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
//  �X�e�[�g�֐�
//-----------------------------------------------// 

//�\����
void CharacterDefaultCounter::Pose()  
{
    //�J�E���^�X�V
    ++m_Count;

    //�C�x���g�N���X�ʒm
    if (m_Count == 1)
    {
        m_pEventClass->Pose();

        {
            chr_func::XZMoveDown(m_pOwner, 1);
        }
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
            //�{�[���̏ꏊ�̏�����ɂ��炷(�P�L������)
            Vector3 V = Vector3Normalize(m_pCounterBall->m_Params.move);
            V.y = 0;
            m_MoveTargetPos += V*m_pOwner->m_Params.size * 2;

            //�J�E���^�[�ł���{�[�������������ꍇ�A�X�e�[�g���ړ��X�e�[�g�Ɉڍs����
            SetState(&CharacterDefaultCounter::Move);

            return;
        }
    }


    //���ԂŏI��
    if (m_Count > m_Param.PoseFrame)
    {
        SetState(&CharacterDefaultCounter::End);
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
        m_pEventClass->Move();
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

    //�ړ�
    {
        Vector3 m = m_MoveTargetPos - m_pOwner->m_Params.pos;

        m.y = 0;
        //m.Normalize();
        m *= m_MoveValue;

        m_pOwner->m_Params.pos += m;
    }


    //�A���O��
    {
        Vector3 ViewPos = m_pOwner->m_Params.pos + (m_pCounterBall->m_Params.pParent->m_Params.pos - m_MoveTargetPos);
        
        ViewPos.y = m_pOwner->m_Params.pos.y;

        chr_func::AngleControll(m_pOwner, ViewPos, PI / (float)m_Param.ShotFrame);
    }


    //���Ԃőł��X�e�[�g�ֈڍs
    if (m_Count > m_Param.ShotFrame)
    {
        SetState(&CharacterDefaultCounter::Shot);
        return;
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
}


//�ł��Ԃ���
void CharacterDefaultCounter::Shot()
{
    //�J�E���^�X�V
    ++m_Count;

    //�C�x���g�N���X�ʒm
    if (m_Count == 1)
    {
        m_pEventClass->Shot();
    }

    //�{�[���ł��Ԃ����I
    if (m_Count == 1)
    {
        //�X�e�B�b�N�ɂ��p�x����
        SetStickAngle(m_pCounterBall->m_Params.pParent->m_Params.pos - m_pOwner->m_Params.pos);

        //�{�[���̐ݒ�
        chr_func::GetFront(m_pOwner, &m_pCounterBall->m_Params.move);
        
        m_pCounterBall->m_Params.move *= m_Param.BallSpeed;
        m_pCounterBall->m_Params.pParent = m_pOwner;

        //�{�[�����̃J�E���^�[����
        m_pCounterBall->Counter();

        COLORf EffectColor(CharacterBase::GetPlayerColor(m_pCounterBall->m_Params.pParent->m_PlayerInfo.number));

        //�G�t�F�N�g�̐ݒ�
        new HitEffectObject(
            m_pCounterBall->m_Params.pos,
            Vector3Normalize(m_pCounterBall->m_Params.move),
            0.1f,
            0.1f,
            Vector3(EffectColor.r, EffectColor.g, EffectColor.b)
            );

    }

    //���ԂŏI���X�e�[�g��
    if (m_Count > m_Param.AfterShotFrame)
    {
        SetState(&CharacterDefaultCounter::End);
        return;
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
}





//�X�e�[�g�Z�b�g
void CharacterDefaultCounter::SetState(void(CharacterDefaultCounter::*pNewState)())
{
    m_pNowState = pNewState;
    m_Count = 0;
}


//�X�e�B�b�N�ɂ��p�x����
void CharacterDefaultCounter::SetStickAngle(CrVector3 OriginVec)
{
    if (m_Stick.x == 0 && m_Stick.y == 0)
    {
        Vector3 ViewPos = m_pOwner->m_Params.pos + OriginVec;
        ViewPos.y = m_pOwner->m_Params.pos.y;

        chr_func::AngleControll(m_pOwner, ViewPos);
        return;
    }

    Vector3 RotateVec;

    //��]����
    float toStickAngle = Vector2Dot(m_Stick, Vector2(OriginVec.x, OriginVec.z));

    m_Stick.x = -m_Stick.x;

    toStickAngle /= Vector2Length(m_Stick)*Vector2Length(Vector2(OriginVec.x, OriginVec.z));
    toStickAngle = acosf(toStickAngle);

    //�p�x����
    toStickAngle = min(toStickAngle, m_Param.ControllRadian);

    //���E����
    float CrossY = m_Stick.y*OriginVec.x + m_Stick.x*OriginVec.z;
    if (CrossY > 0)
    {
        toStickAngle = -toStickAngle;
    }

    Matrix m;
    D3DXMatrixRotationY(&m, toStickAngle);

    RotateVec = Vector3MulMatrix3x3(OriginVec, m);


    RotateVec = m_pOwner->m_Params.pos + RotateVec;
    RotateVec.y = m_pOwner->m_Params.pos.y;

    chr_func::AngleControll(m_pOwner, RotateVec);
}
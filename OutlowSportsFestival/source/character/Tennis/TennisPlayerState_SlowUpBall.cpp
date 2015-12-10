#include "TennisPlayerState_SlowUpBall.h"
#include "../CharacterFunction.h"
#include "Tennis_HitEvent.h"
#include "../../GameSystem/GameController.h"
#include "../CharacterFunction.h"
#include "../../Ball/UsualBall.h"
#include "../../Damage/Damage.h"
#include "../CharacterCounterClass.h"
#include "../CharacterMoveClass.h"
#include "../CharacterShotAttackClass.h"
#include "../CharacterManager.h"

#include "../../Effect/HitEffectObject.h"
#include "../../Sound/Sound.h"
#include "../../GameSystem/GameController.h"
#include "TennisPlayerState_UsualMove.h"



TennisPlayerState_SlowUpBall::TennisPlayerState_SlowUpBall(
    ControllClass*       pControllClass //�I������delete����
    ) :
    m_pTargetEnemy(nullptr),
    m_pUpBall(nullptr),
    m_pControllClass(pControllClass),
    m_Timer(0),
    m_pStateFunc(&TennisPlayerState_SlowUpBall::State_SlowUp),
    m_isDoShot(false)
{

}


TennisPlayerState_SlowUpBall::~TennisPlayerState_SlowUpBall()
{
    delete m_pControllClass;
}


void TennisPlayerState_SlowUpBall::Enter(TennisPlayer* p)
{
    m_pTennis = p;
}

void TennisPlayerState_SlowUpBall::Execute(TennisPlayer* p)
{
    (this->*m_pStateFunc)();

    //��{�I�ȍX�V
    {
        TennisHitEvent HitEvent(p);
        chr_func::UpdateAll(p, &HitEvent);

        //���f���X�V
        p->m_Renderer.Update(1);

        //�s��X�V
        chr_func::CreateTransMatrix(p, p->m_ModelSize, &p->m_Renderer.m_TransMatrix);

    }
}

void TennisPlayerState_SlowUpBall::Exit(TennisPlayer* p)
{

}


TennisPlayerState_SlowUpBall::ShotType TennisPlayerState_SlowUpBall::GetShotType(int Timer)const
{
    //�����n��
    if (Timer < 20)
    {
        return ShotType::Weak;
    }
    else if (Timer < 32)//�ʏ�
    {
        return ShotType::Usual;
    }
    if (Timer < 50) //�{�[���Œ��_
    {
        return ShotType::Smash;
    }

    //����ȊO
    return ShotType::Usual;
}

void TennisPlayerState_SlowUpBall::SetState(void(TennisPlayerState_SlowUpBall::*pStateFunc)())
{
    m_pStateFunc = pStateFunc;
    m_Timer = 0;
}



void TennisPlayerState_SlowUpBall::State_SlowUp()
{
    const int SlowFrame = 10;

    ++m_Timer;

    //���X�ɃX�s�[�h�_�E��
    if (m_Timer < SlowFrame)
    {
        chr_func::XZMoveDown(m_pTennis, 0.2f);
    }

    //�ł��t���O�̍X�V
    if (m_isDoShot == false)
    {
        m_isDoShot = m_pControllClass->isShot();
    }


    if (m_Timer == SlowFrame)
    {
        //�ړ��l���O��
        chr_func::XZMoveDown(m_pTennis, 1.0f);

        //���[�V����
        m_pTennis->m_Renderer.SetMotion(TennisPlayer::_mt_UpBall);

        //�����グ�{�[���쐬
        BallBase::Params param;

        //�ړ��͏����
        param.move = m_pTennis->m_Params.move;
        param.move.y = 0.4f;

        //�L�����̏ꏊ��(�ŏI�I�ɘr�̈ʒu�ɁH)
        param.pos = m_pTennis->m_Params.pos + Vector3(0, 2.0f, 0);
        //�e��������
        param.pParent = m_pTennis;
        //�J�E���^�[�ł��Ȃ��^�C�v
        param.type = BallBase::Type::_CantCounter;

        //����
        m_pUpBall = new PhysicallyMoveBall(param, DamageBase::Type::_WeekDamage, 1, -0.0135f);// -0.015f);

        //�ł��グ���͂����蔻��Ȃ�
        m_pUpBall->m_Damage.m_Enable = false;

        //�T�E���h
        Sound::Play(Sound::Swing1);
    }

    if (m_Timer == 60)
    {
        SetState(&TennisPlayerState_SlowUpBall::State_Finish);
        return;
    }

    if (m_Timer > SlowFrame)
    {
        //�L�����Z��
        if (m_pControllClass->DoOtherAction())
        {
            return;
        }

        //�p�x����
        AngleControll(D3DXToRadian(8), 100.0f);

        //�������߂��o����
        if (m_isDoShot)
        {
            //�V���b�g����
            switch (GetShotType(m_Timer))
            {
            case ShotType::Weak:
                SetState(&TennisPlayerState_SlowUpBall::State_Weak);
                return;
            case ShotType::Usual:
                SetState(&TennisPlayerState_SlowUpBall::State_Usual);
                return;
            case ShotType::Smash:
                SetState(&TennisPlayerState_SlowUpBall::State_Smash);
                return;
            }
        }
    }
}

void TennisPlayerState_SlowUpBall::State_Weak()
{
    const RADIAN angle_speed = D3DXToRadian(15);
    const int ShotFrame = 9;
    const int EndFrame = 36;

    ++m_Timer;

    if (m_Timer == 1)
    {
        //���[�V����
        m_pTennis->m_Renderer.SetMotion(TennisPlayer::_mt_Shot);
    }


    if (m_Timer == ShotFrame)
    {
        //�ł��グ�{�[��������
        m_pUpBall->m_DeleteFlag = true;
        m_pUpBall = nullptr;

        //�{�[������
        BallBase::Params param;

        //�ړ��͑O����
        chr_func::GetFront(m_pTennis, &param.move);
        //�X�s�[�h�͓K��
        param.move *= 0.5f;

        //�L�����̏ꏊ��(�ŏI�I�ɘr�̈ʒu�ɁH)
        param.pos = m_pTennis->m_Params.pos + Vector3(0, UsualBall::UsualBallShotY, 0);
        //�������L�������ʃ{�[�����˂�Y��
        param.pos.y = BallBase::UsualBallShotY;

        //�e��������
        param.pParent = m_pTennis;
        //�ʏ�^�C�v
        param.type = BallBase::Type::_Usual;

        //����
        new UsualBall(param, DamageBase::Type::_WeekDamage, 3, 1, 120);


        //�R���g���[����U��
        chr_func::SetControllerShock(
            m_pTennis,
            0.5f,
            0.15f
            );

        //�T�E���h
        Sound::Play(Sound::Tennis_BallAtk);

        //�G�t�F�N�g
        {
            //�G�t�F�N�g�̐ݒ�
            new HitEffectObject(
                param.pos,
                Vector3Normalize(param.move),
                0.1f,
                0.1f,
                Vector3(1, 1, 1)
                );
        }
    }

    //�L�����Z��
    if (m_Timer > ShotFrame + 3)
    {
        m_pControllClass->DoOtherAction();
    }

    if (m_Timer < ShotFrame)
    {
        //�p�x����
        AngleControll(D3DXToRadian(15), 100.0f);
    }

    //��莞�ԂŏI���X�e�[�g��
    if (m_Timer >= EndFrame)
    {
        SetState(&TennisPlayerState_SlowUpBall::State_Finish);
    }
}

void TennisPlayerState_SlowUpBall::State_Usual()
{
    const RADIAN angle_speed = D3DXToRadian(15);
    const int ShotFrame = 3;
    const int EndFrame = 15;

    ++m_Timer;

    if (m_Timer == 1)
    {
        //���[�V����
        m_pTennis->m_Renderer.SetMotion(TennisPlayer::_mt_Smash);
    }


    if (m_Timer == ShotFrame)
    {
        //�ł��グ�{�[��������
        m_pUpBall->m_DeleteFlag = true;
        m_pUpBall = nullptr;

        //�{�[������
        BallBase::Params param;

        //�ړ��͑O����
        chr_func::GetFront(m_pTennis, &param.move);
        //�X�s�[�h�͓K��
        param.move *= 0.55f;

        //�L�����̏ꏊ��(�ŏI�I�ɘr�̈ʒu�ɁH)
        param.pos = m_pTennis->m_Params.pos + Vector3(0, UsualBall::UsualBallShotY, 0);
        //�������L�������ʃ{�[�����˂�Y��
        param.pos.y = BallBase::UsualBallShotY;

        //�e��������
        param.pParent = m_pTennis;
        //�ʏ�^�C�v
        param.type = BallBase::Type::_Usual;

        //����
        new UsualBall(param, DamageBase::Type::_WeekDamage, 5);


        //�R���g���[����U��
        chr_func::SetControllerShock(
            m_pTennis,
            0.5f,
            0.15f
            );

        //�T�E���h
        Sound::Play(Sound::Tennis_BallAtk);

        //�G�t�F�N�g
        {
            //�G�t�F�N�g�̐ݒ�
            new HitEffectObject(
                param.pos,
                Vector3Normalize(param.move),
                0.1f,
                0.1f,
                Vector3(1, 1, 1)
                );
        }
    }

    if (m_Timer < ShotFrame)
    {
        //�p�x����
        AngleControll(D3DXToRadian(15), 100.0f);
    }

    //��莞�ԂŏI���X�e�[�g��
    if (m_Timer >= EndFrame)
    {
        SetState(&TennisPlayerState_SlowUpBall::State_Finish);
    }
}

void TennisPlayerState_SlowUpBall::State_Smash()
{
    const RADIAN angle_speed = D3DXToRadian(15);
    const int SmashFrame = 3;
    const int EndFrame = 15;

    ++m_Timer;

    if (m_Timer == 1)
    {
        //���[�V����
        m_pTennis->m_Renderer.SetMotion(TennisPlayer::_mt_Smash);
    }


    if (m_Timer == SmashFrame)
    {
        //�ł��グ�{�[��������
        m_pUpBall->m_DeleteFlag = true;
        m_pUpBall = nullptr;

        //�{�[������
        BallBase::Params param;

        //�ړ��͑O����
        chr_func::GetFront(m_pTennis, &param.move);
        //�X�s�[�h�͓K��
        param.move *= 1.2f;

        //�L�����̏ꏊ��(�ŏI�I�ɘr�̈ʒu�ɁH)
        param.pos = m_pTennis->m_Params.pos + Vector3(0, UsualBall::UsualBallShotY, 0);
        //�������L�������ʃ{�[�����˂�Y��
        param.pos.y = BallBase::UsualBallShotY;

        //�e��������
        param.pParent = m_pTennis;
        //�ʏ�^�C�v
        param.type = BallBase::Type::_Usual;

        //����
        (new UsualBall(param, DamageBase::Type::_VanishDamage, 15))->Counter(m_pTennis);


        //�R���g���[����U��
        chr_func::SetControllerShock(
            m_pTennis,
            1.0f,
            0.15f
            );

        //�T�E���h
        Sound::Play(Sound::AtkHit2);

        //�G�t�F�N�g
        {
            //�G�t�F�N�g�̐ݒ�
            new HitEffectObject(
                param.pos,
                Vector3Normalize(param.move),
                0.1f,
                0.1f,
                Vector3(1, 1, 1)
                );
        }
    }

    if (m_Timer < SmashFrame)
    {
        //�p�x����
        AngleControll(D3DXToRadian(15), 100.0f);
    }

    //��莞�ԂŏI���X�e�[�g��
    if (m_Timer >= EndFrame)
    {
        SetState(&TennisPlayerState_SlowUpBall::State_Finish);
    }
}



void TennisPlayerState_SlowUpBall::State_Finish()
{
    m_pTennis->SetState(TennisState_PlayerControll_Move::GetPlayerControllMove(m_pTennis));
}

void TennisPlayerState_SlowUpBall::AngleControll(RADIAN Speed, float CheckLen)
{
    //�p�x����
    CharacterBase* pTarget = nullptr;

    if (chr_func::CalcAtkTarget(m_pTennis, D3DXToRadian(45), CheckLen, &pTarget))
    {
        chr_func::AngleControll(m_pTennis, pTarget->m_Params.pos, D3DXToRadian(10));
    }
    else
    {
        chr_func::AngleControll(m_pTennis, m_pTennis->m_Params.pos + m_pControllClass->GetVec(), Speed);
    }
}
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

#include "TennisState_BoundBallAtk.h"
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
    m_pStateFunc(&TennisPlayerState_SlowUpBall::State_SlowUp)
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


TennisPlayerState_SlowUpBall::ShotType TennisPlayerState_SlowUpBall::GetShotType()const
{
    //�����n��
    if (m_Timer < 20)
    {
        return ShotType::CutBall;
    }
    else 
    if (m_Timer < 40) //�{�[���Œ��_
    {
        return ShotType::Smash;
    }

    //����ȊO
    return ShotType::CutBall;
}

void TennisPlayerState_SlowUpBall::SetState(void(TennisPlayerState_SlowUpBall::*pStateFunc)())
{
    m_pStateFunc = pStateFunc;
    m_Timer = 0;
}



void TennisPlayerState_SlowUpBall::State_SlowUp()
{
    const int SlowFrame = 5;

    ++m_Timer;

    //���X�ɃX�s�[�h�_�E��
    if (m_Timer < SlowFrame)
    {
        chr_func::XZMoveDown(m_pTennis, 0.2f);
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
        param.move.y = 0.40f;

        //�L�����̏ꏊ��(�ŏI�I�ɘr�̈ʒu�ɁH)
        param.pos = m_pTennis->m_Params.pos + Vector3(0, 2.0f, 0);
        //�e��������
        param.pParent = m_pTennis;
        //�J�E���^�[�ł��Ȃ��^�C�v
        param.type = BallBase::Type::_CantCounter;

        //����
        m_pUpBall = new PhysicallyMoveBall(param, DamageBase::Type::_WeekDamage, 1, -0.015f);

        //�ł��グ���͂����蔻��Ȃ�
        m_pUpBall->m_Damage.m_Enable = false;

        //�T�E���h
        Sound::Play(Sound::Swing1);
    }

    if (m_Timer == 60)
    {
        SetState(&TennisPlayerState_SlowUpBall::State_Finish);
    }

    //�������߂��o����
    if (m_pControllClass->isShot())
    {
        //�V���b�g����
        switch (GetShotType())
        {
        case ShotType::Smash:
            SetState(&TennisPlayerState_SlowUpBall::State_Smash);
            break;

        case ShotType::CutBall:
            SetState(&TennisPlayerState_SlowUpBall::State_CutShot);
            break;

        default:
            break;
        }
    }
}


void TennisPlayerState_SlowUpBall::State_Smash()
{
    const RADIAN angle_speed = D3DXToRadian(15);
    const int SmashFrame = 20;
    const int EndFrame = 30;

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
        controller::SetVibration(
            10000,
            0.15f,
            m_pTennis->m_PlayerInfo.number
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
        Vector3 v = m_pControllClass->GetVec();

        chr_func::AngleControll(m_pTennis, m_pTennis->m_Params.pos + v, angle_speed);

    }

    //��莞�ԂŏI���X�e�[�g��
    if (m_Timer >= EndFrame)
    {
        SetState(&TennisPlayerState_SlowUpBall::State_Finish);
    }
}


void TennisPlayerState_SlowUpBall::State_CutShot()
{
    const RADIAN angle_speed = D3DXToRadian(10);
    const int ShothFrame = 8;
    const int EndFrame = 15;

    ++m_Timer;

    if (m_Timer == 1)
    {
        //���[�V����
        m_pTennis->m_Renderer.SetMotion(TennisPlayer::_mt_CutShot);
    }


    if (m_Timer == ShothFrame)
    {
        //�ł��グ�{�[��������
        m_pUpBall->m_DeleteFlag = true;
        m_pUpBall = nullptr;

        //�{�[������
        Vector3 pos, move;

        pos = m_pTennis->m_Params.pos + Vector3(0, BallBase::UsualBallShotY, 0);

        chr_func::GetFront(m_pTennis, &move);
        move *= 0.3f;
        move.y = 0.6f;

        new TennisBoundBall(
            pos,
            move,
            m_pTennis
            );


        //�R���g���[����U��
        controller::SetVibration(
            5000,
            0.15f,
            m_pTennis->m_PlayerInfo.number
            );

        //�T�E���h
        Sound::Play(Sound::Tennis_BallAtk);

        //�G�t�F�N�g
        {
            //�G�t�F�N�g�̐ݒ�
            new HitEffectObject(
                pos,
                Vector3Normalize(move),
                0.1f,
                0.1f,
                Vector3(1, 1, 1)
                );
        }
    }

    if (m_Timer < ShothFrame)
    {
        //�p�x����
        Vector3 v = m_pControllClass->GetVec();

        chr_func::AngleControll(m_pTennis, m_pTennis->m_Params.pos + v, angle_speed);

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
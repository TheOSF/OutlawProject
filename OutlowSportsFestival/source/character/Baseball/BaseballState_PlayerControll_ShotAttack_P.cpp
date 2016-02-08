
#include "BaseballPlayerState.h"
#include "Baseball_HitEvent.h"
#include "BaseballState_PlayerControll_ShotAttack_P.h"

#include "../CharacterFunction.h"
#include "../CharacterManager.h"

#include "../../Ball/UsualBall.h"
#include "../../Ball/MilderHoming.h"
#include "../../Damage/Damage.h"

#include "Sound/Sound.h"
#include "../../GameSystem/GameController.h"

#include "../../Effect/EffectFactory.h"

//�@�X�e�[�g�J�n
void BaseballState_PlayerControll_ShotAttack_P::Enter(BaseballPlayer* b)
{
    m_pChr = b;
    m_StateTimer = 0;

    m_pStateFunc = GetStateFunc(b->m_ChargeBallCount);
}


//�@�X�e�[�g���s
void BaseballState_PlayerControll_ShotAttack_P::Execute(BaseballPlayer* b)
{
    ++m_StateTimer;

    (this->*m_pStateFunc)();
    
    chr_func::XZMoveDown(b, 0.08f);

    chr_func::UpdateAll(b, &BaseballHitEvent(b));
    chr_func::CreateTransMatrix(b, &b->getNowModeModel()->m_TransMatrix);

}

//�@�X�e�[�g�I��
void BaseballState_PlayerControll_ShotAttack_P::Exit(BaseballPlayer* b)
{
    
}


void BaseballState_PlayerControll_ShotAttack_P::State_Throw1()
{
    if (m_StateTimer == 1)
    {
        m_pChr->SetMotion(baseball_player::_mb_Atk2_P);
    }

    if (m_StateTimer == 20)
    {
        Vector3 pos, move;

        pos = m_pChr->m_Params.pos;
        pos += chr_func::GetFront(m_pChr)*2.0f;
        pos.y = UsualBall::UsualBallShotY;

        move = chr_func::GetFront(m_pChr)*0.55f;

        CreateBall(pos, move);

        m_pChr->MinusChargeBallCount(1);

        //SE
        Sound::Play(Sound::Swing2);
    }

    if (m_StateTimer == 35)
    {
        m_pChr->SetState(BaseballState_PlayerControll_Move::GetPlayerControllMove(m_pChr));
    }

    m_pChr->ModelUpdate(1);
}

void BaseballState_PlayerControll_ShotAttack_P::State_Throw2()
{

    if (m_StateTimer == 1)
    {
        m_pChr->SetMotion(baseball_player::_mb_Atk1_P);
    }

    if (m_StateTimer == 15 || m_StateTimer == 30)
    {
        Vector3 pos, move;

        pos = m_pChr->m_Params.pos;
        pos += chr_func::GetFront(m_pChr)*2.0f;
        pos.y = UsualBall::UsualBallShotY;

        move = chr_func::GetFront(m_pChr)*0.9f;

        CreateBall(pos, move, m_StateTimer == 30, 5);

        m_pChr->MinusChargeBallCount(1);

        //SE
        Sound::Play(Sound::Swing2);
    }


    if (m_StateTimer == 15)
    {
        m_pChr->SetMotion(baseball_player::_mb_Atk2_P);
    }


    if (m_StateTimer == 40)
    {
        m_pChr->SetState(BaseballState_PlayerControll_Move::GetPlayerControllMove(m_pChr));
    }

    if (m_StateTimer < 30)
    {
        m_pChr->ModelUpdate(1.5f);
    }
    else
    {
        m_pChr->ModelUpdate(1.0f);
    }
}


void BaseballState_PlayerControll_ShotAttack_P::State_Throw3()
{
    if (m_StateTimer == 1)
    {
        m_pChr->SetMotion(baseball_player::_mb_Atk1_P);
    }

    if (m_StateTimer == 20)
    {
        RADIAN rotate_angles[]=
        {
            D3DXToRadian(30),
            0,
            -D3DXToRadian(30)
        };

        Vector3 pos, move;

        pos = m_pChr->m_Params.pos;
        pos += chr_func::GetFront(m_pChr)*2.0f;
        pos.y = UsualBall::UsualBallShotY;

        move = chr_func::GetFront(m_pChr)*0.95f;

        for (int i = 0; i < ARRAYSIZE(rotate_angles); ++i)
        {
            CreateBall(pos, Vector3RotateAxis(Vector3AxisY, rotate_angles[i], move), true, 12.0f);
        }

        m_pChr->MinusChargeBallCount(2);

        //SE
        Sound::Play(Sound::Beam2);
    }

    if (m_StateTimer == 35)
    {
        m_pChr->SetState(BaseballState_PlayerControll_Move::GetPlayerControllMove(m_pChr));
    }

    m_pChr->ModelUpdate(1);
}

void BaseballState_PlayerControll_ShotAttack_P::State_Throw4()
{
    if (m_StateTimer == 1)
    {
        m_pChr->SetMotion(baseball_player::_mb_Atk1_P);
    }

    if (m_StateTimer == 15 || m_StateTimer == 30)
    {
        RADIAN rotate_angles[]=
        {
            D3DXToRadian(10),
            D3DXToRadian(30)
        };

        Vector3 pos, move;

        pos = m_pChr->m_Params.pos;
        pos += chr_func::GetFront(m_pChr)*2.0f;
        pos.y = UsualBall::UsualBallShotY;

        move = chr_func::GetFront(m_pChr)*1.05f;

        for (int i = 0; i < 2; ++i)
        {
            CreateBall(
                pos,
                Vector3RotateAxis(Vector3AxisY, rotate_angles[i] * ((m_StateTimer == 15) ? (1) : (-1)), move),
                true,
                16
                );
        }

        m_pChr->MinusChargeBallCount(2);

        //SE
        Sound::Play(Sound::Beam2);
    }


    if (m_StateTimer == 15)
    {
        m_pChr->SetMotion(baseball_player::_mb_Atk2_P);
    }


    if (m_StateTimer == 40)
    {
        m_pChr->SetState(BaseballState_PlayerControll_Move::GetPlayerControllMove(m_pChr));
    }

    if (m_StateTimer < 30)
    {
        m_pChr->ModelUpdate(1.5f);
    }
    else
    {
        m_pChr->ModelUpdate(1.0f);
    }
}


void BaseballState_PlayerControll_ShotAttack_P::State_Throw5()
{


    if (m_StateTimer == 35)
    {
        m_pChr->SetState(BaseballState_PlayerControll_Move::GetPlayerControllMove(m_pChr));
    }

}


void BaseballState_PlayerControll_ShotAttack_P::CreateBall(CrVector3 pos, CrVector3 move, bool Strong, float DamageValue)
{
    //�{�[������
    BallBase::Params param;

    param.move = move;
    param.pos = pos;

    //�e��������
    param.pParent = m_pChr;
    //�ʏ�^�C�v
    param.type = BallBase::Type::_Usual;

    //����
    {
        UsualBall* pBall = new UsualBall(param, DamageBase::Type::_WeekDamage, DamageValue, UsualBall::GetUsualMoveControll());
            
        if (Strong)
        {
            pBall->Counter(m_pChr);
        }
    }

    //�R���g���[����U��
    chr_func::SetControllerShock(
        m_pChr,
        0.5f,
        0.15f
        );

    //�G�t�F�N�g
    EffectFactory::ShotEffect(
        m_pChr->m_PlayerInfo.number,
        param.pos,
        param.move
        );

    //�X�L���Q�[�W���Z
    chr_func::AddSkillGauge(m_pChr, UsualBall::AddSkillValueRatio);
}



void BaseballState_PlayerControll_ShotAttack_P::AngleControll()
{
    CharacterBase* pTarget = nullptr;
    const RADIAN AngleSpeed = D3DXToRadian(4);

    Vector3 ViewTargetPos = m_pChr->m_Params.pos;

    if (chr_func::CalcAtkTarget(m_pChr, D3DXToRadian(30), 100, &pTarget))
    {
        ViewTargetPos = pTarget->m_Params.pos;
    }
    else
    {
        if (chr_func::isPlayer(m_pChr))
        {
            Vector2 Stick = controller::GetStickValue(controller::stick::left, m_pChr->m_PlayerInfo.number);

            ViewTargetPos += Vector3(Stick.x, 0, Stick.y);
        }
        else
        {
            if (chr_func::CalcAtkTarget(m_pChr, PI, 100, &pTarget))
            {
                ViewTargetPos = pTarget->m_Params.pos;
            }
        }
    }

    chr_func::AngleControll(m_pChr, ViewTargetPos, AngleSpeed);
}

BaseballState_PlayerControll_ShotAttack_P::StateFunc 
    BaseballState_PlayerControll_ShotAttack_P::GetStateFunc(int ChargeCount)const
{
        switch (ChargeCount)
        {
        case 1:
            return &BaseballState_PlayerControll_ShotAttack_P::State_Throw1;

        case 2:
            return &BaseballState_PlayerControll_ShotAttack_P::State_Throw2;

        case 3:
            return &BaseballState_PlayerControll_ShotAttack_P::State_Throw3;

        case 4:
            return &BaseballState_PlayerControll_ShotAttack_P::State_Throw4;

        case 5:
            return &BaseballState_PlayerControll_ShotAttack_P::State_Throw5;

        default:
            MyAssert(false, "�`���[�W�J�E���g�����������I�I");
            break;
        }

        return &BaseballState_PlayerControll_ShotAttack_P::State_Throw1;
}
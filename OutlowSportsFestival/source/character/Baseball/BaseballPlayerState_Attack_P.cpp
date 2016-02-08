#include "BaseballPlayerState_Attack_P.h"
#include "BaseballPlayerState.h"
#include "../../GameSystem/GameController.h"
#include "../CharacterFunction.h"
#include "../CharacterManager.h"
#include "BaseballAttackInfo_UsualAtk.h"
#include "Baseball_HitEvent.h"
#include "../../Effect/EffectFactory.h"
#include "BaseballState_PlayerControll_ShotAttack_P.h"

const int Baseball_PlayerControll_Attack_P::m_sMaxCombo = 3;


//-------------�ߋ����U���X�e�[�g�N���X-------------

Baseball_PlayerControll_Attack_P::Baseball_PlayerControll_Attack_P(BaseballPlayer* b) :
m_pChr(b),
m_pStateFunc(&Baseball_PlayerControll_Attack_P::State_Atk),
m_NextType(NextType::_PreSelect),
m_StateTimer(0),
m_ComboCount(0)
{

}


Baseball_PlayerControll_Attack_P::~Baseball_PlayerControll_Attack_P()
{

}

// �X�e�[�g�J�n
void  Baseball_PlayerControll_Attack_P::Enter(BaseballPlayer* b)
{

}

// �X�e�[�g���s
void Baseball_PlayerControll_Attack_P::Execute(BaseballPlayer* b)
{
    m_StateTimer++;

    (this->*m_pStateFunc)();

}

// �X�e�[�g�I��
void Baseball_PlayerControll_Attack_P::Exit(BaseballPlayer* b)
{

}


void Baseball_PlayerControll_Attack_P::SetState(StateFunc state)
{
    m_pStateFunc = state;
    m_StateTimer = 0;
}

void Baseball_PlayerControll_Attack_P::UpdateNextType()
{
    if (m_pChr->m_PlayerInfo.player_type == PlayerType::_Player)
    {
        //�v���C���[�̏ꍇ
        if (controller::GetTRG(controller::button::shikaku, m_pChr->m_PlayerInfo.number))
        {
            m_NextType = NextType::DoCombo;
        }

        if (controller::GetTRG(controller::button::sankaku, m_pChr->m_PlayerInfo.number))
        {
            m_NextType = NextType::DoFarAtk;
        }
    }
    else
    {
        if (frand() < 0.25f)
        {
            m_NextType = NextType::None;
        }
        else if (frand() < 0.25f)
        {
            m_NextType = NextType::DoFarAtk;
        }
        else
        {
            m_NextType = NextType::DoCombo;
        }
    }
}

void Baseball_PlayerControll_Attack_P::AngleControll(RADIAN angle)
{
    CharacterBase* pTarget = nullptr;


    if (m_pChr->m_PlayerInfo.player_type == PlayerType::_Player)
    {
        //�v���C���[�̏ꍇ

        //���͈͂ɂ����ꍇ�͂��̃L�����N�^��_��
        if (chr_func::CalcAtkTarget(m_pChr, D3DXToRadian(33), 50, &pTarget))
        {
            chr_func::AngleControll(m_pChr, pTarget->m_Params.pos, angle);
        }
        else
        {
            //�X�e�B�b�N�ŕ����R���g���[��
            Vector2 stick = controller::GetStickValue(controller::stick::left, m_pChr->m_PlayerInfo.number);
            Vector3 pos = m_pChr->m_Params.pos + Vector3(stick.x, 0, stick.y);

            chr_func::AngleControll(m_pChr, pos, angle);
        }
    }
    else
    {
        //�����Ǐ]
        if (chr_func::CalcAtkTarget(m_pChr, PI, 120, &pTarget))
        {
            chr_func::AngleControll(m_pChr, pTarget->m_Params.pos, angle);
        }
    }
}

void Baseball_PlayerControll_Attack_P::State_Atk()
{
    const int Motions[m_sMaxCombo] =
    {
        baseball_player::_mb_Atk1_P,
        baseball_player::_mb_Atk2_P,
        baseball_player::_mb_Atk3_P,
    };

    const int ShotFrames[m_sMaxCombo] =
    {
        15,
        15,
        15,
    };

    const int EndFrames[m_sMaxCombo] =
    {
        33,
        33,
        45,
    };

    const int SwitchFrame = 25;
    const int CanButtonFrame = 18;
    
    //������
    if (m_StateTimer == 1)
    {
        m_pChr->SetMotion(Motions[m_ComboCount]);
    }

    //�ړ��l���_�E��
    chr_func::XZMoveDown(m_pChr, 0.05f);
    

    //�V���b�g�O�Ȃ�
    if (m_StateTimer < ShotFrames[m_ComboCount])
    {
        RADIAN angle_speed = D3DXToRadian(5);

        //�V���b�g�͂��߂Ȃ�p�x����������
        if (m_StateTimer < 5)
        {
            angle_speed = D3DXToRadian(30);
        }

        //�A���O���R���g���[��
        AngleControll(angle_speed);
    }

    //�V���b�g�I
    if (m_StateTimer == ShotFrames[m_ComboCount])
    {

        //�{�[������
        BallBase::Params param;

        //�ړ��͑O����
        chr_func::GetFront(m_pChr, &param.move);
        //�X�s�[�h�͓K��
        param.move *= 0.68f;

        //�r�̈ʒu��
        param.pos = m_pChr->getNowModeModel()->GetWorldBonePos(33);
        //�������L�������ʃ{�[�����˂�Y��
        param.pos.y = BallBase::UsualBallShotY;

        //�e��������
        param.pParent = m_pChr;
        //�ʏ�^�C�v
        param.type = BallBase::Type::_Usual;

        if (m_ComboCount == 2)
        {
            param.move *= 1.2f;

            //����
            new UsualBall(param, DamageBase::Type::_VanishDamage, 4, UsualBall::GetUsualMoveControll());
        }
        else
        {
            //����
            new UsualBall(param, DamageBase::Type::_WeekDamage, 4, UsualBall::GetUsualMoveControll());
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

    //�R���{�\�Ȃ�
    if (m_ComboCount < m_sMaxCombo - 1)
    {
        //�R���{�ڍs����̍X�V
        if (m_NextType == NextType::_PreSelect)
        {
            UpdateNextType();
        }

        //�R���{�ڍs
        if (m_StateTimer >= SwitchFrame)
        {
            switch (m_NextType)
            {
            case NextType::None:
            case NextType::_PreSelect:
                break;

            case NextType::DoCombo:
                SetState(&Baseball_PlayerControll_Attack_P::State_Atk);
                ++m_ComboCount;
                m_NextType = NextType::_PreSelect;
                break;

            case NextType::DoFarAtk:
                m_pChr->SetState(new BaseballState_PlayerControll_ShotAttack_P());
                break;
            }
        }
    }

    //���Ԍo�߂Œʏ�X�e�[�g��
    if (m_StateTimer >= EndFrames[m_ComboCount])
    {
        m_pChr->SetState(BaseballState_PlayerControll_Move::GetPlayerControllMove(m_pChr));
    }

    //�X�V
    {
        chr_func::UpdateAll(m_pChr, &BaseballHitEvent(m_pChr, m_StateTimer < ShotFrames[m_ComboCount]));
        chr_func::CreateTransMatrix(m_pChr, &m_pChr->getNowModeModel()->m_TransMatrix);

        m_pChr->ModelUpdate(1);
    }
}
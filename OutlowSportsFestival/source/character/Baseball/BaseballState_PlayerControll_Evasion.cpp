#include "BaseballState_PlayerControll_Evasion.h"
#include "../CharacterFunction.h"
#include "BaseballPlayerState.h"
#include "Baseball_HitEvent.h"
#include "../../Effect/EffectFactory.h"
#include "../../Sound/Sound.h"
#include "../../GameSystem/GameController.h"


BaseballState_Rolling::BaseballState_Rolling()
{
    m_Vec = Vector3Zero;
}

BaseballState_Rolling::BaseballState_Rolling(CrVector3 Vec)
{
    m_Vec = Vec;
}

BaseballState_Rolling::~BaseballState_Rolling()
{

}


// �X�e�[�g�J�n
void BaseballState_Rolling::Enter(BaseballPlayer* b)
{
	m_Timer = 0;

	//�f�t�H���g�̃��[�����O�����Ƃ��ăL�����̑O�x�N�g��������
    if (m_Vec.Length() < 0.1f)
    {
        chr_func::GetFront(b, &m_Vec);
    }

	//�L�����N�^�̈ړ��ʂ�������
	chr_func::ResetMove(b);

    SetParam(b);
}

// �X�e�[�g���s
void BaseballState_Rolling::Execute(BaseballPlayer* b)
{
	const int CanControllFrame = 4;   //�ړ��������R���g���[���ł���t���[��

	//�t���[���J�E���g�X�V
	++m_Timer;

	//���[�V�����Z�b�g
	if (m_Timer == 1)
	{
        b->SetMotion(m_Param.Motion);

        //�Y�U�[��
        Sound::Play(Sound::Sand1);
	}

	//�ړ��������R���g���[��
	if (m_Timer < CanControllFrame)
	{
        if (b->m_PlayerInfo.player_type == PlayerType::_Player)
        {
            m_Vec = GetPlayerControllMoveVec(b);
        }

        //�ړ��������R���g���[��
        chr_func::AngleControll(b, b->m_Params.pos + m_Vec, 0.3f);
	}

    //�ړ��l�����X�Ƀ_�E��
    chr_func::XZMoveDown(b, m_Param.MoveDownFrame);

	//�R���g���[���ł���t���[�����I������ꍇ�����ƈړ���ݒ�
	if (m_Timer == CanControllFrame)
	{
		chr_func::AngleControll(b, b->m_Params.pos + m_Vec);

		chr_func::AddMoveFront(b, m_Param.MoveValue, 100.0f);
	}



	//�I��
	if (m_Timer >= m_Param.AllFrame)
	{
		//�ʏ�X�e�[�g�Ɉڍs
		b->SetState(BaseballState_PlayerControll_Move::GetPlayerControllMove(b));
	}

	//���G�t�F�N�g
	{
		//�X�^�[�g��
		if (m_Timer == 2)
		{

            for (int i = 0; i < 15; ++i)
            {
                EffectFactory::SmokeParticle(
                    b->m_Params.pos + Vector3Rand()*0.1f,
                    Vector3(frand()*2.0f - 1.0f, frand(), frand()*2.0f - 1.0f)*0.04f,
                    40,
                    1.0f + frand()*1.0f,
                    0x0AFFFFFF
                    );
            }
		}

		//�O��
		if (m_Timer < 12)
		{
            Vector3 pos;

            pos =  b->getNowModeModel()->GetWorldBonePos(6);

			for (int i = 0; i < 1; ++i)
			{
                EffectFactory::SmokeParticle(
                    pos + Vector3Rand()*0.1f,
                    Vector3Zero,
                    20,
                    0.8f + frand()*0.2f * (1 - (float)m_Timer / 12.0f), 
					0x20FFFFFF
					);
			}
		}

        if (m_Timer ==  m_Param.StandStartFrame)
        {
            for (int i = 0; i < 15; ++i)
            {
                EffectFactory::SmokeParticle(
                    b->m_Params.pos + Vector3Rand()*0.1f,
                    Vector3(frand()*2.0f - 1.0f, frand(), frand()*2.0f - 1.0f)*0.04f,
                    40,
                    1.0f + frand()*1.0f,
                    0x0AFFFFFF
                    );
            }
        }

	}

	//��{�I�ȍX�V
	{
		DamageManager::HitEventBase NoDmgHitEvent;   //�m�[�_���[�W
		BaseballHitEvent            UsualHitEvent(b);//�ʏ�


		//���G�t���[�����ɂ���ăq�b�g�C�x���g�N���X�̕���
		if (m_Timer < m_Param.NoDamageFrame)
		{
			chr_func::UpdateAll(b, &NoDmgHitEvent);
		}
		else
		{
			chr_func::UpdateAll(b, &UsualHitEvent);
		}

		//���f���X�V
		b->ModelUpdate(1);

		//�s��X�V
        chr_func::CreateTransMatrix(b, &b->getNowModeModel()->m_TransMatrix);
	}
}

// �X�e�[�g�I��
void BaseballState_Rolling::Exit(BaseballPlayer* b)
{

}

Vector3 BaseballState_Rolling::GetPlayerControllMoveVec(BaseballPlayer* b)
{   
    Vector2 st = controller::GetStickValue(controller::stick::left, b->m_PlayerInfo.number);
    Vector3 ret(st.x, 0, st.y);

    return ret;
}

void BaseballState_Rolling::SetParam(BaseballPlayer* b)
{
    if (b->getBatterFlg())
    {
        m_Param.AllFrame = 40;
        m_Param.Motion = baseball_player::_mb_Evasion_B;
        m_Param.NoDamageFrame = 15;
        m_Param.StandStartFrame = 20;

        m_Param.MoveValue = 0.8f;
        m_Param.MoveDownFrame = 0.065f;
    }
    else
    {
        m_Param.AllFrame = 30;
        m_Param.Motion = baseball_player::_mb_Evasion_P;
        m_Param.NoDamageFrame = 10;
        m_Param.StandStartFrame = 14;

        m_Param.MoveValue = 0.8f;
        m_Param.MoveDownFrame = 0.065f;
    }
}
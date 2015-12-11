#include "TennisPlayerState_UsualMove.h"
#include "TennisState_BoundShot.h"
#include "TennisPlayerState_Counter.h"
#include "../../GameSystem/GameController.h"
#include "../CharacterFunction.h"
#include "../../Ball/UsualBall.h"
#include "../../Damage/Damage.h"
#include "../CharacterMoveClass.h"
#include "../CharacterShotAttackClass.h"
#include "Tennis_HitEvent.h"
#include "TennisPlayerState_Attack.h"
#include "TennisState_Rolling.h"

#include "Computer\TennisPlayerState_ComMove.h"
#include "../../Camera/Camera.h"
#include "TennisPlayerState_DamageMotionDie.h"
#include "TennisPlayerState_PoseMotion.h"

#include "../../Render/LightObject.h"
#include "TennisPlayerState_SlowUpBall.h"
#include "../../Effect/SoccerSpecialHitEffect.h"
#include "TennisSpecialAtk.h"
#include "TennisUtillityClass.h"


//****************************************************
//	�e�j�X�v���C���[�̑���N���X
//****************************************************


TennisState* TennisState_PlayerControll_Move::GetPlayerControllMove(
	TennisPlayer* pt)
{
	switch (pt->m_PlayerInfo.player_type)
	{
	case PlayerType::_Player:
		return new TennisState_PlayerControll_Move();

	case PlayerType::_Computer:

        return new TennisState_ComputerControll_Move();

		switch (pt->m_PlayerInfo.strong_type)
		{
		case StrongType::_Weak:
			//return new �ア�ʏ�ړ�
		case StrongType::_Usual:
			//return new ���ʂ̒ʏ�ړ�
		case StrongType::_Strong:
			//return new �����ʏ�ړ�
		default:break;
		}

	default:break;
	}

	assert("�ʏ�X�e�[�g���쐬�ł��Ȃ��L�����N�^�^�C�v�ł� TennisState_PlayerControll_Move::GetPlayerControllMove" && 0);
	return nullptr;
}


bool TennisState_PlayerControll_Move::SwitchGameState(TennisPlayer* pt)
{
    Vector3 v;

    switch (pt->GetStateType())
    {
    case CharacterBase::State::Usual:

        return false;

    case CharacterBase::State::Freeze:

        return true;

    case CharacterBase::State::LosePose:
        pt->SetState(new TennisState_PoseMotion(TennisPlayer::_mt_LosePose, 0.2f, 1000));
        return true;

    case CharacterBase::State::WinPose:
        pt->SetState(new TennisState_PoseMotion(TennisPlayer::_mt_WinPose, 0.2f, 1000));

        return true;
    default:
        break;
    }

    return false;


}

//�X�e�[�g�J�n
void TennisState_PlayerControll_Move::Enter(TennisPlayer* t)
{
	//�ړ��C�x���g�N���X
	class TennisMoveEvent :public CharacterUsualMove::MoveEvent
	{
		TennisPlayer* m_pTennis;
	public:
		TennisMoveEvent(TennisPlayer* pTennis) :
			m_pTennis(pTennis){}

		//�A�j���[�V�����̍X�V
		void Update(bool isRun, RATIO speed_ratio)
		{
			m_pTennis->m_Renderer.Update(1);
		}
		//����n�߂Ƀ��[�V�������Z�b�g
		void RunStart()
		{
			m_pTennis->m_Renderer.SetMotion(TennisPlayer::_mt_Run);
		}
		//�����͂��߂Ƀ��[�V�������Z�b�g
		void StandStart()
		{
			m_pTennis->m_Renderer.SetMotion(TennisPlayer::_mt_Stand);
		}

        //����I��胂�[�V�������Z�b�g
        void RunEnd()
        {
            m_pTennis->m_Renderer.SetMotion(TennisPlayer::_mt_RunEnd);
        }
	};

	//�ړ��p�����[�^����
	CharacterUsualMove::Params p;

	p.Acceleration = 0.15f;
	p.MaxSpeed = 0.28f;
	p.TurnSpeed = 0.3f;
	p.DownSpeed = 0.08f;
    p.RunEndFrame = 35;

	//�ړ��N���X�̍쐬
	m_pMoveClass = new CharacterUsualMove(
		t,
		p,
		new TennisMoveEvent(t),
		new TennisHitEvent(t)
		);

    //�����̂������[�V�����Z�b�g
    t->m_Renderer.SetMotion(TennisPlayer::_mt_Stand);
}


void TennisState_PlayerControll_Move::Execute(TennisPlayer* t)
{
    if (SwitchGameState(t) == false)
    {
        //�e�A�N�V�����ֈڍs�\
        ActionStateSwitch(t);

        //�X�e�B�b�N�̒l���Z�b�g
        Vector2 st = controller::GetStickValue(controller::stick::left, t->m_PlayerInfo.number);
        Vector3 st_vec3;

        //�r���[��Ԃɕϊ�
        st_vec3 = DefCamera.GetRight()*st.x + DefCamera.GetForward()*st.y;
        st.x = st_vec3.x;
        st.y = st_vec3.z;

        //�X�e�B�b�N�̒l�Z�b�g
        m_pMoveClass->SetStickValue(st);

    }
    else
    {
        //�X�e�B�b�N�̒l�Z�b�g
        m_pMoveClass->SetStickValue(Vector2(0, 0));
    }

    //�X�V
    m_pMoveClass->Update();


    //���f���̃��[���h�ϊ��s����X�V
    chr_func::CreateTransMatrix(t, t->m_ModelSize, &t->m_Renderer.m_TransMatrix);

}

void TennisState_PlayerControll_Move::Exit(TennisPlayer* t)
{
	delete m_pMoveClass;
}

void TennisState_PlayerControll_Move::ActionStateSwitch(TennisPlayer* t)
{
    if (controller::GetTRG(controller::button::sankaku, t->m_PlayerInfo.number))
    {// [��] �Ń{�[������
        t->SetState(new TennisPlayerState_SlowUpBall(new TennisUtillityClass::PlayerSlowBallControll(t)));
        return;
    }

    if (chr_func::isCanSpecialAttack(t) && controller::GetTRG(controller::button::maru, t->m_PlayerInfo.number))
    {// [��] �ŕK�E�Z
        t->SetState(new TennisState_SpecialAtk(t));
        chr_func::ResetSkillGauge(t);
        return;
    }

    if (controller::GetTRG(controller::button::shikaku, t->m_PlayerInfo.number))
    {// [��] �� [�ߋ����U��]
        t->SetState(new TennisState_PlayerControll_Attack(t, false));
        return;
    }

    if (controller::GetTRG(controller::button::batu, t->m_PlayerInfo.number))
    {// [�~] �� [���[�����O]
        t->SetState(new TennisState_Rolling(new TennisUtillityClass::PlayerRollingControll(t)));
        return;
    }

    if (controller::GetTRG(controller::button::_R1, t->m_PlayerInfo.number))
    {// [R1] �� [�J�E���^�[]
        t->SetState(new TennisState_PlayerControll_Counter());
        return;
    }

    if (controller::GetTRG(controller::button::_L1, t->m_PlayerInfo.number) &&
        t->isCanBoundBallAtk())
    {// [L1] �� �{�[������ɓ�����U��
        t->SetState(new TennisState_BoundShot(new TennisUtillityClass::PlayerShotControllClass(t)));
        return;
    }
}
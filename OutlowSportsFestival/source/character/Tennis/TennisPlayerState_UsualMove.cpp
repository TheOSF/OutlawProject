#include "TennisPlayerState_UsualMove.h"
#include "TennisState_Shot.h"
#include "TennisPlayerState_Counter.h"
#include "../../GameSystem/GameController.h"
#include "../CharacterFunction.h"
#include "../../Ball/UsualBall.h"
#include "../../Damage/Damage.h"
#include "../CharacterCounterClass.h"
#include "../CharacterMoveClass.h"
#include "../CharacterShotAttackClass.h"
#include "Tennis_HitEvent.h"
#include "TennisPlayerState_Attack.h"
#include "TennisState_Rolling.h"
#include "TennisState_BoundBallAtk.h"

#include "Computer\TennisPlayerState_ComMove.h"
#include "../../Camera/Camera.h"


//���[�����O�̍��Z����N���X
class PlayerRollingControll :public TennisState_Rolling::CallBackClass
{
public:
    TennisPlayer*const pt;

    PlayerRollingControll(TennisPlayer* pt) :pt(pt){}


    Vector3 GetVec()override
    {
        Vector2 stick = controller::GetStickValue(controller::stick::left, pt->m_PlayerInfo.number);
        Vector3 vec(stick.x, 0, stick.y);

        if (vec.Length() < 0.25f)
        {
            return Vector3Zero;
        }

        vec = DefCamera.GetRight()*vec.x + DefCamera.GetUp()*vec.z;
        vec.Normalize();

        return vec;
    }
};


//�o�E���h�U���̕�������N���X
class PlayerBoundBallControll :public TennisState_BoundBallAtk::ControllClass
{
public:
    TennisPlayer*const pt;

    PlayerBoundBallControll(TennisPlayer* pt) :pt(pt){}

    Vector3 GetBoundVec()override
    {
        Vector2 stick = controller::GetStickValue(controller::stick::left, pt->m_PlayerInfo.number);
        Vector3 vec(stick.x, 0, stick.y);

        if (vec.Length() < 0.25f)
        {
            return Vector3Zero;
        }

        vec = DefCamera.GetRight()*vec.x + DefCamera.GetUp()*vec.z;
        vec.Normalize();

        return vec;
    }
};


//�V���b�g���̃R���g���[���N���X
class PlayerShotControllClass :public TennisState_Shot::ControllClass
{
    TennisPlayer* const   m_pTennis;
public:
    PlayerShotControllClass(TennisPlayer* pTennis) :
        m_pTennis(pTennis){}

    Vector3 GetVec()
    {
        Vector2 stick = controller::GetStickValue(controller::stick::left, m_pTennis->m_PlayerInfo.number);
        Vector3 vec(stick.x, 0, stick.y);

        if (vec.Length() < 0.25f)
        {
            return Vector3Zero;
        }

        vec = DefCamera.GetRight()*vec.x + DefCamera.GetUp()*vec.z;
        vec.Normalize();

        return vec;
    }

    bool DoOtherAction()
    {
        TennisPlayer * const t = m_pTennis;

        if (controller::GetTRG(controller::button::shikaku, t->m_PlayerInfo.number))
        {// [��] �� [�ߋ����U��]
            t->SetState(new TennisState_PlayerControll_Attack(t));
            return true;
        }

        if (controller::GetTRG(controller::button::batu, t->m_PlayerInfo.number))
        {// [�~] �� [���[�����O]
            t->SetState(new TennisState_Rolling(new PlayerRollingControll(t)));
            return true;
        }

        if (controller::GetTRG(controller::button::_R1, t->m_PlayerInfo.number))
        {// [R1] �� [�J�E���^�[]
            t->SetState(new TennisState_PlayerControll_Counter());
            return true;
        }

        if (controller::GetTRG(controller::button::_L1, t->m_PlayerInfo.number))
        {// [L1] �� [�o�E���h�{�[���U��
            t->SetState(new TennisState_BoundBallAtk(new PlayerBoundBallControll(t)));
            return true;
        }

        return false;
    }

    bool DoShotAfterAction()
    {
        TennisPlayer * const t = m_pTennis;

        if (controller::GetTRG(controller::button::shikaku, t->m_PlayerInfo.number))
        {// [��] �� [�ߋ����U��]
            t->SetState(new TennisState_PlayerControll_Attack(t));
            return true;
        }

        return false;
    }
};

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
	};

	//�ړ��p�����[�^����
	CharacterUsualMove::Params p;

	p.Acceleration = 0.2f;
	p.MaxSpeed = 0.2f;
	p.TurnSpeed = 0.3f;
	p.DownSpeed = 0.2f;

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



    if (controller::GetTRG(controller::button::sankaku, t->m_PlayerInfo.number))
    {// [��] �Ń{�[������
        t->SetState(new TennisState_Shot(new PlayerShotControllClass(t)));
        return;
    }

    if (controller::GetTRG(controller::button::shikaku, t->m_PlayerInfo.number))
    {// [��] �� [�ߋ����U��]
        t->SetState(new TennisState_PlayerControll_Attack(t));
        return;
    }

    if (controller::GetTRG(controller::button::batu, t->m_PlayerInfo.number))
    {// [�~] �� [���[�����O]
        t->SetState(new TennisState_Rolling(new PlayerRollingControll(t)));
        return;
    }

    if (controller::GetTRG(controller::button::_R1, t->m_PlayerInfo.number))
    {// [R1] �� [�J�E���^�[]
        t->SetState(new TennisState_PlayerControll_Counter());
        return;
    }

    if (controller::GetTRG(controller::button::_L1, t->m_PlayerInfo.number) &&
        t->isCanBoundBallAtk())
    {// [L1] �� [�o�E���h�{�[���U��
        t->SetState(new TennisState_BoundBallAtk(new PlayerBoundBallControll(t)));
        return;
    }

    {
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

    //�X�V
    m_pMoveClass->Update();


    //���f���̃��[���h�ϊ��s����X�V
    chr_func::CreateTransMatrix(t, t->m_ModelSize, &t->m_Renderer.m_TransMatrix);
}

void TennisState_PlayerControll_Move::Exit(TennisPlayer* t)
{
	delete m_pMoveClass;
}



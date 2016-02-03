#include "BaseballPlayerState.h"


#include "BaseballState_PlayerControll_Evasion.h"
#include "BaseballState_PlayerControll_ShotAttack_B.h"
#include "BaseballState_PlayerControll_ShotAttack_P.h"
#include "BaseballPlayerState_Attack_B.h"
#include "BaseballPlayerState_Attack_P.h"
#include "BaseballPlayerState_Counter.h"
#include "BaseballState_SPAttack_B.h"
#include "BaseballState_SPAttack_P.h"
#include "BaseballState_PoseMotion.h"
#include "BaseballState_Change.h"
#include "Baseball_HitEvent.h"

#include "Computer/BaseballPlayerState_ComMove.h"

#include "../../GameSystem/GameController.h"
#include "../CharacterFunction.h"
#include "../CharacterManager.h"
#include "../CharacterEvasionClass.h"

#include "../../Ball/UsualBall.h"
#include "../../Ball/MilderHoming.h"
#include "../../Damage/Damage.h"
#include "../../Camera/Camera.h"

//�V���b�g���̃R���g���[���N���X

Vector3 PlayerShotControllClass_B::GetVec()
{
	Vector2 stick = controller::GetStickValue(controller::stick::left, m_pBaseball->m_PlayerInfo.number);
	Vector3 vec(stick.x, 0, stick.y);

	if (vec.Length() < 0.25f)
	{
		return Vector3Zero;
	}

	vec = DefCamera.GetRight()*vec.x + DefCamera.GetUp()*vec.z;
	vec.Normalize();

	return vec;
}

bool PlayerShotControllClass_B::DoOtherAction()
{
	BaseballPlayer * const b = m_pBaseball;

	//�@�ߋ����U��[��]
	if (controller::GetTRG(controller::button::shikaku, b->m_PlayerInfo.number))
	{
		b->SetState(new Baseball_PlayerControll_Attack_B(b));
		return true;
	}
	//�@����s��[�~]
	if (controller::GetTRG(controller::button::batu, b->m_PlayerInfo.number))
	{
		b->SetState(new BaseballState_Rolling());
		return true;
	}

	// �J�E���^�[[R1]
	if (controller::GetTRG(controller::button::_R1, b->m_PlayerInfo.number))
	{
		b->SetState(new  BaseballState_PlayerControll_Counter());
		return true;
	}

	return false;
}

bool PlayerShotControllClass_B::DoShotAfterAction()
{
	BaseballPlayer * const b = m_pBaseball;

	if (controller::GetTRG(controller::button::shikaku, b->m_PlayerInfo.number))
	{// [��] �� [�ߋ����U��]
		b->SetState(new Baseball_PlayerControll_Attack_B(b));
		return true;
	}

	return false;
}




//***************************************
//�@�ړ�
//***************************************

BaseballState* BaseballState_PlayerControll_Move::GetPlayerControllMove(BaseballPlayer* pt)
{
	switch (pt->m_PlayerInfo.player_type)
	{
	case PlayerType::_Player:
		return new BaseballState_PlayerControll_Move();

	case PlayerType::_Computer:
		return new BaseballPlayerState_ComMove();
		switch (pt->m_PlayerInfo.strong_type)
		{
		case StrongType::_Weak:
			//return new �ア�ʏ�ړ�
		case StrongType::_Usual:
			//return new �ア�ʏ�ړ�
		case StrongType::_Strong:
			
		default:break;
		}
	default:break;
	}

	assert("�ʏ�X�e�[�g���쐬�ł��Ȃ��L�����N�^�^�C�v�ł� BaseballState_PlayerControll_Move::GetPlayerControllMove" && 0);
	return nullptr;
}

bool BaseballState_PlayerControll_Move::SwitchGameState(BaseballPlayer* pb)
{
	Vector3 v;

	switch (pb->GetStateType())
	{
	case CharacterBase::State::Usual:

		return false;

	case CharacterBase::State::Freeze:

		return true;

	case CharacterBase::State::LosePose:
        pb->SetMode(true);
		pb->SetState(new BaseballState_PoseMotion(baseball_player::_mb_LosePose, 0.2f, 1000));
		return true;

	case CharacterBase::State::WinPose:
        pb->SetMode(true);
		pb->SetState(new BaseballState_PoseMotion(baseball_player::_mb_WinPose, 0.2f, 1000));

		return true;
	default:
		break;
	}

	return false;


}
//�@�X�e�[�g�J�n
void BaseballState_PlayerControll_Move::Enter(BaseballPlayer* b)
{
	//�@�ړ��N���X�̍쐬
	m_pMoveClass = new CharacterUsualMove(
		b,
		BaseballMoveEvent::GetMoveParams(),
		new BaseballMoveEvent(b),
		new BaseballHitEvent(b)
		);


	
}


//�@�X�e�[�g���s
void BaseballState_PlayerControll_Move::Execute(BaseballPlayer* b)
{

	if (SwitchGameState(b) == false)
	{
		//�@�X�e�B�b�N�̒l���擾
		Vector2 st = controller::GetStickValue(controller::stick::left, b->m_PlayerInfo.number);

		//�@�؂�ւ�

		SetBatterFlg(b);
		//�@���s�p�^�[��
		if (batterflg){
			//�@�o�b�^�[��
			Batter(b);
		}
		else{
			//�@���莞
			Pitcher(b);
		}
		//�@�X�e�B�b�N�̒l�Z�b�g
		m_pMoveClass->SetStickValue(st);
	}
	else
	{
		//�X�e�B�b�N�̒l�Z�b�g
		m_pMoveClass->SetStickValue(Vector2(0, 0));
	}

	//�@�X�V
	m_pMoveClass->Update();
	//�@���f���̃��[���h�ϊ��s����X�V
    chr_func::CreateTransMatrix(b, &b->getNowModeModel()->m_TransMatrix);

}

//�@�X�e�[�g�̏I��
void BaseballState_PlayerControll_Move::Exit(BaseballPlayer* b)
{
	delete m_pMoveClass;
}

//�@�o�b�^�[��
void BaseballState_PlayerControll_Move::Batter(BaseballPlayer* b){
	//�@�������U��[��]
	if (controller::GetTRG(controller::button::sankaku, b->m_PlayerInfo.number)){
		b->SetState(new BaseballState_PlayerControll_ShotAttack_B(new PlayerShotControllClass_B(b)));
		return;
	}
	//�@�ߋ����U��[��]
	if (controller::GetTRG(controller::button::shikaku, b->m_PlayerInfo.number)){
		b->SetState(new Baseball_PlayerControll_Attack_B(b));
		return;
	}
	//�@����s��[�~]
	if (controller::GetTRG(controller::button::batu, b->m_PlayerInfo.number)){
		b->SetState(new BaseballState_Rolling());
		return;
	}



	//�@�K�E�łĂ�Ȃ�
    if (chr_func::isCanSpecialAttack(b))
    {
        //�@�K�E�Z[��]
        if (controller::GetTRG(controller::button::maru, b->m_PlayerInfo.number)){
            b->SetState(new BaseballState_SPAttack_B(b));
            return;
        }
    }
	// �J�E���^�[[R1]
	if (controller::GetTRG(controller::button::_R1, b->m_PlayerInfo.number))
	{
		b->SetState(new  BaseballState_PlayerControll_Counter());
		return;
	}
	// �؂�ւ�[L1]
	if (controller::GetTRG(controller::button::_L1, b->m_PlayerInfo.number))
	{
		b->SetState(new BaseballState_Change());
		return;
	}
}

//�@���莞
void  BaseballState_PlayerControll_Move::Pitcher(BaseballPlayer* b){
	//�@�������U��[��]
	if (controller::GetTRG(controller::button::sankaku, b->m_PlayerInfo.number)){
		b->SetState(new BaseballState_PlayerControll_ShotAttack_P());
		return;
	}
	//�@�ߋ����U��[��]
	if (controller::GetTRG(controller::button::shikaku, b->m_PlayerInfo.number)){
		b->SetState(new Baseball_PlayerControll_Attack_P(b));
		return;
	}
	//�@����s��[�~]
	if (controller::GetTRG(controller::button::batu, b->m_PlayerInfo.number)){
		b->SetState(new BaseballState_Rolling());
		return;
	}

	//�@�K�E�łĂ�Ȃ�
    if (chr_func::isCanSpecialAttack(b))
    {
        //�@�K�E�Z[��]
        if (controller::GetTRG(controller::button::maru, b->m_PlayerInfo.number)){
            b->SetState(new BaseballState_SPAttack_P());
            return;
        }
    }



	// �J�E���^�[[R1]
	if (controller::GetTRG(controller::button::_R1, b->m_PlayerInfo.number))
	{
		b->SetState(new  BaseballState_PlayerControll_Counter());
		return;
	}
	// �؂�ւ�[L1]
	if (controller::GetTRG(controller::button::_L1, b->m_PlayerInfo.number))
	{
		b->SetState(new BaseballState_Change());
		return;
	}
}


//----------------------------------------------------------------------------------

BaseballMoveEvent::BaseballMoveEvent(BaseballPlayer* pBaseball) :
m_pBaseball(pBaseball){}

CharacterUsualMove::Params BaseballMoveEvent::GetMoveParams()
{
    CharacterUsualMove::Params ret;

    ret.Acceleration = 0.15f;
    ret.MaxSpeed = 0.32f;
    ret.TurnSpeed = 0.3f;
    ret.DownSpeed = 0.08f;
    ret.RunEndFrame = 35;

    return ret;
}

//�@�A�j���[�V�����X�V
void BaseballMoveEvent::Update(bool isRun, RATIO speed_ratio)
{
    m_pBaseball->ModelUpdate(m_pBaseball->isBatter() ? (0.5f) : (1.0f));
}

//�@����n�߂Ƀ��[�V�����Z�b�g
void BaseballMoveEvent::RunStart(){
    //�@���s�p�^�[��
    if (m_pBaseball->getBatterFlg())
    {
        m_pBaseball->SetMotion(baseball_player::_mb_Run_B);
    }
    else
    {
        m_pBaseball->SetMotion(baseball_player::_mb_Run_P);
    }
}

//�@�����n�߂Ƀ��[�V�����Z�b�g
void BaseballMoveEvent::StandStart(){
    if (m_pBaseball->getBatterFlg())
    {
        m_pBaseball->SetMotion(baseball_player::_mb_Stand_B);
    }
    else
    {
        m_pBaseball->SetMotion(baseball_player::_mb_Stand_P);
    }
}


//����I���
void BaseballMoveEvent::RunEnd()
{
    if (m_pBaseball->getBatterFlg())
    {
        m_pBaseball->SetMotion(baseball_player::_mb_Stop_B);
    }
    else
    {
        m_pBaseball->SetMotion(baseball_player::_mb_Stop_P);
    }
}

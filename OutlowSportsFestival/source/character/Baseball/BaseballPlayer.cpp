#include "BaseballPlayer.h"
#include "BaseballPlayerState.h"
#include "Computer\/BaseballPlayerState_ComMove.h"
#include "BaseballState_Change.h"
#include "../../Damage/Damage.h"
#include "../../GameSystem/GameController.h"
#include "../../Ball/UsualBall.h"
#include "../../Effect/EffectFactory.h"
#include "Sound/Sound.h"
#include "../CharacterFunction.h"
#include "../CharacterManager.h"

//*************************************************************
//		�싅�v���C���[�N���X
//*************************************************************

//�@�R���X�g���N�^
BaseballPlayer::BaseballPlayer(const CharacterBase::PlayerInfo& info) :
CharacterBase(info), batterflg(true), m_ModelSize(0.05f), changetime(20),
m_Renderer(new  BlendAnimationMesh("DATA\\CHR\\BaseBall\\player_B.iem"))
{
	m_pStateMachine = new BaseballStateMachine(this);

	//�@�̗͒ቺ(�f�o�b�N�p)
	m_Params.maxHP = m_Params.HP = 100;
}

//�@�f�X�g���N�^
BaseballPlayer::~BaseballPlayer()
{
	delete m_pStateMachine;
}

//�@�X�e�[�g�Z�b�g
void BaseballPlayer::SetState(BaseballState* state)
{
	//�@�V�����X�e�[�g���Z�b�g
	m_pStateMachine->set_state(state);
}

//�@�X�V
bool BaseballPlayer::Update()
{
	//�@�؂�ւ��\���ԑ���
	changetime++;
	// �X�e�[�g���s
	m_pStateMachine->state_execute();

	//�L�����N�^��{�X�V
	BaseUpdate();

	return true;	//���true��Ԃ��Ə�������Ȃ�
}


bool  BaseballPlayer::CharacterMsg(MsgType mt)
{
	//�@���E���h���ƂɃ��Z�b�g
	if (mt == MsgType::_RoundReset)
	{
		Riset();
	}

	return m_pStateMachine->Msg(mt);
}


//�@���Z�b�g
void BaseballPlayer::Riset()
{
	SetState(BaseballState_PlayerControll_Move::GetPlayerControllMove(this));
	

	m_Renderer.SetMotion(baseball_player::_mb_Stand);
	m_Renderer.Update(0);
	ResetRound();
	batterflg = true;
	changetime = 20;

}
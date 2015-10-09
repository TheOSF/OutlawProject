#include "SoccerPlayer.h"
#include "SoccerPlayerState.h"
#include "../../Damage/Damage.h"
#include "../../GameSystem/GameController.h"
#include "../../Ball/UsualBall.h"

#include "../CharacterFunction.h"
#include "../CharacterManager.h"

//*************************************************************
//		�T�b�J�[�v���C���[�N���X
//*************************************************************

SoccerPlayer::SoccerPlayer(const CharacterBase::PlayerInfo& info) :
CharacterBase(info), m_ModelSize(0.05f),
m_Renderer(new BlendAnimationMesh("DATA\\CHR\\Soccer\\Player_S.iem"))
{
	m_pStateMachine = new SoccerStateMachine(this);
	m_Renderer.SetMotion(0);
}

SoccerPlayer::~SoccerPlayer()
{
	delete m_pStateMachine;
}

void SoccerPlayer::SetState(SoccerState* state)
{
	//�V�����X�e�[�g���Z�b�g
	m_pStateMachine->set_state(state);
}


bool SoccerPlayer::Update()
{
	//�X�e�[�g���s
	m_pStateMachine->state_execute();

	return true;	//���true��Ԃ��Ə�������Ȃ�
}

bool SoccerPlayer::Msg(MsgType mt)
{
	if (mt == MsgType::_RoundReset)
	{
		SetState(SoccerState_PlayerControll_Move::GetPlayerControllMove(this));
		m_Renderer.SetMotion(_ms_Stand);
		m_Renderer.Update(0);
		ResetRound();
	}
	return m_pStateMachine->Msg(mt);
}

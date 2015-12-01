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
const SoccerPlayer::SkillParam SoccerPlayer::skillparams = 

{
	0.05f,
	0.05f,
	0.05f,
	0.02f


};


SoccerPlayer::SoccerPlayer(const CharacterBase::PlayerInfo& info) :
CharacterBase(info, new BlendAnimationMesh("DATA\\CHR\\Soccer\\Player_S.iem")),
m_ModelSize(0.05f)
{
	
	m_pStateMachine = new SoccerStateMachine(this);
	m_Renderer.SetMotion(0);
	SetState(SoccerState_PlayerControll_Move::GetPlayerControllMove(this));
	//�@�̗͒ቺ(�f�o�b�N�p)
	m_Params.maxHP = m_Params.HP = 100;
}

SoccerPlayer::~SoccerPlayer()
{
	delete m_pStateMachine;
}

void SoccerPlayer::SetState(SoccerState* state,bool Important)
{
	//�V�����X�e�[�g���Z�b�g
	m_pStateMachine->set_state(state,Important);
}


bool SoccerPlayer::Update()
{
	//�X�e�[�g���s
	m_pStateMachine->state_execute();

	//�L�����N�^��{�X�V
	BaseUpdate();

	return true;	//���true��Ԃ��Ə�������Ȃ�
}

bool SoccerPlayer::CharacterMsg(MsgType mt)
{
	if (mt == MsgType::_RoundReset)
	{
		SetState(SoccerState_PlayerControll_Move::GetPlayerControllMove(this));
		m_Renderer.SetMotion(_ms_Stand);
		m_Renderer.Update(0);
	}
	return m_pStateMachine->Msg(mt);
}

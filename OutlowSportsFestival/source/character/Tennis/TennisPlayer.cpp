#include "TennisPlayer.h"
#include "../../Damage/Damage.h"
#include "../CharacterManager.h"
#include "../CharacterFunction.h"
#include "TennisPlayerState_UsualMove.h"

//*************************************************************
//		�e�j�X�v���C���[�N���X
//*************************************************************

TennisPlayer::TennisPlayer(const CharacterBase::PlayerInfo& info) :
CharacterBase(info),
m_Renderer(new BlendAnimationMesh("DATA\\CHR\\Tennis_player\\Player_T.iem")),
m_ModelSize(0.05f),
m_DontBoundBallAtkTimer(0)
{
	m_pStateMachine = new TennisStateMachine(this);

    m_Params.maxHP = m_Params.HP = 100;
}

TennisPlayer::~TennisPlayer()
{
	delete m_pStateMachine;
}

void TennisPlayer::SetState(TennisState* state)
{
	//�V�����X�e�[�g���Z�b�g
	m_pStateMachine->set_state(state);
}


bool TennisPlayer::Update()
{
	//�X�e�[�g���s
	m_pStateMachine->state_execute();

    if (m_DontBoundBallAtkTimer > 0)
    {
        --m_DontBoundBallAtkTimer;
    }

    //�L�����N�^��{�X�V
    BaseUpdate();

	return true;	//���true��Ԃ��Ə�������Ȃ�
}

bool TennisPlayer::CharacterMsg(MsgType mt)
{
    if (mt == MsgType::_RoundReset)
    {
        SetState(TennisState_PlayerControll_Move::GetPlayerControllMove(this));
        m_Renderer.SetMotion(_mt_Stand);
        m_Renderer.Update(0);
    }

	return m_pStateMachine->Msg(mt);
}

bool TennisPlayer::isCanBoundBallAtk()
{
    return m_DontBoundBallAtkTimer == 0;
}

void TennisPlayer::SetDontBoundBallAtkTimer()
{
    m_DontBoundBallAtkTimer = 5;
}


#include "SoccerPlayer.h"
#include "SoccerPlayerState.h"
#include "../../Damage/Damage.h"
#include "../../GameSystem/GameController.h"
#include "../../Ball/UsualBall.h"

#include "../CharacterFunction.h"
#include "../CharacterManager.h"

//*************************************************************
//		サッカープレイヤークラス
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
	//新しいステートをセット
	m_pStateMachine->set_state(state);
}


bool SoccerPlayer::Update()
{
	//ステート実行
	m_pStateMachine->state_execute();

	return true;	//常にtrueを返すと消去されない
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

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
const SoccerPlayer::SkillParam SoccerPlayer::skillparams = 

{
	0.05f,
	0.05f,
	0.05f,
	0.02f


};


SoccerPlayer::SoccerPlayer(const CharacterBase::PlayerInfo& info) :
CharacterBase(info, new BlendAnimationMesh(GetCharacterModelPath(CharacterType::_Soccer)))
{
	
	m_pStateMachine = new SoccerStateMachine(this);
	m_Renderer.SetMotion(0);
	SetState(SoccerState_PlayerControll_Move::GetPlayerControllMove(this));
	//　体力低下(デバック用)
	m_Params.maxHP = m_Params.HP = 100;

    m_ModelSize = 0.05f;
}

SoccerPlayer::~SoccerPlayer()
{
	delete m_pStateMachine;
}

bool SoccerPlayer::SetState(SoccerState* state,int Important)
{
	//新しいステートをセット
	return m_pStateMachine->set_state(state,Important);
}


bool SoccerPlayer::Update()
{
	//ステート実行
	m_pStateMachine->state_execute();

	//キャラクタ基本更新
	BaseUpdate();

	return true;	//常にtrueを返すと消去されない
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

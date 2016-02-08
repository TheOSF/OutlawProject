#include "TennisPlayer.h"
#include "../../Damage/Damage.h"
#include "../CharacterManager.h"
#include "../CharacterFunction.h"
#include "TennisPlayerState_UsualMove.h"

//*************************************************************
//		テニスプレイヤークラス
//*************************************************************

TennisPlayer::TennisPlayer(const CharacterBase::PlayerInfo& info) :
CharacterBase(info, new BlendAnimationMesh(GetCharacterModelPath(CharacterType::_Tennis))),
m_DontBoundBallAtkTimer(0)
{
	m_pStateMachine = new TennisStateMachine(this);

    m_Params.maxHP = m_Params.HP = 90; 

    m_ModelSize = 0.055f;
}

TennisPlayer::~TennisPlayer()
{
	delete m_pStateMachine;
}

bool TennisPlayer::SetState(TennisState* state, int Important)
{
	//新しいステートをセット
    return m_pStateMachine->set_state(state, Important);
}


bool TennisPlayer::Update()
{
	//ステート実行
	m_pStateMachine->state_execute();

    if (m_DontBoundBallAtkTimer > 0)
    {
        --m_DontBoundBallAtkTimer;
    }

    //キャラクタ基本更新
    BaseUpdate();

	return true;	//常にtrueを返すと消去されない
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
    m_DontBoundBallAtkTimer = 12;
}


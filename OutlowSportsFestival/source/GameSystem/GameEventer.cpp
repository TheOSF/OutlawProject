#include "GameEventer.h"
#include "../Camera/CameraState.h"
#include "../character/CharacterManager.h"
#include "../character/CharacterBase.h"
#include "../character/CharacterFunction.h"


//----------------------------------------------------
//  試合遷移メッセージを送信するクラス
//----------------------------------------------------

GameEventer::GameEventer(const Param& p, State* pInitState) :
m_Param(p)
{
	m_pStateMachine = new StateMachine(this);

	if (pInitState != nullptr)
	{
		m_pStateMachine->set_state(pInitState);
	}
}

GameEventer::~GameEventer()
{
	delete m_pStateMachine;
}

void GameEventer::SetState(State* pState)
{
	m_pStateMachine->set_state(pState);
}

bool GameEventer::Update()
{
	m_pStateMachine->state_execute();
	
	return true;
}

bool GameEventer::Msg(MsgType mt)
{

	return false;
}


//--------------------------------------------------
//         試合開始のカウントダウン
//--------------------------------------------------

MatchState::StartCountdown::StartCountdown()
{
	m_Timer = 0;
}

MatchState::StartCountdown::~StartCountdown()
{

}

//キャラクタをそれぞれ移すカメラを設定

void MatchState::StartCountdown::SetCamera()
{

	const CharacterManager::CharacterMap& ChrMap = DefCharacterMgr.GetCharacterMap();
	CameraState* pSetState, *pNext;

	Vector3 pos, target, front;

	DefCamera.m_Position = pos;
	DefCamera.m_Target = target;

	//最後のキャラクタから順番にカメラを生成
	for (int i = 3; i >= 0; --i)
	{
		for (auto& it : ChrMap)
		{
			if ((int)it.first->m_PlayerInfo.number != i)
			{
				continue;
			}

			//カメラ位置を設定
			pos = target = it.first->m_Params.pos;

			target.y += 3.0f;
			pos.y += 3.5f;

			//位置をキャラクタのちょっと前方に
			chr_func::GetFront(it.first, &front);
			pos += front*8.0f;

			//最後のカメラだった場合は試合カメラを次に設定
			if (i == 3)
			{
				pNext = new CameraStateGamePlay();
			}
			else
			{
				pNext = pSetState;
			}

			pSetState = new CameraStateMovetoPoint(
				pos,
				target,
				0.8f,
				80,
				CameraStateMovetoPoint::CalcType::_Linear,
				pNext
				);
		}
	}

	DefCamera.SetNewState(pSetState);
}

void MatchState::StartCountdown::Enter(_Client_type_ptr p)
{
	//キャラクタをそれぞれ移すカメラを設定
	SetCamera();

	//５秒
	m_MaxTime = 60 * 5;

}

void MatchState::StartCountdown::Execute(_Client_type_ptr p)
{
	if (++m_Timer > m_MaxTime)
	{
		p->SetState(new MatchPlay());
	}
}

void MatchState::StartCountdown::Exit(_Client_type_ptr p)
{

}

//--------------------------------------------------
//              試合中のステート
//--------------------------------------------------

MatchState::MatchPlay::MatchPlay():
m_Frame(0)
{

}

MatchState::MatchPlay::~MatchPlay()
{

}

void MatchState::MatchPlay::Enter(_Client_type_ptr p)
{

}

void MatchState::MatchPlay::Execute(_Client_type_ptr p)
{
	const UINT liveCount = DefCharacterMgr.GetCharacterLiveCount();

	if (++m_Frame > p->m_Param.time)
	{

	}

	if (liveCount == 1)
	{

	}

	if (liveCount == 0)
	{

	}
}

void MatchState::MatchPlay::Exit(_Client_type_ptr p)
{

}
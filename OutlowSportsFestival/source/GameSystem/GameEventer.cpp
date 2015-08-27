#include "GameEventer.h"
#include "../Camera/CameraState.h"
#include "../character/CharacterManager.h"
#include "../character/CharacterBase.h"
#include "../character/CharacterFunction.h"
#include "../Effect/ParticleMoveObject.h"
#include "../Effect/ParticleRenderer.h"
#include "../GameSystem/ResourceManager.h"
#include "../Collision/Collision.h"
#include "../Effect/FadeGameObject.h"


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
//         ２ラウンド目以降の開始ステート
//--------------------------------------------------

MatchState::RoundResetCountdown::RoundResetCountdown()
{
    m_Frame = 0;
}

MatchState::RoundResetCountdown::~RoundResetCountdown()
{

}

void MatchState::RoundResetCountdown::Enter(_Client_type_ptr p)
{

}

void MatchState::RoundResetCountdown::Execute(_Client_type_ptr p)
{
    if (++m_Frame > 120)
    {
        DefCamera.SetNewState(new CameraStateGamePlay());
        p->SetState(new MatchPlay());
    }
}

void MatchState::RoundResetCountdown::Exit(_Client_type_ptr p)
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


void MatchState::MatchPlay::GetLiveCharacterMap(CharacterManager::CharacterMap& Out)
{
    Out = DefCharacterMgr.GetCharacterMap();
    CharacterManager::CharacterMap::iterator it = Out.begin();

    while (it != Out.end())
    {
        if (chr_func::isDie(it->first))
        {
            it = Out.erase(it);
            continue;
        }
        ++it;
    }
}

void MatchState::MatchPlay::Enter(_Client_type_ptr p)
{

}

void MatchState::MatchPlay::Execute(_Client_type_ptr p)
{
    const UINT liveCount = DefCharacterMgr.GetCharacterLiveCount();
    
    if (liveCount == 1)
    {
        //一人がちパターンの場合

        CharacterManager::CharacterMap NowLiveCharacterMap;
        GetLiveCharacterMap(NowLiveCharacterMap);

        //勝利したキャラクタを検索
        LpCharacterBase  WinCharacter = NowLiveCharacterMap.begin()->first;
        LpCharacterBase  LastDieCharacter = nullptr;
        
        //最後に倒されたキャラクタを検索
        for (auto& it : m_CharacterMap)
        {
            if (WinCharacter != it.first)
            {
                LastDieCharacter = it.first;
                break;
            }
        }

        MyAssert(LastDieCharacter != nullptr, "最後に倒されたキャラクタが見つかりませんでした　（プレイ人数が１の可能性あり）");

        //一人がちステートをセット
        p->SetState(new WinPose(LastDieCharacter, WinCharacter));

        return;
    }
    else if (liveCount == 0)
    {
        //引き分けの場合

        int a = 0;
    }
    else if (++m_Frame > p->m_Param.time)
	{
        //タイムアップの場合
        LpCharacterBase  WinCharacter = nullptr;
        RATIO            MostManyLife = 0.0f;
        RATIO            Temp;

        m_CharacterMap = DefCharacterMgr.GetCharacterMap();

        for (auto& it : m_CharacterMap)
        {
            Temp = chr_func::GetLifeRatio(it.first);

            if (Temp > MostManyLife)
            {
                MostManyLife = Temp;
                WinCharacter = it.first;
            }
        }

        MyAssert(WinCharacter != nullptr, "タイムアップ時にキャラクタがいませんでした(ありえない!)");

        //タイムアップ勝ちステートをセット
     //   p->SetState(new WinPose(LastDieCharacter, WinCharacter));

        return;
	}

    //生存しているキャラクタデータを更新する
    GetLiveCharacterMap(m_CharacterMap);
}

void MatchState::MatchPlay::Exit(_Client_type_ptr p)
{

}


//--------------------------------------------------
//              一人勝ちステート
//--------------------------------------------------

MatchState::WinPose::WinPose(
    LpCharacterBase pLastDieCharacter,
    LpCharacterBase pWinCharacter
    ) :
m_Frame(0),
m_pLastDieCharacter(pLastDieCharacter),
m_pWinCharacter(pWinCharacter)
{

}

MatchState::WinPose::~WinPose()
{

}

void MatchState::WinPose::Enter(_Client_type_ptr p)
{
    DefCamera.SetNewState(new CameraStateCharacterZoom(m_pLastDieCharacter, 0.05f));
}

void MatchState::WinPose::Execute(_Client_type_ptr p)
{
    ++m_Frame;

    if (m_Frame == 120)
    {
        DefCamera.SetNewState(new CameraStateCharacterZoom(m_pWinCharacter, 0.05f));
    }

    if (m_Frame == 300)
    {
        p->SetState(new ResetRound());
    }
}

void MatchState::WinPose::Exit(_Client_type_ptr p)
{

}


//--------------------------------------------------
//            ラウンドリセットステート
//--------------------------------------------------

void MatchState::ResetRound::Enter(_Client_type_ptr p)
{
    m_Frame = 0;

    //ブラックアウトさせる
    new FadeGameObject(
        COLORf(1, 0, 0, 0),
        40,
        0,
        30
        );
}

void MatchState::ResetRound::Execute(_Client_type_ptr p)
{
    if (++m_Frame == 40)
    {
        DefGameObjMgr.SendMsg(GameObjectBase::MsgType::_RoundReset);
        DefCamera.SetNewState(new CameraStateGamePlay(true));

        p->SetState(new RoundResetCountdown());
    }
}

void MatchState::ResetRound::Exit(_Client_type_ptr p)
{

}


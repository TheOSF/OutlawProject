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
#include "../Effect/BlurImpact.h"
#include "../Stage/PhysicsMeshObject.h"

#include "../UI/RoundUI.h"
#include "../UI/FightUI.h"
#include "../UI/GameSetUI.h"

#include "../Sound/Sound.h"
#include "MatchLightManager.h"
#include "MatchManager.h"
#include "../UI/PlayerCursorBillbord.h"


//----------------------------------------------------
//  試合遷移メッセージを送信するクラス
//----------------------------------------------------

GameEventer::GameEventer(const Param& p, State* pInitState) :
m_Param(p)
{
    m_Param.round_count = 1;

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

void GameEventer::SetLightChange()
{
    DefMatchLightManager.LightChangeAndBack(
        3,
        30,
        10,
        Vector3(0.6f, 0.6f, 0.6f)
        );
}

void GameEventer::ResultCheck(CharacterBase* pNowWinCharacter)
{
    for (auto& it : DefCharacterMgr.GetCharacterMap())
    {
        UINT Point = it.first->m_Params.win;

        if (it.first == pNowWinCharacter)
        {
            ++Point;
        }

        if (Point >= m_Param.round)
        {
            ++it.first->m_Params.win;
            DefMatchManager.SceneChange(MatchManager::NextSceneType::GoResult);
            break;
        }
    }
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
    CameraState* pSetState;

    struct CameraPoint
    {
        Vector3 pos, target;
        float speed;
    };

    std::list<CameraPoint> CposList;

    CposList.push_front({ Vector3(18.710548f, 13.073265f, -40.158432f), Vector3(18.517427f, 12.809369f, -39.213413f),0.2f, });
    CposList.push_front({ Vector3(-7.837331f, 13.073265f, -44.821030f), Vector3(-7.887062f, 12.809370f, -43.857761f),0.2f });

    pSetState = new CameraStateGamePlay(true);

    for (auto& it : CposList)
    {
        pSetState =
        new CameraStateMovetoPoint(
            it.pos,
            it.target,
            it.speed,
            300,
            CameraStateMovetoPoint::CalcType::_Linear,
            pSetState
            );
    }

    //CameraStateGamePlay
    //CameraStateMovetoPoint

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


//------------------------------------------------------------
//       プレイヤー操作のキャラクタに矢印を出すステート
//------------------------------------------------------------

bool MatchState::PlayerCharacterDrawCursor::GetEffectChr(CharacterBase** ppOut, UINT ChrNum)
{
    UINT CalcCount = 0;

    *ppOut = nullptr;

    //プレイヤ操作　＆　前回より上でもっとも近いキャラクタ
    for (auto &it : DefCharacterMgr.GetCharacterMap())
    {
        if (ChrNum == (UINT)it.first->m_PlayerInfo.number)
        {
            *ppOut = it.first;
            break;

            //if (it.first->m_PlayerInfo.player_type == PlayerType::_Player)
            //{
            //    *ppOut = it.first;
            //    break;
            //}
            //else
            //{
            //    break;
            //}
        }
        
    }
    
    return *ppOut != nullptr;
}

void MatchState::PlayerCharacterDrawCursor::Effect(CharacterBase* p)
{
    //カーソルＵＩ
    PlayerCursorBillbord* UI = new PlayerCursorBillbord(p);;
    UI->LightUp(40);

    //振動
    chr_func::SetControllerShock(p, 0.6f, 0.5f);

}

void MatchState::PlayerCharacterDrawCursor::Enter(_Client_type_ptr p)
{
    m_Frame = 60;
    m_ChrNum = 0;
    m_PrePlayerNum = PlayerNum::Value::_ErrorType;
}

void MatchState::PlayerCharacterDrawCursor::Execute(_Client_type_ptr p)
{
    CharacterBase* pEffectChr = nullptr;

    //カウント中なら処理しない
    if (m_Frame > 0)
    {
        --m_Frame;
        return;
    }

    //カウントセット
    m_Frame = 60;

    //エフェクトを出すキャラクタを得る
    if (GetEffectChr(&pEffectChr, m_ChrNum))
    {
        //キャラクタにセット
        Effect(pEffectChr);

        //キャラクタカウント加算
        ++m_ChrNum;
    }
    else
    {
        //エフェクトを出すキャラクタがいなかった！！

        //ステートを開始ステートに
        p->SetState(new RoundResetCountdown());
    }
}

void MatchState::PlayerCharacterDrawCursor::Exit(_Client_type_ptr p)
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
    new RoundUI(p->m_Param.round_count++, 0);
}

void MatchState::RoundResetCountdown::Execute(_Client_type_ptr p)
{

    //ＵＩ表示
    if (m_Frame == 120)
    {
        new FightUI(0);
    }

    if (++m_Frame > 140)
    {
        //ゴング音再生
        Sound::Play(Sound::Gong_Start);

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
m_Frame(0),
m_PreLiveCount(0)
{

}

MatchState::MatchPlay::~MatchPlay()
{

}

MatchState::MatchPlay::GameStopMgr::GameStopMgr()
{
    m_Ep_frame = 0;
    m_Stop_frame = 0;
    m_pUpdateChr = nullptr;
}

//毎フレームの更新
void MatchState::MatchPlay::GameStopMgr::Update()
{
    if (m_Ep_frame == 0)
    {
        std::list<LpGameObjectBase> UpdateList;

        if (m_pUpdateChr != nullptr)
        {
            UpdateList.push_back(m_pUpdateChr);
        }

        DefGameObjMgr.FreezeOtherObjectUpdate(UpdateList, (UINT)m_Stop_frame);
    }

    m_Ep_frame = max(m_Ep_frame - 1, -1);
}

//ストップさせる(引数：何フレーム後にストップするか、何フレームストップするか)
void MatchState::MatchPlay::GameStopMgr::SetStop(UINT ep_frame, UINT stop_frame, CharacterBase* pUpdateChr)
{
    m_Ep_frame   = (int)ep_frame;
    m_Stop_frame = (int)stop_frame;
    m_pUpdateChr = pUpdateChr;
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
    //はじめのラウンドなら再生
    if (p->m_Param.round_count == 2)
    {
        Sound::Play(Sound::BGM_Battle, 1, true);
    }

    //プレイスタートメッセージを送信
    DefGameObjMgr.SendMsg(GameObjectBase::MsgType::_PlayStart);
}

void MatchState::MatchPlay::Execute(_Client_type_ptr p)
{
    const UINT liveCount = DefCharacterMgr.GetCharacterLiveCount();

    //ゲームストップ管理クラスの更新
    m_GameStopMgr.Update();

    //初期化
    if (m_PreLiveCount == 0)
    {
        m_PreLiveCount = liveCount;
    }

    //ステート変更関数の呼び出し
    SwitchState(liveCount,p);

    //前回のフレームとキャラクタ数が違った場合は時間をとめる
    if (m_PreLiveCount != liveCount && liveCount > 1)
    {
       // Sound::Play(Sound::);

        {
            //死んだキャラクタ以外の更新をとめる
            m_GameStopMgr.SetStop(4, 30);
        }
        p->SetLightChange();
    }

    //生存キャラクタカウンタを更新
    m_PreLiveCount = liveCount;

    //生存しているキャラクタデータを更新する
    GetLiveCharacterMap(m_CharacterMap);
}

void MatchState::MatchPlay::Exit(_Client_type_ptr p)
{

}

CharacterBase* MatchState::MatchPlay::GetNowDieCharacter()
{
    //今死んだキャラクタを検索する
    CharacterBase* pRet = nullptr;

    CharacterManager::CharacterMap NowLiveMap;
    GetLiveCharacterMap(NowLiveMap);


    for (auto& it1 : m_CharacterMap)
    {
        //見つけたかどうか
        bool find_succes = true;

        for (auto& it2 : NowLiveMap)
        {
            //両方のデータに生存していたら生きている
            if (it1.first == it2.first)
            {
                find_succes = false;
                break;
            }
        }

        if (find_succes)
        {
            pRet = it1.first;
        }
    }

    return pRet;
}

void MatchState::MatchPlay::SwitchState(const UINT liveCount, _Client_type_ptr p)
{
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

        //プレイ終了メッセージを送信
        DefGameObjMgr.SendMsg(GameObjectBase::MsgType::_GameSet);

        return;
    }
    else if (liveCount == 0)
    {
        //引き分けの場合
        p->SetState(new Draw);

        //カメラを止める
        DefCamera.SetNewState(new CameraState);

        //プレイ終了メッセージを送信
        DefGameObjMgr.SendMsg(GameObjectBase::MsgType::_GameSet);

        return;
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
        p->SetState(new TimeUp(WinCharacter));

        //プレイ終了メッセージを送信
        DefGameObjMgr.SendMsg(GameObjectBase::MsgType::_GameSet);

        return;
    }

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

    if (m_Frame == 1)
    {
        //ゴング音再生
        Sound::Play(Sound::Gong_End);
    }

    if (m_Frame == 6)
    {
        std::list<LpGameObjectBase> UpdateList;

        UpdateList.push_back(new GameSetUI());
        UpdateList.push_back(p);
      
        DefGameObjMgr.FreezeOtherObjectUpdate(UpdateList, 120);

        DefMatchLightManager.LightChangeAndBack(
            3,
            100,
            10,
            Vector3(0.8f, 0.8f, 0.8f)
            );
    }

    
    if (m_Frame == 124)
    {
        //リザルト画面に移行可能な場合は移行
        p->ResultCheck(m_pWinCharacter);
    }

    if (m_Frame == 240)
    {
        //勝利ポーズメッセージを送信
        m_pWinCharacter->Msg(GameObjectBase::MsgType::_WinPose);

        Sound::Play(Sound::Kira_n);
        DefCamera.SetNewState(new CameraStateCharacterZoom(m_pWinCharacter, 0.05f));

        //ラウンド勝利数を加算
        chr_func::AddWinPoint(m_pWinCharacter);
    }

    if (m_Frame == 400)
    {
        p->SetState(new ResetRound());
    }
}

void MatchState::WinPose::Exit(_Client_type_ptr p)
{

}

//--------------------------------------------------
//              タイムアップステート
//--------------------------------------------------

MatchState::TimeUp::TimeUp(LpCharacterBase pWinCharacter) :
m_pWinCharacter(pWinCharacter),
m_Frame(0)
{

}

void MatchState::TimeUp::Enter(_Client_type_ptr p)
{

}

void MatchState::TimeUp::Execute(_Client_type_ptr p)
{
    ++m_Frame;

    if (m_Frame == 1)
    {
        //ゴング音再生
        Sound::Play(Sound::Gong_End);

        //ＴｉｍｅＵｐ　ＵＩを表示
        new GameSetUI();

        //リザルト画面に移行可能な場合は移行
        p->ResultCheck(m_pWinCharacter);
    }

    if (m_Frame == 120)
    {
        //勝利ポーズメッセージを送信
        m_pWinCharacter->Msg(GameObjectBase::MsgType::_WinPose);

        CharacterManager::CharacterMap ChrMap = DefCharacterMgr.GetCharacterMap();

        for (auto& it : ChrMap)
        {
            if (it.first != m_pWinCharacter)
            {
                it.first->Msg(GameObjectBase::MsgType::_LosePose);
            }
        }

        //ラウンド勝利数を加算
        chr_func::AddWinPoint(m_pWinCharacter);

        Sound::Play(Sound::Kira_n);
        DefCamera.SetNewState(new CameraStateCharacterZoom(m_pWinCharacter, 0.05f));
    }

    if (m_Frame == 280)
    {
        p->SetState(new ResetRound());
    }
}

void MatchState::TimeUp::Exit(_Client_type_ptr p)
{

}



//--------------------------------------------------
//              引き分けステート
//--------------------------------------------------

void MatchState::Draw::Enter(_Client_type_ptr p)
{
    m_Frame = 0;
}

void MatchState::Draw::Execute(_Client_type_ptr p)
{
    ++m_Frame;

    if (m_Frame == 1)
    {
        //ゴング音再生
        Sound::Play(Sound::Gong_End);

        //ＴｉｍｅＵｐ　ＵＩを表示
        new GameSetUI();
    }


    if (m_Frame == 120)
    {
        //ＤｒａｗＧａｍｅ　ＵＩを表示
        new GameSetUI();
    }

    if (m_Frame == 280)
    {
        p->SetState(new ResetRound());
    }
}

void MatchState::Draw::Exit(_Client_type_ptr p)
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
        20,
        5,
        30
        );
}

void MatchState::ResetRound::Execute(_Client_type_ptr p)
{
    if (++m_Frame == 21)
    {
        //リセットメッセージを送信
        DefGameObjMgr.SendMsg(GameObjectBase::MsgType::_RoundReset);
        
        //カメラを初期位置へ
        DefCamera.SetNewState(new CameraStateGamePlay(true));

        //ステート移行
        p->SetState(new RoundResetCountdown());

    }
}

void MatchState::ResetRound::Exit(_Client_type_ptr p)
{

}


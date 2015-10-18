#ifndef __GAME_EVENTER_H__
#define __GAME_EVENTER_H__

#include "GameObject.h"
#include "../utillity/StateTemplate.h"
#include "ForwardDecl.h"
#include "../character/CharacterManager.h"

//----------------------------------------------------
//  試合遷移メッセージを送信するクラス
//----------------------------------------------------

class GameEventer :public GameObjectBase
{
public:
	typedef State<GameEventer, void> State;
	typedef StateMachine<GameEventer, void> StateMachine;

	struct Param
	{
		UINT  round;
		UINT  time;

        UINT  round_count;
	};

	Param          m_Param;
    
    GameEventer(const Param& p, State* pInitState, LPVECTOR3 pControllDirColor);
	~GameEventer();
	
	void SetState(State* pState);
    void SetLightChange();

private:

	StateMachine*  m_pStateMachine;
    Vector3* const m_pControllDirColor;
    const Vector3  m_InitDirColor;


	bool Update();
	bool Msg(MsgType mt);

};


//----------------------------------------------------
//  ステートクラス
//----------------------------------------------------

namespace MatchState
{
	//試合開始のカウントダウン
	class StartCountdown :public GameEventer::State
	{
	public:
		StartCountdown();
		~StartCountdown();

	private:
		UINT m_Timer;
		UINT m_MaxTime;

		void SetCamera();

		void Enter(_Client_type_ptr);
		void Execute(_Client_type_ptr);
		void Exit(_Client_type_ptr);
	};

    //２ラウンド目以降の開始ステート
    class RoundResetCountdown :public GameEventer::State
    {
    public:
        RoundResetCountdown();
        ~RoundResetCountdown();

    private:
        UINT m_Frame;

        void Enter(_Client_type_ptr);
        void Execute(_Client_type_ptr);
        void Exit(_Client_type_ptr);
    };

	//試合中のステート
	class MatchPlay :public GameEventer::State
	{
	public:
		MatchPlay();
		~MatchPlay();

	private:

        //ゲームストップを管理するクラス
        class GameStopMgr
        {
        public:
            GameStopMgr();

            //毎フレームの更新
            void Update();

            //ストップさせる(引数：何フレーム後にストップするか、何フレームストップするか)
            void SetStop(UINT ep_frame, UINT stop_frame, CharacterBase* pUpdateChr=nullptr);
        private:
            int m_Ep_frame;
            int m_Stop_frame;
            CharacterBase* m_pUpdateChr;
        };

        CharacterManager::CharacterMap  m_CharacterMap;
		UINT                            m_Frame;
        UINT                            m_PreLiveCount;
        GameStopMgr                     m_GameStopMgr;

        void GetLiveCharacterMap(CharacterManager::CharacterMap& Out);

		void Enter(_Client_type_ptr);
		void Execute(_Client_type_ptr);
		void Exit(_Client_type_ptr);

        void SwitchState(const UINT now_LiveCount, _Client_type_ptr p);
        CharacterBase* GetNowDieCharacter();
	};

    //一人勝ちステート
    class WinPose :public GameEventer::State
    {
    public:
        WinPose(
            LpCharacterBase pLastDieCharacter,
            LpCharacterBase pWinCharacter
            );
        ~WinPose();

    private:
    
        LpCharacterBase  m_pLastDieCharacter;
        LpCharacterBase  m_pWinCharacter;
        UINT             m_Frame;

        void Enter(_Client_type_ptr);
        void Execute(_Client_type_ptr);
        void Exit(_Client_type_ptr);
    };

    //タイムアップステート
    class TimeUp :public GameEventer::State
    {
    public:
        TimeUp(LpCharacterBase pWinCharacter);

    private:
        UINT             m_Frame;
        LpCharacterBase  m_pWinCharacter;

        void Enter(_Client_type_ptr);
        void Execute(_Client_type_ptr);
        void Exit(_Client_type_ptr);
    };


    //引き分けステート
    class Draw :public GameEventer::State
    {
    private:
        UINT             m_Frame;
        LpCharacterBase  m_pWinCharacter;

        void Enter(_Client_type_ptr);
        void Execute(_Client_type_ptr);
        void Exit(_Client_type_ptr);
    };

    //ラウンドリセットステート
    class ResetRound :public GameEventer::State
    {
    private:
        UINT             m_Frame;

        void Enter(_Client_type_ptr);
        void Execute(_Client_type_ptr);
        void Exit(_Client_type_ptr);
    };
}

#endif
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
	};

	const Param m_Param;

	GameEventer(const Param& p, State* pInitState);
	~GameEventer();
	
	void SetState(State* pState);

private:

	StateMachine*  m_pStateMachine;

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
        CharacterManager::CharacterMap  m_CharacterMap;
		UINT                            m_Frame;

        void GetLiveCharacterMap(CharacterManager::CharacterMap& Out);

		void Enter(_Client_type_ptr);
		void Execute(_Client_type_ptr);
		void Exit(_Client_type_ptr);
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
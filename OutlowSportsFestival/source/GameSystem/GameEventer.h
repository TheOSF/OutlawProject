#ifndef __GAME_EVENTER_H__
#define __GAME_EVENTER_H__

#include "GameObject.h"
#include "../utillity/StateTemplate.h"

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

	//試合中のステート
	class MatchPlay :public GameEventer::State
	{
	public:
		MatchPlay();
		~MatchPlay();

	private:
		UINT   m_Frame;

		void Enter(_Client_type_ptr);
		void Execute(_Client_type_ptr);
		void Exit(_Client_type_ptr);
	};

}

#endif
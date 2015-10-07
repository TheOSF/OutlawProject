#pragma once

#include "SoccerPlayer.h"
#include "../CharacterEvasionClass.h"

//***************************************************
//		サッカー回避クラス
//***************************************************

class SoccerState_Rolling : public SoccerState
{
public:

	

	class CallBackClass
	{
	public:
		virtual ~CallBackClass(){}
		virtual Vector3 GetVec() = 0;
	};

	SoccerState_Rolling(
		CallBackClass* pCallBackClass,  //ローリング方向コントロールクラス(終了時にdeleteする)
	bool dash);
	~SoccerState_Rolling();

	// ステート開始
	void Enter(SoccerPlayer* t)override;

	// ステート実行
	void Execute(SoccerPlayer* t)override;

	// ステート終了
	void Exit(SoccerPlayer* t)override;

private:
	CallBackClass * const m_pCallBackClass;
	Vector3               m_Vec;
	int                   m_Timer;
	bool                  Dash;
};

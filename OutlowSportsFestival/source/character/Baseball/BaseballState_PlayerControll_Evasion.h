#pragma once

#include "BaseballPlayer.h"
#include "../CharacterEvasionClass.h"

//***************************************************
//		テニス回避クラス
//***************************************************

class BaseballState_Rolling : public BaseballState
{
public:

	// 回避用パラメータ
	struct EvasionParams
	{
		int AllFrame;                          // 全フレーム
		int NoDamageStartFrame;     // 無敵開始フレーム
		int NoDamageEndFrame;      // 無敵終了フレーム
		float MoveDownSpeed;          // 減速割合
		float MoveSpeed;                   // 移動スピード
		RADIAN MaxTurnRadian;             // 向き補正の角度制限
	};

	class CallBackClass
	{
	public:
		virtual ~CallBackClass(){}
		virtual Vector3 GetVec() = 0;
	};

	BaseballState_Rolling(
		CallBackClass* pCallBackClass  //ローリング方向コントロールクラス(終了時にdeleteする)
		);
	~BaseballState_Rolling();

	// ステート開始
	void Enter(BaseballPlayer* b)override;

	// ステート実行
	void Execute(BaseballPlayer* b)override;

	// ステート終了
	void Exit(BaseballPlayer* b)override;

private:
	CallBackClass * const m_pCallBackClass;
	Vector3               m_Vec;
	int                   m_Timer;
};


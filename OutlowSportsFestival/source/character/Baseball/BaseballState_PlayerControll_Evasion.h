#pragma once

#include "BaseballPlayer.h"
#include "../CharacterEvasionClass.h"

//***************************************************
//		テニス回避クラス
//***************************************************

class BaseballState_Rolling : public BaseballState
{
public:

    BaseballState_Rolling();
    BaseballState_Rolling(CrVector3 Vec);

	~BaseballState_Rolling();

	// ステート開始
	void Enter(BaseballPlayer* b)override;

	// ステート実行
	void Execute(BaseballPlayer* b)override;

	// ステート終了
	void Exit(BaseballPlayer* b)override;

private:

    // 回避用パラメータ
    struct EvasionParams
    {
        int         AllFrame;
        int         Motion;
        int         NoDamageFrame;
        int         StandStartFrame;

        float       MoveValue;
        float       MoveDownFrame;
    };

	int                   m_Timer;
    EvasionParams         m_Param;
    Vector3               m_Vec;

    Vector3 GetPlayerControllMoveVec(BaseballPlayer* b);

    void SetParam(BaseballPlayer* b);
};


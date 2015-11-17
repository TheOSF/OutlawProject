#pragma once

#include "TennisPlayer.h"
#include "../CharacterEvasionClass.h"

//***************************************************
//		テニス回避クラス
//***************************************************

class TennisState_Rolling : public TennisState
{
public:
    class CallBackClass
    {
    public:
        virtual ~CallBackClass(){}
        virtual Vector3 GetVec() = 0;
        virtual void CancelUpdate() = 0;
    };

    TennisState_Rolling(
        CallBackClass* pCallBackClass  //ローリング方向コントロールクラス(終了時にdeleteする)
        );
    ~TennisState_Rolling();

    // ステート開始
    void Enter(TennisPlayer* t)override;

    // ステート実行
    void Execute(TennisPlayer* t)override;

    // ステート終了
    void Exit(TennisPlayer* t)override;

private:
    CallBackClass * const m_pCallBackClass;
    Vector3               m_Vec;
    int                   m_Timer;
};


#pragma once

#include "SoccerPlayer.h"
#include "../../Damage/Damage.h"

//-----------------------------------------------//
//  サッカー攻撃ステート
//-----------------------------------------------//

class SoccerAttackState :public SoccerState
{
public:

    SoccerAttackState(SoccerPlayer* const pChr, bool FistSecond = false);


    // ステート開始
    void Enter(SoccerPlayer* p)override;

    // ステート実行
    void Execute(SoccerPlayer* p)override;

    // ステート終了
    void Exit(SoccerPlayer* p)override;

private:
    typedef void(SoccerAttackState::*StateFunc)();

    static const float        m_DamageValue_FirstKick;
    static const float        m_DamageValue_ChunliKick;
    static const float        m_DamageValue_SummerSault;

    SoccerPlayer* const       m_pChr;
    StateFunc                 m_pStateFunc;
    int                       m_Timer;
    int                       m_HitStopCount;
    bool                      m_HitStopUsed;
    DamageShpere              m_Damage;
    bool                      m_DoNextAtk;
    bool                      m_isCounterHit;

    Vector3                   m_LocalVec;
    Vector3                   m_LocalPosOffset;
    UINT                      m_DamageEnableCount;
    
    void AngleControll(bool StartFrame);

    void UpdateDoCombo();
    void UpdateDamage();

    void SetState(StateFunc next);
    void SetDamage(UINT Frame);

    void State_Atk1();
    void State_Atk2();
    void State_Atk3();

    void State_Finish();
};
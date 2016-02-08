
#pragma once

#include "SoccerPlayer.h"
#include "../../Damage/Damage.h"
#include "../../utillity/Locus.h"

#include "../../Effect/SpeedEffect.h"


//****************************************************************
//		サッカーのダッシュクラス
//****************************************************************

class SoccerDash
{
public:
	SoccerDash(
		SoccerPlayer* pParent	//操るキャラクタのポインタ
		);

	~SoccerDash();

    void SetDashVec(CrVector3 Vec);  //向きをセット
    bool SetEnd(); //走るのやめる
    bool SetEnd(CrVector3 vec); //走るのやめる

    bool isDash()const; //走っているかどうか

	void Update();	//更新

private:
    typedef void(SoccerDash::*StateFunc)();

    SoccerPlayer* const m_pChr;
    StateFunc           m_pStateFunc;
    int                 m_StateTimer;
    SpeedEffect         m_SpeedEffect;
    Vector3             m_TurnViewVec;

    bool isHitWall();
    void UpdateSpeed();
    void SetState(StateFunc pState);

    void State_Start();
    void State_Dash();

    void State_DashEnd();
    void State_HitWall();

    void State_Finish();
};

#pragma once

#include "TennisPlayer.h"
#include "../../GameSystem/ForwardDecl.h"
#include "../../Ball/PhysicallyMoveBall.h"

//****************************************************
//	テニス_バウンドボール攻撃クラス
//****************************************************
class TennisState_BoundShot :public TennisState
{
public:
    TennisState_BoundShot();

    ~TennisState_BoundShot();


	void Enter(TennisPlayer* t)override;
	void Execute(TennisPlayer* t)override;
	void Exit(TennisPlayer* t)override;

private:
    int                  m_Timer;

    const CharacterBase* GetFrontTarget(TennisPlayer* t)const;
    Vector3 GetVec();

};


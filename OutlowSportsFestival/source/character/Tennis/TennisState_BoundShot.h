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
    class ControllClass
    {
    public:
        virtual ~ControllClass(){}
        virtual Vector3 GetVec() = 0;
        virtual bool DoOtherAction() = 0;
        virtual bool DoShotAfterAction() = 0;
    };
    
    TennisState_BoundShot(
        ControllClass*       pControllClass //終了時にdeleteする
        );

    ~TennisState_BoundShot();


	void Enter(TennisPlayer* t)override;
	void Execute(TennisPlayer* t)override;
	void Exit(TennisPlayer* t)override;

private:
    int                  m_Timer;
    ControllClass*       m_pControllClass;

    const CharacterBase* GetFrontTarget(TennisPlayer* t)const;

};


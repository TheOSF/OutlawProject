#ifndef __TENNIS_STATE_H__
#define __TENNIS_STATE_H__

#include "TennisPlayer.h"
#include "../../GameSystem/ForwardDecl.h"
#include "../../Ball/PhysicallyMoveBall.h"

//****************************************************
//	ejX_£UNX
//****************************************************
class TennisState_Shot :public TennisState
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
    
    TennisState_Shot(
        ControllClass*       pControllClass //IΉΙdelete·ι
        );

    ~TennisState_Shot();


	void Enter(TennisPlayer* t)override;
	void Execute(TennisPlayer* t)override;
	void Exit(TennisPlayer* t)override;

private:
    int                  m_Timer;
    ControllClass*       m_pControllClass;

    const CharacterBase* GetFrontTarget(TennisPlayer* t)const;

};

#endif
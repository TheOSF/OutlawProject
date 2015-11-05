#pragma once

#include "BaseballPlayer.h"
#include "../../GameSystem/ForwardDecl.h"
#include "../../Ball/PhysicallyMoveBall.h"

//****************************************************
//	ñÏãÖâìãóó£çUåÇÉNÉâÉX
//****************************************************
class BaseballState_PlayerControll_ShotAttack_B :public BaseballState
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

	BaseballState_PlayerControll_ShotAttack_B(
		ControllClass*       pControllClass //èIóπéûÇ…deleteÇ∑ÇÈ
		);

	~BaseballState_PlayerControll_ShotAttack_B();


	void Enter(BaseballPlayer* b)override;
	void Execute(BaseballPlayer* b)override;
	void Exit(BaseballPlayer* b)override;

public:
	const CharacterBase* ComExcute(BaseballPlayer* b)const;
private:
	int                  m_Timer;
	PhysicallyMoveBall*  m_pUpBall;
	ControllClass*       m_pControllClass;

	const CharacterBase* GetFrontTarget(BaseballPlayer* b)const;

};

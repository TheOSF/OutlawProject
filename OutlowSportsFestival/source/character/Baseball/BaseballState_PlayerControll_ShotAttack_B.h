#pragma once

#include "BaseballPlayer.h"
#include "../../GameSystem/ForwardDecl.h"
#include "../../Ball/PhysicallyMoveBall.h"

//****************************************************
//	–ì‹…‰“‹——£UŒ‚ƒNƒ‰ƒX
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
		ControllClass*       pControllClass //I—¹‚Édelete‚·‚é
		);

	~BaseballState_PlayerControll_ShotAttack_B();


	void Enter(BaseballPlayer* b)override;
	void Execute(BaseballPlayer* b)override;
	void Exit(BaseballPlayer* b)override;

private:
	int                  m_Timer;
	PhysicallyMoveBall*  m_pUpBall;
	ControllClass*       m_pControllClass;
	const CharacterBase* pTargetEnemy;

};

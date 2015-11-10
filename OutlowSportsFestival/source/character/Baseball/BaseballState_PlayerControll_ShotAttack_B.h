#pragma once

#include "BaseballPlayer.h"
#include "../../GameSystem/ForwardDecl.h"
#include "../../Ball/PhysicallyMoveBall.h"

//****************************************************
//	野球遠距離攻撃クラス
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
		virtual bool DoOtherAction_Com() = 0;
		virtual bool DoShotAfterAction() = 0;
	};

	BaseballState_PlayerControll_ShotAttack_B(
		ControllClass*       pControllClass //終了時にdeleteする
		);

	~BaseballState_PlayerControll_ShotAttack_B();


	void Enter(BaseballPlayer* b)override;
	void Execute(BaseballPlayer* b)override;
	void Exit(BaseballPlayer* b)override;

//　コンピュータ関連
public:
    void ComExcute(BaseballPlayer* b);
	//float TargetLen(Vector3 v1, Vector3 v2);
private:
	int                  m_Timer;
	PhysicallyMoveBall*  m_pUpBall;
	ControllClass*       m_pControllClass;
	const CharacterBase* pTargetEnemy;
	const CharacterBase* GetFrontTarget(BaseballPlayer* b)const;

};

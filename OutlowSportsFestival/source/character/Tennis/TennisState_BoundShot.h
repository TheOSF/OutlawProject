#pragma once

#include "TennisPlayer.h"
#include "../../GameSystem/ForwardDecl.h"
#include "../../Ball/PhysicallyMoveBall.h"

//****************************************************
//	�e�j�X_�o�E���h�{�[���U���N���X
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
        ControllClass*       pControllClass //�I������delete����
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


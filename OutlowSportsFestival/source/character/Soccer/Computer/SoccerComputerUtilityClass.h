#pragma once

#include "../SoccerRolling.h"

#include "../SoccerPlayerState.h"
#include "../SoccerPlayer.h"

#include "../../Computer/CharacterComputerDoAction.h"
#include "../../Computer/CharacterComputerReaction.h"


class SocceComputerrUtillityClass
{
public:
	//���[�����O�̕�������N���X
	class ComputerRollingControll :public SoccerState_Rolling::CallBackClass
	{
	public:
		SoccerPlayer*const cs;
		ComputerRollingControll(SoccerPlayer* ps, Vector3 vec) :cs(cs), stick(vec) {}
		Vector3 stick;

		Vector3 GetVec()override;
	};

    //�U���C�x���g�Ăяo���N���X
    class AttackEvent :public CharacterComputerDoAction::ActionEvent
    {
    public:
        AttackEvent(SoccerPlayer* p);

        bool Action(CharacterBase* pTarget, float len);

    private:
        SoccerPlayer*const m_pChr;
    };


    //�����C�x���g�Ăяo���N���X
    class ReactionEvent :public CharacterComputerReaction::ActionEvent
    {
    public:
        ReactionEvent(SoccerPlayer* p);

        void Reaction(const TypeParam& param, CrVector3 vec);

        static CharacterComputerReaction::InParam GetInParam();

    private:
        SoccerPlayer*const m_pChr;
    };

};
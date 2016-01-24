#pragma once

#include "../SoccerRolling.h"

#include "../SoccerPlayerState.h"
#include "../SoccerPlayer.h"

#include "../../Computer/CharacterComputerDoAction.h"
#include "../../Computer/CharacterComputerReaction.h"


class SocceComputerrUtillityClass
{
public:
	//ローリングの方向制御クラス
	class ComputerRollingControll :public SoccerState_Rolling::CallBackClass
	{
	public:
		SoccerPlayer*const cs;
		ComputerRollingControll(SoccerPlayer* ps, Vector3 vec) :cs(cs), stick(vec) {}
		Vector3 stick;

		Vector3 GetVec()override;
	};

    //攻撃イベント呼び出しクラス
    class AttackEvent :public CharacterComputerDoAction::ActionEvent
    {
    public:
        AttackEvent(SoccerPlayer* p);

        bool Action(CharacterBase* pTarget, float len);

    private:
        SoccerPlayer*const m_pChr;
    };


    //反応イベント呼び出しクラス
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
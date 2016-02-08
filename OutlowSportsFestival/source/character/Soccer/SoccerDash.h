
#pragma once

#include "SoccerPlayer.h"
#include "../../Damage/Damage.h"
#include "../../utillity/Locus.h"

#include "../../Effect/SpeedEffect.h"


//****************************************************************
//		�T�b�J�[�̃_�b�V���N���X
//****************************************************************

class SoccerDash
{
public:
	SoccerDash(
		SoccerPlayer* pParent	//����L�����N�^�̃|�C���^
		);

	~SoccerDash();

    void SetDashVec(CrVector3 Vec);  //�������Z�b�g
    bool SetEnd(); //����̂�߂�
    bool SetEnd(CrVector3 vec); //����̂�߂�

    bool isDash()const; //�����Ă��邩�ǂ���

	void Update();	//�X�V

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

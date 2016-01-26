#ifndef __SOCCER_PLAYER_STATE_H__
#define __SOCCER_PLAYER_STATE_H__

#include "SoccerSliding.h"
#include "SoccerDash.h"
#include "SoccerPlayer.h"

#include "SoccerAttackInfo_UsualAttack.h"

#include "../Attack/CharacterAttack.h"
#include "../../Ball/UsualBall.h"
#include "../CharacterMoveClass.h"
#include "../CharacterSliding.h"
#include "../Attack/CharacterAttack.h"
#include "../CharacterMoveClass.h"

#include "../CharacterShotAttackClass.h"
#include "../CharacterDamageMotion.h"
#include "SoccerRolling.h"
#include "../../GameSystem/GameController.h"

//****************************************************
//	�T�b�J�[�v���C���[�̑���N���X�w�b�_�[
//****************************************************


class SoccerUtillityClass
{
public:
    //���[�����O�̕�������N���X
    class PlayerRollingControll :public SoccerState_Rolling::CallBackClass
    {
    public:
        SoccerPlayer*const ps;

        PlayerRollingControll(SoccerPlayer* ps) :ps(ps){}


        Vector3 GetVec()override
        {
            Vector2 stick = controller::GetStickValue(controller::stick::left, ps->m_PlayerInfo.number);
            Vector3 vec(stick.x, 0, stick.y);

            if (vec.Length() < 0.25f)
            {
                return Vector3Zero;
            }

            vec = DefCamera.GetRight()*vec.x + DefCamera.GetForward()*vec.z;
            vec.y = 0;
            vec.Normalize();

            return vec;
        }
    };

};

//�v���C���[����̈ړ��N���X
class SoccerState_PlayerControll_Move :public SoccerState
{
private:
	CharacterUsualMove*		m_pMoveClass;
	void ActionStateSwitch(SoccerPlayer* s);
public:

	static SoccerState* GetPlayerControllMove(SoccerPlayer* ps);
	static bool SwitchGameState(SoccerPlayer* ps);
	void Enter(SoccerPlayer* s);
	void Execute(SoccerPlayer* s);
	void Exit(SoccerPlayer* s);
};

class SoccerMoveEvent :public CharacterUsualMove::MoveEvent
{
    SoccerPlayer* m_pSoccer;
public:
    SoccerMoveEvent(SoccerPlayer* pSoccer) :
        m_pSoccer(pSoccer){}

    //�A�j���[�V�����̍X�V
    void Update(bool isRun, RATIO speed_ratio)
    {
        m_pSoccer->m_Renderer.Update(0.5f);
    }
    //����n�߂Ƀ��[�V�������Z�b�g
    void RunStart()
    {
        m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_Run);
    }
    //�����͂��߂Ƀ��[�V�������Z�b�g
    void StandStart()
    {
        m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_Stand);
    }

    //����I���
    void RunEnd()
    {
        m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_RunStop);
    }
};

//�v���C���[����̃X���C�f�B���O�N���X
class SoccerState_Sliding :public SoccerState
{
public:
    SoccerState_Sliding(SoccerPlayer* s);
	// �X�e�[�g�J�n
	void Enter(SoccerPlayer* t)override;

	// �X�e�[�g���s
	void Execute(SoccerPlayer* t)override;

	// �X�e�[�g�I��
	void Exit(SoccerPlayer* t)override;

private:
    typedef void(SoccerState_Sliding::*StateFunc)();

    StateFunc               m_pStateFunc;
    SoccerPlayer* const     m_pChr;
    DamageShpere            m_Damage;
	int                     m_Timer;
    
    void SetState(StateFunc State);
    void UpdateDamage();
    void NoBallDamageUpdate();
    void StartAngleControll();
    void AutoAngleControll();
    void SetMoveByAngle();
    void SetEffect(RATIO alpha);

    void State_Start();
    void State_Sliding();
    void State_End();

};


//�v���C���[����̍U���N���X
class SoccerState_PlayerControll_Attack :public SoccerState
{
	//�U������N���X
	class PlayerControllEvent :public SoccerAttackClass::ControllEvent
	{
	public:
		PlayerControllEvent(SoccerPlayer*const pSoccer);

		bool isDoCombo();
		void AngleControll(RADIAN angle);

	private:
		SoccerPlayer*const m_pSoccer;

		const CharacterBase* GetFrontTargetEnemy();
	};
public:
	SoccerState_PlayerControll_Attack(SoccerPlayer* s);
	~SoccerState_PlayerControll_Attack();

	// �X�e�[�g�J�n
	void Enter(SoccerPlayer* t)override;

	// �X�e�[�g���s
	void Execute(SoccerPlayer* t)override;

	// �X�e�[�g�I��
	void Exit(SoccerPlayer* t)override;

private:
	SoccerAttackClass   m_Attack;        //�U���N���X�ւ̃|�C���^
};
//�v���C���[����̎ˌ��N���X
class SoccerState_PlayerControll_Shot :public SoccerState
{
private:
	CharacterShotAttack*		m_pShotClass;
public:
	void Enter(SoccerPlayer* s);
	void Execute(SoccerPlayer* s);
	void Exit(SoccerPlayer* s);
};
//�v���C���[����̃J�E���^�[�N���X
class SoccerState_PlayerControll_Counter : public SoccerState
{
private:
	CharacterDefaultCounter* m_pCounter; // �J�E���^�[�N���X
public:
	void Enter(SoccerPlayer* s);
	void Execute(SoccerPlayer* s);
	void Exit(SoccerPlayer* s);

};
//�v���C���[����̌ŗL�Z(�_�b�V��)�N���X
class SoccerState_PlayerControll_Dash : public SoccerState
{
private:
	SoccerDash*		m_pMoveClass;
	int m_timer;
public:

	void Enter(SoccerPlayer* s);
	void Execute(SoccerPlayer* s);
	void Exit(SoccerPlayer* s);
};

class SoccerDashEvent
{
    SoccerPlayer* m_pSoccer;
public:
    SoccerDashEvent(SoccerPlayer* pSoccer) :
        m_pSoccer(pSoccer){}

    void Update(bool isRun, RATIO speed_ratio)
    {
        m_pSoccer->m_Renderer.Update(1.5f);
    }

    void RunStart()
    {
        m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_Run);
    }

    void StandStart()
    {
        m_pSoccer->m_Renderer.SetMotion(SoccerPlayer::_ms_Stand);
    }
};

//�v���C���[����̕K�E�Z�N���X
class SoccerState_PlayerControll_Finisher : public SoccerState
{
private:
	//�@�������N���X�쐬
	CharacterShotAttack* SnakeShotClass(SoccerPlayer* s);
private:
	//�@�������N���X
	CharacterShotAttack* m_pSnakeShotClass;
	
public:
	int m_Timer;
	bool timeflg;
	//�@�R���X�g���N�^
	SoccerState_PlayerControll_Finisher();
	void Enter(SoccerPlayer* s);
	void Execute(SoccerPlayer* s);
	void Exit(SoccerPlayer* s);

};



#endif
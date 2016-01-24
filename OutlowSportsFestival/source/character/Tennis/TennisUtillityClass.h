#pragma once

#include "TennisPlayer.h"
#include "TennisState_Rolling.h"
#include "TennisPlayerState_SlowUpBall.h"
#include "TennisPlayerState_Counter.h"
#include "TennisPlayerState_Attack.h"
#include "TennisState_BoundShot.h"
#include "TennisPlayerState_UsualMove.h"

#include "../CharacterMoveClass.h"
#include "../../GameSystem/GameController.h"
#include "../../Camera/Camera.h"



//----------------------------------------------------------------------//
// �e�j�X�@���܂��܂Ƃ����N���X����
//----------------------------------------------------------------------//

class TennisUtillityClass
{
public:

    //���[�����O�̍��Z����N���X
    class PlayerRollingControll :public TennisState_Rolling::CallBackClass
    {
    public:
        TennisPlayer* const pt;

        PlayerRollingControll(TennisPlayer* pt) :pt(pt){}


        Vector3 GetVec()override
        {
            Vector2 stick = controller::GetStickValue(controller::stick::left, pt->m_PlayerInfo.number);
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

        void CancelUpdate()override
        {
            if (controller::GetTRG(controller::button::sankaku, pt->m_PlayerInfo.number))
            {// [��] �Ń{�[������
                pt->SetState(new TennisPlayerState_SlowUpBall(new TennisUtillityClass::PlayerSlowBallControll(pt)));
                return;
            }

            if (controller::GetTRG(controller::button::shikaku, pt->m_PlayerInfo.number))
            {// [��] �� [�ߋ����U��]
                pt->SetState(new TennisState_PlayerControll_Attack(pt,true));
                return;
            }

            if (controller::GetTRG(controller::button::_R1, pt->m_PlayerInfo.number))
            {// [R1] �� [�J�E���^�[]
                pt->SetState(new TennisState_Counter());
                return;
            }
        }
    };


    //�����グ����V���b�g�̃R���g���[���N���X
    class PlayerSlowBallControll :public TennisPlayerState_SlowUpBall::ControllClass
    {
    public:
        TennisPlayer*const m_pTennis;

        PlayerSlowBallControll(TennisPlayer* pt) :m_pTennis(pt){}

        Vector3 GetVec()override
        {
            Vector2 stick = controller::GetStickValue(controller::stick::left, m_pTennis->m_PlayerInfo.number);
            Vector3 vec(stick.x, 0, stick.y);

            if (vec.Length() < 0.25f)
            {
                return Vector3Zero;
            }

            vec = DefCamera.GetRight()*vec.x + DefCamera.GetForward()*vec.z;
            vec.Normalize();

            return vec;
        }

        bool DoOtherAction()
        {
            if (controller::GetTRG(controller::button::shikaku, m_pTennis->m_PlayerInfo.number))
            {// [��] �� [�ߋ����U��]
                m_pTennis->SetState(new TennisState_PlayerControll_Attack(m_pTennis,true));
                return true;
            }

            if (controller::GetTRG(controller::button::batu, m_pTennis->m_PlayerInfo.number))
            {// [�~] �� [���[�����O]
                m_pTennis->SetState(new TennisState_Rolling(new PlayerRollingControll(m_pTennis)));
                return true;
            }

            if (controller::GetTRG(controller::button::_R1, m_pTennis->m_PlayerInfo.number))
            {// [R1] �� [�_�u���J�E���^�[�I�H
                m_pTennis->SetState(new TennisState_Counter());
                return true;
            }

            return false;
        }

        bool DoShotAfterAction()
        {

            if (controller::GetTRG(controller::button::shikaku, m_pTennis->m_PlayerInfo.number))
            {// [��] �� [�ߋ����U��]
                m_pTennis->SetState(new TennisState_PlayerControll_Attack(m_pTennis, true));
                return true;
            }

            return false;
        }

        bool isShot(bool isSmashFrame)
        {
            return !controller::GetPush(controller::button::sankaku, m_pTennis->m_PlayerInfo.number);
            //return controller::GetTRG(controller::button::sankaku, m_pTennis->m_PlayerInfo.number);
        }
    };


    //�ړ��C�x���g�N���X
    class TennisMoveEvent :public CharacterUsualMove::MoveEvent
    {
        TennisPlayer* m_pTennis;
    public:
        TennisMoveEvent(TennisPlayer* pTennis) :
            m_pTennis(pTennis){}

        //�A�j���[�V�����̍X�V
        void Update(bool isRun, RATIO speed_ratio)
        {
            m_pTennis->m_Renderer.Update(1);
        }
        //����n�߂Ƀ��[�V�������Z�b�g
        void RunStart()
        {
            m_pTennis->m_Renderer.SetMotion(TennisPlayer::_mt_Run);
        }
        //�����͂��߂Ƀ��[�V�������Z�b�g
        void StandStart()
        {
            m_pTennis->m_Renderer.SetMotion(TennisPlayer::_mt_Stand);
        }

        //����I��胂�[�V�������Z�b�g
        void RunEnd()
        {
            m_pTennis->m_Renderer.SetMotion(TennisPlayer::_mt_RunEnd);
        }
    };
};


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
// テニス　こまごまとしたクラスたち
//----------------------------------------------------------------------//

class TennisUtillityClass
{
public:

    //ローリングの高校制御クラス
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
            {// [△] でボール発射
                pt->SetState(new TennisPlayerState_SlowUpBall(new TennisUtillityClass::PlayerSlowBallControll(pt)));
                return;
            }

            if (controller::GetTRG(controller::button::shikaku, pt->m_PlayerInfo.number))
            {// [□] で [近距離攻撃]
                pt->SetState(new TennisState_PlayerControll_Attack(pt,true));
                return;
            }

            if (controller::GetTRG(controller::button::_R1, pt->m_PlayerInfo.number))
            {// [R1] で [カウンター]
                pt->SetState(new TennisState_Counter());
                return;
            }
        }
    };


    //投げ上げ特殊ショットのコントロールクラス
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
            {// [□] で [近距離攻撃]
                m_pTennis->SetState(new TennisState_PlayerControll_Attack(m_pTennis,true));
                return true;
            }

            if (controller::GetTRG(controller::button::batu, m_pTennis->m_PlayerInfo.number))
            {// [×] で [ローリング]
                m_pTennis->SetState(new TennisState_Rolling(new PlayerRollingControll(m_pTennis)));
                return true;
            }

            if (controller::GetTRG(controller::button::_R1, m_pTennis->m_PlayerInfo.number))
            {// [R1] で [ダブルカウンター！？
                m_pTennis->SetState(new TennisState_Counter());
                return true;
            }

            return false;
        }

        bool DoShotAfterAction()
        {

            if (controller::GetTRG(controller::button::shikaku, m_pTennis->m_PlayerInfo.number))
            {// [□] で [近距離攻撃]
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


    //移動イベントクラス
    class TennisMoveEvent :public CharacterUsualMove::MoveEvent
    {
        TennisPlayer* m_pTennis;
    public:
        TennisMoveEvent(TennisPlayer* pTennis) :
            m_pTennis(pTennis){}

        //アニメーションの更新
        void Update(bool isRun, RATIO speed_ratio)
        {
            m_pTennis->m_Renderer.Update(1);
        }
        //走り始めにモーションをセット
        void RunStart()
        {
            m_pTennis->m_Renderer.SetMotion(TennisPlayer::_mt_Run);
        }
        //立ちはじめにモーションをセット
        void StandStart()
        {
            m_pTennis->m_Renderer.SetMotion(TennisPlayer::_mt_Stand);
        }

        //走り終わりモーションをセット
        void RunEnd()
        {
            m_pTennis->m_Renderer.SetMotion(TennisPlayer::_mt_RunEnd);
        }
    };
};


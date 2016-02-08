#include "BaseballPlayerState_Attack_P.h"
#include "BaseballPlayerState.h"
#include "../../GameSystem/GameController.h"
#include "../CharacterFunction.h"
#include "../CharacterManager.h"
#include "BaseballAttackInfo_UsualAtk.h"
#include "Baseball_HitEvent.h"
#include "../../Effect/EffectFactory.h"
#include "BaseballState_PlayerControll_ShotAttack_P.h"

const int Baseball_PlayerControll_Attack_P::m_sMaxCombo = 3;


//-------------近距離攻撃ステートクラス-------------

Baseball_PlayerControll_Attack_P::Baseball_PlayerControll_Attack_P(BaseballPlayer* b) :
m_pChr(b),
m_pStateFunc(&Baseball_PlayerControll_Attack_P::State_Atk),
m_NextType(NextType::_PreSelect),
m_StateTimer(0),
m_ComboCount(0)
{

}


Baseball_PlayerControll_Attack_P::~Baseball_PlayerControll_Attack_P()
{

}

// ステート開始
void  Baseball_PlayerControll_Attack_P::Enter(BaseballPlayer* b)
{

}

// ステート実行
void Baseball_PlayerControll_Attack_P::Execute(BaseballPlayer* b)
{
    m_StateTimer++;

    (this->*m_pStateFunc)();

}

// ステート終了
void Baseball_PlayerControll_Attack_P::Exit(BaseballPlayer* b)
{

}


void Baseball_PlayerControll_Attack_P::SetState(StateFunc state)
{
    m_pStateFunc = state;
    m_StateTimer = 0;
}

void Baseball_PlayerControll_Attack_P::UpdateNextType()
{
    if (m_pChr->m_PlayerInfo.player_type == PlayerType::_Player)
    {
        //プレイヤーの場合
        if (controller::GetTRG(controller::button::shikaku, m_pChr->m_PlayerInfo.number))
        {
            m_NextType = NextType::DoCombo;
        }

        if (controller::GetTRG(controller::button::sankaku, m_pChr->m_PlayerInfo.number))
        {
            m_NextType = NextType::DoFarAtk;
        }
    }
    else
    {
        if (frand() < 0.25f)
        {
            m_NextType = NextType::None;
        }
        else if (frand() < 0.25f)
        {
            m_NextType = NextType::DoFarAtk;
        }
        else
        {
            m_NextType = NextType::DoCombo;
        }
    }
}

void Baseball_PlayerControll_Attack_P::AngleControll(RADIAN angle)
{
    CharacterBase* pTarget = nullptr;


    if (m_pChr->m_PlayerInfo.player_type == PlayerType::_Player)
    {
        //プレイヤーの場合

        //一定範囲にいた場合はそのキャラクタを狙う
        if (chr_func::CalcAtkTarget(m_pChr, D3DXToRadian(33), 50, &pTarget))
        {
            chr_func::AngleControll(m_pChr, pTarget->m_Params.pos, angle);
        }
        else
        {
            //スティックで方向コントロール
            Vector2 stick = controller::GetStickValue(controller::stick::left, m_pChr->m_PlayerInfo.number);
            Vector3 pos = m_pChr->m_Params.pos + Vector3(stick.x, 0, stick.y);

            chr_func::AngleControll(m_pChr, pos, angle);
        }
    }
    else
    {
        //自動追従
        if (chr_func::CalcAtkTarget(m_pChr, PI, 120, &pTarget))
        {
            chr_func::AngleControll(m_pChr, pTarget->m_Params.pos, angle);
        }
    }
}

void Baseball_PlayerControll_Attack_P::State_Atk()
{
    const int Motions[m_sMaxCombo] =
    {
        baseball_player::_mb_Atk1_P,
        baseball_player::_mb_Atk2_P,
        baseball_player::_mb_Atk3_P,
    };

    const int ShotFrames[m_sMaxCombo] =
    {
        15,
        15,
        15,
    };

    const int EndFrames[m_sMaxCombo] =
    {
        33,
        33,
        45,
    };

    const int SwitchFrame = 25;
    const int CanButtonFrame = 18;
    
    //初期化
    if (m_StateTimer == 1)
    {
        m_pChr->SetMotion(Motions[m_ComboCount]);
    }

    //移動値をダウン
    chr_func::XZMoveDown(m_pChr, 0.05f);
    

    //ショット前なら
    if (m_StateTimer < ShotFrames[m_ComboCount])
    {
        RADIAN angle_speed = D3DXToRadian(5);

        //ショットはじめなら角度調整が強め
        if (m_StateTimer < 5)
        {
            angle_speed = D3DXToRadian(30);
        }

        //アングルコントロール
        AngleControll(angle_speed);
    }

    //ショット！
    if (m_StateTimer == ShotFrames[m_ComboCount])
    {

        //ボール発射
        BallBase::Params param;

        //移動は前向き
        chr_func::GetFront(m_pChr, &param.move);
        //スピードは適当
        param.move *= 0.68f;

        //腕の位置に
        param.pos = m_pChr->getNowModeModel()->GetWorldBonePos(33);
        //高さをキャラ共通ボール発射のYに
        param.pos.y = BallBase::UsualBallShotY;

        //親を自分に
        param.pParent = m_pChr;
        //通常タイプ
        param.type = BallBase::Type::_Usual;

        if (m_ComboCount == 2)
        {
            param.move *= 1.2f;

            //生成
            new UsualBall(param, DamageBase::Type::_VanishDamage, 4, UsualBall::GetUsualMoveControll());
        }
        else
        {
            //生成
            new UsualBall(param, DamageBase::Type::_WeekDamage, 4, UsualBall::GetUsualMoveControll());
        }


        //コントローラを振動
        chr_func::SetControllerShock(
            m_pChr,
            0.5f,
            0.15f
            );

        //エフェクト
        EffectFactory::ShotEffect(
            m_pChr->m_PlayerInfo.number,
            param.pos,
            param.move
            );

        //スキルゲージ加算
        chr_func::AddSkillGauge(m_pChr, UsualBall::AddSkillValueRatio);

    }

    //コンボ可能なら
    if (m_ComboCount < m_sMaxCombo - 1)
    {
        //コンボ移行判定の更新
        if (m_NextType == NextType::_PreSelect)
        {
            UpdateNextType();
        }

        //コンボ移行
        if (m_StateTimer >= SwitchFrame)
        {
            switch (m_NextType)
            {
            case NextType::None:
            case NextType::_PreSelect:
                break;

            case NextType::DoCombo:
                SetState(&Baseball_PlayerControll_Attack_P::State_Atk);
                ++m_ComboCount;
                m_NextType = NextType::_PreSelect;
                break;

            case NextType::DoFarAtk:
                m_pChr->SetState(new BaseballState_PlayerControll_ShotAttack_P());
                break;
            }
        }
    }

    //時間経過で通常ステートに
    if (m_StateTimer >= EndFrames[m_ComboCount])
    {
        m_pChr->SetState(BaseballState_PlayerControll_Move::GetPlayerControllMove(m_pChr));
    }

    //更新
    {
        chr_func::UpdateAll(m_pChr, &BaseballHitEvent(m_pChr, m_StateTimer < ShotFrames[m_ComboCount]));
        chr_func::CreateTransMatrix(m_pChr, &m_pChr->getNowModeModel()->m_TransMatrix);

        m_pChr->ModelUpdate(1);
    }
}
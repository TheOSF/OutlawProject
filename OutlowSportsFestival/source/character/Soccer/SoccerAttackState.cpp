#include "SoccerAttackState.h"
#include "../CharacterFunction.h"

#include "../CharacterBase.h"
#include "../../utillity/DamageObject.h"

#include "SoccerHitEvent.h"

#include "SoccerPlayerState.h"
#include "../../GameSystem/GameController.h"

#include "../../Camera/Camera.h"
#include "../../Sound/Sound.h"


//-----------------------------------------------//
//          サッカー攻撃ステート
//-----------------------------------------------//

const float SoccerAttackState::m_DamageValue_FirstKick = 4.0f;
const float SoccerAttackState::m_DamageValue_ChunliKick = 2.0f;
const float SoccerAttackState::m_DamageValue_SummerSault = 6.0f;


SoccerAttackState::SoccerAttackState(SoccerPlayer* const pChr, bool FistSecond) :
m_pChr(pChr),
m_HitStopCount(0),
m_DamageEnableCount(0),
m_DoNextAtk(false),
m_HitStopUsed(false)
{
    m_Damage.m_Enable = false;

    //初期化
    if (FistSecond)
    {   
        SetState(&SoccerAttackState::State_Atk3);
    }
    else
    {
        SetState(&SoccerAttackState::State_Atk1);
    }
}

void SoccerAttackState::Enter(SoccerPlayer* p)
{
    

    //ダメージ初期化
    m_Damage.HitMotionFrame = 25;
    m_Damage.MaxChrHit = 1;
    m_Damage.m_Enable = false;
    m_Damage.m_VecPower = Vector2(0.01f, 0);
    m_Damage.m_Param.size = 2.0f;
    m_Damage.m_VecType = DamageShpere::DamageVecType::MemberParam;
    m_Damage.pParent = m_pChr;
    m_Damage.type = DamageBase::Type::_WeekDamage;
    m_Damage.Value = 1.0f;
    m_Damage.AddSkillGaugeValue = 4.0f;

    m_LocalPosOffset = Vector3(0, 2, 2);
    m_LocalVec = Vector3(0, 0, 1);

    UpdateDamage();
}

void SoccerAttackState::Execute(SoccerPlayer* p)
{
    if (m_HitStopCount <= 0)
    {
        //通常更新
        (this->*m_pStateFunc)();

        //カウント更新
        ++m_Timer;

        m_pChr->m_Renderer.Update(1);

        chr_func::XZMoveDown(m_pChr, 0.1f);
        chr_func::UpdateAll(m_pChr, &SoccerHitEvent(m_pChr, m_isCounterHit));


        if (m_Damage.m_Enable&&m_Damage.HitCount > 0 && m_HitStopUsed == false)
        {
            m_HitStopCount = 0;
            m_HitStopUsed = true;
        }
    }
    else
    {
        //ヒットストップ時の更新
        --m_HitStopCount;

        chr_func::UpdateAll(m_pChr, &SoccerHitEvent(m_pChr, m_isCounterHit), 0);
    }

    //デフォルトでカウンタヒットoffに
    m_isCounterHit = false;

    //ダメージ更新
    UpdateDamage();

    //更新
    chr_func::CreateTransMatrix(m_pChr, &m_pChr->m_Renderer.m_TransMatrix);

}

void SoccerAttackState::Exit(SoccerPlayer* p)
{

}

void SoccerAttackState::AngleControll(bool StartFrame)
{
    const RADIAN AngleSpeed = (StartFrame) ? (D3DXToRadian(180)) : (D3DXToRadian(10));

    if (m_pChr->m_PlayerInfo.player_type == PlayerType::_Player)
    {
        CharacterBase* pTarget = nullptr;

        if (chr_func::CalcAtkTarget(m_pChr, D3DXToRadian(90), 8.0f, &pTarget))
        {
            chr_func::AngleControll(m_pChr, pTarget->m_Params.pos, AngleSpeed);
        }
        else
        {
            Vector2 Stick = controller::GetStickValue(controller::stick::left, m_pChr->m_PlayerInfo.number);
            Vector3 OriginVec = DefCamera.GetForward()*Stick.y + DefCamera.GetRight()*Stick.x;

            chr_func::AngleControll(m_pChr, m_pChr->m_Params.pos + OriginVec, AngleSpeed);
        }


    }
    else
    {
        CharacterBase* pTarget = nullptr;

        if (chr_func::CalcAtkTarget(m_pChr, D3DXToRadian(180), 8.0f, &pTarget))
        {
            chr_func::AngleControll(m_pChr, pTarget->m_Params.pos, AngleSpeed);
        }
    }

    
}



void SoccerAttackState::UpdateDoCombo()
{
    if (m_DoNextAtk)
    {
        return;
    }

    if (m_pChr->m_PlayerInfo.player_type == PlayerType::_Player)
    {
        //ボタン入力で連続技に
        m_DoNextAtk = controller::GetTRG(controller::button::shikaku, m_pChr->m_PlayerInfo.number);

    }
    else
    {
        //コンピュータの場合
        RATIO RandRatio = 0.0f;

        switch (m_pChr->m_PlayerInfo.strong_type)
        {
        case StrongType::_Weak:
            RandRatio = 0.1f;
            break;

        case StrongType::_Usual:
            RandRatio = 0.05f;
            break;

        case StrongType::_Strong:
            RandRatio = 0.0025f;
            break;
        }
        
        //当たっていたら or 乱数で連続技に
        m_DoNextAtk =
            (m_Damage.HitCount > 0) ||
            (frand() <= RandRatio);
    }
}

//ダメージ更新
void SoccerAttackState::UpdateDamage()
{
    Matrix M;

    {
        //有効、無効を計算
        m_Damage.m_Enable = m_DamageEnableCount > 0;

        if (m_Damage.m_Enable)
        {
            --m_DamageEnableCount;
        }
    }

    {
        //ダメージ地点をセット
        M = m_pChr->m_Renderer.m_TransMatrix;

        D3DXMatrixWorldScaleNormalize(M);

        m_Damage.m_Param.pos = Vector3MulMatrix(m_LocalPosOffset, M);
    }

    {
        //方向をセット
        D3DXMatrixRotationY(&M, m_pChr->m_Params.angle);

        m_Damage.m_Vec = Vector3Normalize(Vector3MulMatrix3x3(m_LocalVec, M));
    }
}

void SoccerAttackState::SetState(StateFunc next)
{
    m_pStateFunc = next;
    m_Timer = 0;
    m_DoNextAtk = false;
}

void SoccerAttackState::SetDamage(UINT Frame)
{
    Sound::Play(Sound::Swing2);

    m_DamageEnableCount = max(Frame, m_DamageEnableCount);
    m_Damage.m_Enable = true;
    m_Damage.ResetCounts();
    m_HitStopUsed = false;
}

//-------------------------------------------------//

void SoccerAttackState::State_Atk1()
{
    const int DamageFrame = 15;
    const int EndFrame = 34;
    const int CanButtonFrame = 10;

    const int SwitchStartFrame = 22;
    const int SwitchEndFrame = 28;


    //初期化
    if (m_Timer == 1)
    {
        m_pChr->m_Renderer.SetMotion(SoccerPlayer::_ms_Atk1);

        //移動
        chr_func::AddMoveFront(m_pChr, 0.5f, 1.0f);
    }

    //角度調整
    if (m_Timer < DamageFrame)
    {
        AngleControll(m_Timer == 1);

        //カウンターヒットon
        m_isCounterHit = true;
    }
    

    //ダメージフレーム
    if (m_Timer == DamageFrame)
    {
        //ダメージパラメータセット        
        m_Damage.Value = m_DamageValue_FirstKick;
        
        //発生
        SetDamage(2);
    }

    //時間経過で終了
    if (m_Timer > EndFrame)
    {
        SetState(&SoccerAttackState::State_Finish);
    }

    //コンボ受付
    if (m_Timer > CanButtonFrame)
    {
        UpdateDoCombo();
    }

    //次の攻撃へ
    if (m_DoNextAtk && (m_Timer >= SwitchStartFrame) && (m_Timer <= SwitchEndFrame))
    {
        SetState(&SoccerAttackState::State_Atk2);
    }

}

void SoccerAttackState::State_Atk2()
{
    const int DamageFrames[] = 
    {
        5, 14, 20, 27,
    };

    const int EndFrame = 44;
    const int CanButtonFrame = 25;

    const int SwitchStartFrame = 32;
    const int SwitchEndFrame = 37;


    //初期化
    if (m_Timer == 1)
    {
        m_pChr->m_Renderer.SetMotion(SoccerPlayer::_ms_Atk2);

        //移動
        chr_func::AddMoveFront(m_pChr, 0.25f, 1.0f);
    }

    //角度調整
    if (m_Timer < DamageFrames[(int)ARRAYSIZE(DamageFrames) - 1])
    {
        AngleControll(m_Timer == 1);

        //カウンターヒットon
        m_isCounterHit = true;
    }

    //ダメージフレーム
    for (int i = 0; i < (int)ARRAYSIZE(DamageFrames); ++i)
    {
        if (m_Timer == DamageFrames[i])
        {
            //ダメージパラメータセット        
            m_Damage.Value = m_DamageValue_ChunliKick;

            //発生
            SetDamage(2);
        }
    }

    //時間経過で終了
    if (m_Timer > EndFrame)
    {
        SetState(&SoccerAttackState::State_Finish);
    }

    //コンボ受付
    if (m_Timer > CanButtonFrame)
    {
        UpdateDoCombo();
    }

    //次の攻撃へ
    if (m_DoNextAtk && (m_Timer >= SwitchStartFrame) && (m_Timer <= SwitchEndFrame))
    {
        SetState(&SoccerAttackState::State_Atk3);
    }
}

void SoccerAttackState::State_Atk3()
{
    const int DamageFrame = 15;
    const int EndFrame = 45;

    //初期化
    if (m_Timer == 1)
    {
        m_pChr->m_Renderer.SetMotion(SoccerPlayer::_ms_Atk3);

        //移動
        chr_func::AddMoveFront(m_pChr, 0.3f, 1.0f);
    }

    //角度調整
    if (m_Timer < DamageFrame)
    {
        AngleControll(m_Timer == 1);

        //カウンターヒットon
        m_isCounterHit = true;
    }

    //ダメージフレーム
    if (m_Timer == DamageFrame)
    {
        //ダメージパラメータセット        
        m_Damage.Value = m_DamageValue_SummerSault;
        m_LocalVec = Vector3Normalize(Vector3(0, 1, 1));
        m_Damage.m_VecPower = Vector2(0.35f, 0.7f);
        m_Damage.type = DamageBase::Type::_VanishDamage;

        //発生
        SetDamage(2);
    }

    //時間経過で終了
    if (m_Timer > EndFrame)
    {
        SetState(&SoccerAttackState::State_Finish);
    }
}

void SoccerAttackState::State_Finish()
{
    m_pChr->SetState(SoccerState_PlayerControll_Move::GetPlayerControllMove(m_pChr));
}

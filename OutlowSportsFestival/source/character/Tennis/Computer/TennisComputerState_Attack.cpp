#include "TennisComputerState_Attack.h"
#include "../TennisPlayerState_UsualMove.h"
#include "../../CharacterFunction.h"
#include "../../CharacterManager.h"
#include "../TennisAttackInfo_UsualAtk.h"

//------------プレイヤー操作の攻撃操作クラス--------------

TennisComputerState_Attack::ComputerControllEvent::ComputerControllEvent(TennisPlayer*const pTennis) :
m_pTennis(pTennis)
{

}

bool TennisComputerState_Attack::ComputerControllEvent::isDoCombo(bool isHit, int CheckFrame)
{
    RATIO val = 0.0f;

    switch (m_pTennis->m_PlayerInfo.strong_type)
    {
    case StrongType::_Weak:
        val = 0.5f;
        break;

    case StrongType::_Usual:
        val = 0.25f;
        break;

    case StrongType::_Strong:
        val = 0.1f;
        break;
    }

    MyAssert(CheckFrame > 0, "フレーム数がおかしい！");

    return isHit || frand() <= val / (float)CheckFrame;
}

void TennisComputerState_Attack::ComputerControllEvent::AngleControll(RADIAN angle)
{
    CharacterBase* pTargetCharacter = nullptr;

    if (chr_func::CalcAtkTarget(m_pTennis, PI, 15.0f, &pTargetCharacter))
    {
        //自動回転
        chr_func::AngleControll(m_pTennis, pTargetCharacter->m_Params.pos, angle);
    }
}


//-------------近距離攻撃ステートクラス-------------

TennisComputerState_Attack::TennisComputerState_Attack(TennisPlayer* t, bool StartSecond) :
m_Attack(t, new ComputerControllEvent(t)),
m_StartSecond(StartSecond)
{

}


TennisComputerState_Attack::~TennisComputerState_Attack()
{

}

// ステート開始
void TennisComputerState_Attack::Enter(TennisPlayer* t)
{
    //攻撃クラス作成
    TennisAttackInfo_UsualAtk* pAtk;

    TennisAttackInfo_UsualAtk::Param AtkParam[] =
    {
        { 6, 1.9f, 1.5f, DamageBase::Type::_WeekDamage, 18, 25, 0.15f, 8, 12, TennisPlayer::_mt_Attack1, 34, 10, 27, 34, 0, 15, D3DXToRadian(10), 42, 43, 3, 0.1f, 25 },
        { 2, 1.9f, 1.5f, DamageBase::Type::_WeekDamage, 4, 5, 0.075f, 1, 5, TennisPlayer::_mt_Attack2, 30, 0, 13, 14, 0, 2, D3DXToRadian(10), 42, 43, 3, 0.55f, 35 },
        { 8, 1.6f, 1.5f, DamageBase::Type::_VanishDamage, 18, 19, 1.0f, 15, 15, TennisPlayer::_mt_Attack3, 50, -1, -1, -1, 0, 4, D3DXToRadian(10), 55, 59, 3, 0.8f, 25 },
    };

    //二段目からの攻撃
    if (m_StartSecond)
    {
        for (int i = 0; i < (int)ARRAYSIZE(AtkParam) - 1; ++i)
        {
            AtkParam[i] = AtkParam[i + 1];
        }
    }

    for (int i = 0; i < (int)ARRAYSIZE(AtkParam); ++i)
    {
        pAtk = new TennisAttackInfo_UsualAtk(t);

        pAtk->m_Param = AtkParam[i];

        m_Attack.m_AttackInfoArray.push_back(pAtk);
    }
}


// ステート実行
void TennisComputerState_Attack::Execute(TennisPlayer* t)
{
    m_Attack.Update();

    if (m_Attack.isEnd())
    {
        t->SetState(TennisState_PlayerControll_Move::GetPlayerControllMove(t));
    }
}

// ステート終了
void TennisComputerState_Attack::Exit(TennisPlayer* t)
{

}
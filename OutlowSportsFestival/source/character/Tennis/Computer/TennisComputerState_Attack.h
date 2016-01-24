#pragma once

#include "../TennisPlayer.h"
#include "../TennisAttackClass.h"

//***************************************************
//		プレイヤー操作の 近距離攻撃クラス
//***************************************************

class TennisComputerState_Attack : public TennisState
{
public:

    //攻撃操作クラス
    class ComputerControllEvent :public TennisAttackClass::ControllEvent
    {
    public:
        ComputerControllEvent(TennisPlayer*const pTennis);

        bool isDoCombo(bool isHit, int CheckFrame);
        void AngleControll(RADIAN angle);

    private:
        TennisPlayer*const m_pTennis;

        const CharacterBase* GetFrontTargetEnemy();
    };

    TennisComputerState_Attack(TennisPlayer* t, bool StartSecond);
    ~TennisComputerState_Attack();

    // ステート開始
    void Enter(TennisPlayer* t)override;

    // ステート実行
    void Execute(TennisPlayer* t)override;

    // ステート終了
    void Exit(TennisPlayer* t)override;

private:
    TennisAttackClass   m_Attack;        //攻撃クラスへのポインタ
    bool                m_StartSecond;
};

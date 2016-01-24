#ifndef __TENNIS_PLAYER_STATE_ATTACK_H__
#define __TENNIS_PLAYER_STATE_ATTACK_H__

#include "TennisPlayer.h"
#include "TennisAttackClass.h"

//***************************************************
//		プレイヤー操作の 近距離攻撃クラス
//***************************************************

class TennisState_PlayerControll_Attack : public TennisState
{
public:

    //攻撃操作クラス
    class PlayerControllEvent :public TennisAttackClass::ControllEvent
    {
    public:
        PlayerControllEvent(TennisPlayer*const pTennis);

        bool isDoCombo(bool isHit, int CheckFrame);
        void AngleControll(RADIAN angle);

    private:
        TennisPlayer*const m_pTennis;

        const CharacterBase* GetFrontTargetEnemy();
    };

    TennisState_PlayerControll_Attack(TennisPlayer* t, bool StartSecond);
    ~TennisState_PlayerControll_Attack();

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

#endif
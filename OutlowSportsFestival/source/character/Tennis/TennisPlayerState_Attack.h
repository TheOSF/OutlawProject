#ifndef __TENNIS_PLAYER_STATE_ATTACK_H__
#define __TENNIS_PLAYER_STATE_ATTACK_H__

#include "TennisPlayer.h"
#include "../Attack/CharacterAttack.h"

//***************************************************
//		プレイヤー操作の 近距離攻撃クラス
//***************************************************

class TennisState_PlayerControll_Attack : public TennisState
{
public:

    TennisState_PlayerControll_Attack();
    ~TennisState_PlayerControll_Attack();

	// ステート開始
	void Enter(TennisPlayer* t)override;

	// ステート実行
	void Execute(TennisPlayer* t)override;

	// ステート終了
	void Exit(TennisPlayer* t)override;

private:
    const UINT                m_ComboNum;       //最大コンボ数
    UINT                      m_ComboCount;     //コンボカウント
    CharacterNearAttack*      m_pAttack;        //現在の攻撃クラスへのポインタ
    bool                      m_DoNextAttack;   //次の攻撃を行うかどうか

    //次の攻撃クラスをセット
    bool SetNextAttack(TennisPlayer* t);
};

#endif
#ifndef __TENNIS_PLAYER_STATE_VANISH_H__
#define __TENNIS_PLAYER_STATE_VANISH_H__

#include "TennisPlayer.h"
#include "../CharacterDamageVanish.h"

//***************************************************
//		テニスキャラクタの弱ひるみステート
//***************************************************
class TennisState_DamageVanish : public TennisState
{
public:
    TennisState_DamageVanish(
        TennisPlayer*  pTennis,
        const Vector3& Damage_vec  //ダメージを受けた方向
        );

    // ステート開始
    void Enter(TennisPlayer* t)override;

    // ステート実行
    void Execute(TennisPlayer* t)override;

    // ステート終了
    void Exit(TennisPlayer* t)override;

    void StateMachineExit(TennisPlayer* t)override;

private:
    CharacterDamageVanish*      m_pDamageVanishClass;	//ひるみ更新クラス
    TennisPlayer*               m_pTennis;
    Vector3                     m_Damage_vec;
};

#endif
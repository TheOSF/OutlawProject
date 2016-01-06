#pragma once

#include "../AmefootPlayer.h"

class CharacterDamageVanish;

class AmefootPlayerState_Die : public AmefootState
{
public:
    AmefootPlayerState_Die(
        AmefootPlayer* pAmefootPlayer,
        const Vector3& Damage_vec  //ダメージを受けた方向
        );

    void Enter(AmefootPlayer* pCharacter)override;

    void Execute(AmefootPlayer* pCharacter)override;

    void Exit(AmefootPlayer* pCharacter)override;

private:
    AmefootPlayer* m_pAmefootPlayer;
    CharacterDamageVanish*   m_pDamageVanishClass;	//ひるみ更新クラス
    Vector3                  m_Damage_vec;
};


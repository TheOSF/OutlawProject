#pragma once

#include "../AmefootPlayer.h"

class CharacterDamageVanish;

//-----------------------------------------------------------------------------------------
// AmefootPlayerState_ReceiveVanishDamage
// [ アメフト ] 吹き飛びダメージを受けるステートクラス
//-----------------------------------------------------------------------------------------
class AmefootPlayerState_ReceiveVanishDamage : public AmefootState
{
public:
     AmefootPlayerState_ReceiveVanishDamage(Vector3 const& damageVec);

     void Enter(AmefootPlayer* pCharacter)override;

     void Execute(AmefootPlayer* pCharacter)override;

     void Exit(AmefootPlayer* pCharacter)override;

     void StateMachineExit(AmefootPlayer* pCharacter)override;
private:
     class VanishEvent;

private:
     CharacterDamageVanish* m_pCharacterDamageVanish;
     Vector3 const m_damageVec;

};



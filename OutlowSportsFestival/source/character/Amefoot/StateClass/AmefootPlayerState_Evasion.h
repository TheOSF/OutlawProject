#pragma once


#include "../AmefootPlayer.h"

class AmefootEvasionStandUpClass;
class CharacterEvasion;

//-----------------------------------------------------------------------------------------
// AmefootPlayerState_Evasion
// [ アメフト ] 回避行動をするステートクラス
//-----------------------------------------------------------------------------------------
class AmefootPlayerState_Evasion : public AmefootState
{
public:
     AmefootPlayerState_Evasion();

     void Enter(AmefootPlayer* pCharacter)override;

     void Execute(AmefootPlayer* pCharacter)override;

     void Exit(AmefootPlayer* pCharacter)override;

private:
     class EvasionEvent;

private:
     AmefootEvasionStandUpClass* m_pAmefootEvasionStandUpClass;
     CharacterEvasion* m_pCharacterEvasion;
};


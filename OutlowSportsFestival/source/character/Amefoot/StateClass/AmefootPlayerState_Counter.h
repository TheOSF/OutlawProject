#pragma once


#include "../AmefootPlayer.h"

class CharacterDefaultCounter;

//-----------------------------------------------------------------------------------------
// AmefootPlayerState_Counter
// [ アメフト ] カウンターをするステートクラス
//-----------------------------------------------------------------------------------------
class AmefootPlayerState_Counter : public AmefootState
{
public:
     AmefootPlayerState_Counter();

     void Enter(AmefootPlayer* pCharacter)override;

     void Execute(AmefootPlayer* pCharacter)override;

     void Exit(AmefootPlayer* pCharacter)override;

private:
     class CounterEvent;

private:
     CharacterDefaultCounter* m_pCharacterDefaultCounter;
};



#pragma once


#include "../AmefootPlayer.h"

class CharacterDefaultCounter;

//-----------------------------------------------------------------------------------------
// AmefootPlayerState_Counter
// [ �A���t�g ] �J�E���^�[������X�e�[�g�N���X
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



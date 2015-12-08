#pragma once

#include "../AmefootPlayer.h"

//-----------------------------------------------------------------------------------------
// AmefootPlayerState_UsualMove
// [ �A���t�g ] �ʏ�ړ�����X�e�[�g�N���X
//-----------------------------------------------------------------------------------------
class AmefootPlayerState_UsualMove : public AmefootState
{
public:
     AmefootPlayerState_UsualMove();

     void Enter(AmefootPlayer* pCharacter)override;

     void Execute(AmefootPlayer* pCharacter)override;

     void Exit(AmefootPlayer* pCharacter)override;

private:
     class MoveEvent;

private:
     void ActionStateSwitch(AmefootPlayer* pCharacter);

private:
     CharacterUsualMove* m_pCharacterUsualMove;

};
//-----------------------------------------------------------------------------------------


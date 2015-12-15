#pragma once

#include "../AmefootPlayer.h"

class CharacterDamageMotion;

//-----------------------------------------------------------------------------------------
// AmefootPlayerState_ReceiveWeakDamage
// [ �A���t�g ] �ア�_���[�W���󂯂�X�e�[�g�N���X
//-----------------------------------------------------------------------------------------
class AmefootPlayerState_ReceiveWeakDamage : public AmefootState
{
public:
     AmefootPlayerState_ReceiveWeakDamage(Vector3 const& damageVec);

     void Enter(AmefootPlayer* pCharacter)override;

     void Execute(AmefootPlayer* pCharacter)override;

     void Exit(AmefootPlayer* pCharacter)override;

private:
     class MotionEvent;
     
private:
     CharacterDamageMotion* m_pCharacterDamageMotion;
     Vector3 const m_damageVec;
};
//-----------------------------------------------------------------------------------------


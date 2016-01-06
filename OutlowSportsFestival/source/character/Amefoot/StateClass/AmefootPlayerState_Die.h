#pragma once

#include "../AmefootPlayer.h"

class CharacterDamageVanish;

class AmefootPlayerState_Die : public AmefootState
{
public:
    AmefootPlayerState_Die(
        AmefootPlayer* pAmefootPlayer,
        const Vector3& Damage_vec  //�_���[�W���󂯂�����
        );

    void Enter(AmefootPlayer* pCharacter)override;

    void Execute(AmefootPlayer* pCharacter)override;

    void Exit(AmefootPlayer* pCharacter)override;

private:
    AmefootPlayer* m_pAmefootPlayer;
    CharacterDamageVanish*   m_pDamageVanishClass;	//�Ђ�ݍX�V�N���X
    Vector3                  m_Damage_vec;
};


#ifndef __TENNIS_PLAYER_STATE_VANISH_H__
#define __TENNIS_PLAYER_STATE_VANISH_H__

#include "TennisPlayer.h"
#include "../CharacterDamageVanish.h"

//***************************************************
//		�e�j�X�L�����N�^�̎�Ђ�݃X�e�[�g
//***************************************************
class TennisState_DamageVanish : public TennisState
{
public:
    TennisState_DamageVanish(
        TennisPlayer*  pTennis,
        const Vector3& Damage_vec  //�_���[�W���󂯂�����
        );

    // �X�e�[�g�J�n
    void Enter(TennisPlayer* t)override;

    // �X�e�[�g���s
    void Execute(TennisPlayer* t)override;

    // �X�e�[�g�I��
    void Exit(TennisPlayer* t)override;

    void StateMachineExit(TennisPlayer* t)override;

private:
    CharacterDamageVanish*      m_pDamageVanishClass;	//�Ђ�ݍX�V�N���X
    TennisPlayer*               m_pTennis;
    Vector3                     m_Damage_vec;
};

#endif
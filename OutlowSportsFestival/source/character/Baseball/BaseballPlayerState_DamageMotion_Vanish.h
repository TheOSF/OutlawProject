#ifndef __BASEBALLPLAYER_STATE_DAMAGEMOTION_VANIS_H__
#define __BASEBALLPLAYER_STATE_DAMAGEMOTION_VANIS_H__

#include "BaseballPlayer.h"
#include "../CharacterDamageVanish.h"

//***************************************************
//		�e�j�X�L�����N�^�̎�Ђ�݃X�e�[�g
//***************************************************
class BaseballState_DamageVanish : public BaseballState
{
public:
	BaseballState_DamageVanish(
		BaseballPlayer*  pBaseball,
		const Vector3& Damage_vec  //�_���[�W���󂯂�����
		);

	// �X�e�[�g�J�n
	void Enter(BaseballPlayer* b)override;

	// �X�e�[�g���s
	void Execute(BaseballPlayer* b)override;

	// �X�e�[�g�I��
	void Exit(BaseballPlayer* b)override;

private:
	CharacterDamageVanish*      m_pDamageVanishClass;	//�Ђ�ݍX�V�N���X
	BaseballPlayer*             m_pBaseball;
	Vector3                     m_Damage_vec;
};

#endif
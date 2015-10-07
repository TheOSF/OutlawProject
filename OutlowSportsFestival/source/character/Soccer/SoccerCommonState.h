#ifndef __SOCCER_COMMON_STATE_H__
#define __SOCCER_COMMON_STATE_H__

#include "SoccerPlayer.h"
#include "../CharacterDamageMotion.h"
#include "../CharacterDamageVanish.h"

class SoccerState_SmallDamage :public SoccerState
{
public:
	SoccerState_SmallDamage(
		SoccerPlayer*  pSoccer,
		const Vector3& Damage_vec  //�_���[�W���󂯂�����
		);
public:
	void Enter(SoccerPlayer* s);
	void Execute(SoccerPlayer* s);
	void Exit(SoccerPlayer* s);
private:
	CharacterDamageMotion*   m_pDamageMotionClass;	//�Ђ�ݍX�V�N���X
	SoccerPlayer*            m_pSoccer;
	Vector3                  m_Damage_vec;
};

class SoccerState_DamageVanish : public SoccerState
{
public:
	SoccerState_DamageVanish(
		SoccerPlayer*  m_pSoccer,
		const Vector3& Damage_vec  //�_���[�W���󂯂�����
		);

	// �X�e�[�g�J�n
	void Enter(SoccerPlayer* s)override;

	// �X�e�[�g���s
	void Execute(SoccerPlayer* s)override;

	// �X�e�[�g�I��
	void Exit(SoccerPlayer* s)override;

private:
	CharacterDamageVanish*      m_pDamageVanishClass;	//�Ђ�ݍX�V�N���X
	SoccerPlayer*               m_pSoccer;
	Vector3                     m_Damage_vec;
};

#endif
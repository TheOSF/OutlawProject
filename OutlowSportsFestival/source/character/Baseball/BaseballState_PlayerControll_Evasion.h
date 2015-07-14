#ifndef __BASEBALL_EVASION_H__
#define __BASEBALL_EVASION_H__

#include "../../Damage/Damage.h"
#include "BaseballPlayer.h"


//�v���C���[����̉���N���X
class CharacterEvasion;
class BaseballState_PlayerControll_Evasion :public BaseballState{
private:
	float roolspeed;//�@������̑���
private:
	// ����N���X�쐬
	CharacterEvasion* CreateEvasionClass(BaseballPlayer* b);
private:
	// ����N���X
	CharacterEvasion* m_pEvasionClass;
public:
	//�@�R���X�g���N�^
	BaseballState_PlayerControll_Evasion(float speed);
	//�@�X�e�[�g�J�n
	void Enter(BaseballPlayer* b)override;
	//�@�X�e�[�g���s
	void Execute(BaseballPlayer* b)override;
	//�@�X�e�[�g�I��
	void Exit(BaseballPlayer* b)override;

};

#endif
#ifndef __BASEBALL_SHOTATTACK_B_H__
#define __BASEBALL_SHOTATTACK_B_H__

#include "../../Damage/Damage.h"
#include "BaseballPlayer.h"




//�v���C���[����̉������N���X(�o�b�^�[)
class CharacterShotAttack;
class BaseballState_PlayerControll_ShotAttack_B :public BaseballState{
private:
	//�@�������N���X�쐬(�o�b�^�[)
	CharacterShotAttack* CreateShotAttackClass_B(BaseballPlayer* b);
private:
	//�@�������N���X
	CharacterShotAttack* m_pShotAttackClass_B;
public:
	//�@�R���X�g���N�^
	BaseballState_PlayerControll_ShotAttack_B();
	//�@�X�e�[�g�J�n
	void Enter(BaseballPlayer* b)override;
	//�@�X�e�[�g���s
	void Execute(BaseballPlayer* b)override;
	//�@�X�e�[�g�I��
	void Exit(BaseballPlayer* b)override;
};

#endif
#ifndef __BASEBALL_SHOTATTACK_P_H__
#define __BASEBALL_SHOTATTACK_P_H__

#include "../../Damage/Damage.h"
#include "BaseballPlayer.h"



//�v���C���[����̉������N���X(�s�b�`���[)
class CharacterShotAttack;
class BaseballState_PlayerControll_ShotAttack_P :public BaseballState{
private:
	//�@�������N���X�쐬(�s�b�`���[)
	CharacterShotAttack* CreateShotAttackClass_P(BaseballPlayer* b);
private:
	//�@�������N���X
	CharacterShotAttack* m_pShotAttackClass_P;
public:

	//�@�R���X�g���N�^
	BaseballState_PlayerControll_ShotAttack_P();
	//�@�X�e�[�g�J�n
	void Enter(BaseballPlayer* b)override;
	//�@�X�e�[�g���s
	void Execute(BaseballPlayer* b)override;
	//�@�X�e�[�g�I��
	void Exit(BaseballPlayer* b)override;

public:
	void ComExcute(BaseballPlayer* b);
};

#endif
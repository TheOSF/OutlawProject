#ifndef __BASEBALL_PLAYER_STATE_H__
#define __BASEBALL_PLAYER_STATE_H__

#include "BaseballPlayer.h"
#include "BaseballState_PlayerControll_ShotAttack_B.h"

#include "../CharacterMoveClass.h"
#include "../CharacterShotAttackClass.h"

//****************************************************
//	�싅�v���C���[�̑���N���X�w�b�_�[
//****************************************************

//�v���C���[����̈ړ��N���X
class BaseballState_PlayerControll_Move :public BaseballState
{
private:
	CharacterUsualMove* m_pMoveClass;

private:
	bool batterflg;//�@(true:�o�b�^�[,false:����)

public:
	//�@�R���X�g���N�^
	BaseballState_PlayerControll_Move() :batterflg(true){}
	//�@�X�e�[�g�J�n
	void Enter(BaseballPlayer* b)override;
	//�@�X�e�[�g���s
	void Execute(BaseballPlayer* b)override;
	//�@�X�e�[�g�I��
	void Exit(BaseballPlayer* b)override;
public:
	static bool SwitchGameState(BaseballPlayer* pb);

	static BaseballState* GetPlayerControllMove(BaseballPlayer* pt);

	//�@�o�b�^�[�����s
	void Batter(BaseballPlayer* b);
	//�@���莞���s
	void Pitcher(BaseballPlayer* b);
	//�@�Z�b�^�[
	void SetBatterFlg(BaseballPlayer* b){ batterflg = b->getBatterFlg(); }
};


class PlayerShotControllClass_B :public BaseballState_PlayerControll_ShotAttack_B::ControllClass
{
	BaseballPlayer* const   m_pBaseball;
public:
	PlayerShotControllClass_B(BaseballPlayer* pBaseball) :
		m_pBaseball(pBaseball){}

	Vector3 GetVec();

	bool DoOtherAction();
	bool DoOtherAction_Com();
	bool DoShotAfterAction();
};

#endif
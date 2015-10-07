#ifndef __TENNIS_PLAYER_STATE_COUNTER_H__
#define __TENNIS_PLAYER_STATE_COUNTER_H__

#include "BaseballPlayer.h"
#include "../../GameSystem/ForwardDecl.h"

//***************************************************
//		�v���C���[����� �J�E���^�[�N���X
//***************************************************


class BaseballState_PlayerControll_Counter : public BaseballState
{
private:
	int areasize;
public:
	BaseballState_PlayerControll_Counter(int size);
	// �X�e�[�g�J�n
	void Enter(BaseballPlayer* b)override;

	// �X�e�[�g���s
	void Execute(BaseballPlayer* b)override;

	// �X�e�[�g�I��
	void Exit(BaseballPlayer* b)override;

private:
	// �J�E���^�[�N���X�쐬
	CharacterDefaultCounter* m_pCounter; // �J�E���^�[�N���X
};



#endif
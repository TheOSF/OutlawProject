#ifndef __TENNIS_PLAYER_STATE_ATTACK_H__
#define __TENNIS_PLAYER_STATE_ATTACK_H__

#include "TennisPlayer.h"
#include "../Attack/CharacterAttack.h"

//***************************************************
//		�v���C���[����� �ߋ����U���N���X
//***************************************************

class TennisState_PlayerControll_Attack : public TennisState
{
public:

    TennisState_PlayerControll_Attack();
    ~TennisState_PlayerControll_Attack();

	// �X�e�[�g�J�n
	void Enter(TennisPlayer* t)override;

	// �X�e�[�g���s
	void Execute(TennisPlayer* t)override;

	// �X�e�[�g�I��
	void Exit(TennisPlayer* t)override;

private:
    const UINT                m_ComboNum;       //�ő�R���{��
    UINT                      m_ComboCount;     //�R���{�J�E���g
    CharacterNearAttack*      m_pAttack;        //���݂̍U���N���X�ւ̃|�C���^
    bool                      m_DoNextAttack;   //���̍U�����s�����ǂ���

    //���̍U���N���X���Z�b�g
    bool SetNextAttack(TennisPlayer* t);
};

#endif
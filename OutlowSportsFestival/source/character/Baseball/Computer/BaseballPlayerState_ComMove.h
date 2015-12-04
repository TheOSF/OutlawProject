#pragma once

#include "../BaseballPlayer.h"
#include "../../../GameSystem/ForwardDecl.h"
#include "../../../character/Computer/CharacterComputerMove.h"
#include "../../../character/Computer/CharacterComputerDoAction.h"
#include "../../../character/Computer/CharacterComputerReaction.h"

//****************************************************
//	�싅�v���C���[(Com)�̒ʏ�ړ��N���X�w�b�_�[
//****************************************************


class BaseballPlayer_Change;
//class BaseballEquip;

//�R���s���[�^�[�̈ړ��N���X
class BaseballPlayerState_ComMove :public BaseballState
{
private:
	bool batterflg;//�@(true:�o�b�^�[,false:����)
	Vector3 nearpos;//�@��ԋ߂��L������pos
	int changeTime;//�@�؂�ւ��^�C�~���O�p
public:

	class MoveControllClass
	{
	public:
		virtual ~MoveControllClass(){}
		virtual Vector2 Move(BaseballPlayer* b) = 0;

	};
public:
	//�@�R���X�g���N�^
	BaseballPlayerState_ComMove() :batterflg(true), nearpos(Vector3Zero), changeTime(0){

	}
	//�@�X�e�[�g�J�n
	void Enter(BaseballPlayer* b)override;
	//�@�X�e�[�g���s
	void Execute(BaseballPlayer* b)override;
	//�@�X�e�[�g�I��
	void Exit(BaseballPlayer* b)override;
	static bool SwitchGameState(BaseballPlayer* pb);
public:

	//�@�Z�b�^�[
	void SetBatterFlg(BaseballPlayer* b){ batterflg = b->getBatterFlg(); }
	Vector3 GetNearTargetPos(BaseballPlayer* b);

public:
	//�@�؂�ւ�
	void doChange(BaseballPlayer* b);
	//�@�ړ�
	void doMove(BaseballPlayer* b);
	//�@�U��
	void doAction(BaseballPlayer* b);
	//�@����(����or���)
	void doReaction(BaseballPlayer* b);
private:
	CharacterUsualMove*		m_pMoveClass;
	CharacterComputerMove*      m_pMoveControllClass;
	CharacterComputerDoAction*  m_pDoActionClass;
	CharacterComputerReaction*  m_pReactionClass;
};


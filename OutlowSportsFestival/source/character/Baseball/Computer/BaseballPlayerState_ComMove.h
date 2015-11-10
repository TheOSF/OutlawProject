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
//�R���s���[�^�[�̈ړ��N���X
class BaseballPlayerState_ComMove :public BaseballState
{
private:
	bool batterflg;//�@(true:�o�b�^�[,false:����)
	Vector3 nearpos;//�@��ԋ߂��L������pos
public:

	class MoveControllClass
	{
	public:
		virtual ~MoveControllClass(){}
		virtual Vector2 Move(BaseballPlayer* b) = 0;
		
	};
public:
	//�@�R���X�g���N�^
	BaseballPlayerState_ComMove() :batterflg(true), nearpos(Vector3Zero){}
	//�@�X�e�[�g�J�n
	void Enter(BaseballPlayer* b)override;
	//�@�X�e�[�g���s
	void Execute(BaseballPlayer* b)override;
	//�@�X�e�[�g�I��
	void Exit(BaseballPlayer* b)override;

public:

	//�@�o�b�^�[�����s
	void Batter(BaseballPlayer* b);
	//�@���莞���s
	void Pitcher(BaseballPlayer* b);
	//�@�Z�b�^�[
	void SetBatterFlg(BaseballPlayer* b){ batterflg = b->getBatterFlg(); }


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


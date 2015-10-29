#pragma once

#include "../BaseballPlayer.h"
#include "../../../GameSystem/ForwardDecl.h"

//****************************************************
//	�싅�v���C���[(Com)�̒ʏ�ړ��N���X�w�b�_�[
//****************************************************



//�R���s���[�^�[�̈ړ��N���X
class BaseballPlayerState_ComMove :public BaseballState
{
private:
	bool batterflg;//�@(true:�o�b�^�[,false:����)

public:

	class MoveControllClass
	{
	public:
		virtual ~MoveControllClass(){}
		virtual Vector2 Move(BaseballPlayer* b) = 0;
		
	};
public:
	//�@�R���X�g���N�^
	BaseballPlayerState_ComMove() :batterflg(true){}
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

private:
	CharacterUsualMove*		m_pMoveClass;
	MoveControllClass*      m_pMoveControllClass;
};



//�T���h�o�b�O�R���g���[���N���X
class SandBagMove_Com_ControllClass :public BaseballPlayerState_ComMove::MoveControllClass
{
public:
	Vector2 Move(BaseballPlayer* b)override;

};


class ComBaseballMoveControllClass :public BaseballPlayerState_ComMove::MoveControllClass
{
private:

	struct Param
	{
		RATIO   BallCounter;     //�{�[���Ώ���
		RATIO   BallCounterSpeed;//�{�[���������x
		RATIO   BallCounterTec;  //�Ώ��̓I�m��
		RATIO   RunStop;         //�����~�܂闦
		RATIO   RunPlaceTec;     //�ړ��ڕW�̐��m��
		RATIO   DangerEscape;    //�댯�@�m�̐��m��
		RATIO   ActionFrequence; //�s���̕p�x
		RATIO   NamePlay;        //�Ȃ߃v���C�x
	};

	Param m_Param;
	void(ComBaseballMoveControllClass::*m_pStateFunc)(Vector2&);

	int       m_Count;
	int       m_Type;//�@�s���^�C�v
	Vector3 nearpos;//�@��ԋ߂��L������pos
	Vector3   m_MoveTargetPos;
	Vector3   m_Vec;//�@�ړ��������
	BaseballPlayer* const m_pBaseball;
	

public:

	ComBaseballMoveControllClass(BaseballPlayer* b);
	~ComBaseballMoveControllClass();

	Vector2 Move(BaseballPlayer* b)override;

public:
	
	void StateChoise(Vector2& out);
	void StateMove_StartPos(Vector2& out);
	void StateMove_TargetPos(Vector2& out);
	void StateStop(Vector2& out);
	
public:
	void Change(BaseballPlayer* b);
public:
	void GetParams(Param& out, StrongType::Value st);
	Vector3 GetTargetPos();//�@�^�[�Q�b�g�ݒ�
	Vector3 GetMoveTargetPos();//�@�ʏ�ړ���
};

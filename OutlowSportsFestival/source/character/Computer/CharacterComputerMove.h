#pragma once
#include "../CharacterBase.h"
#include "../CharacterRenderer.h"
#include "../CharacterStateTemplate.h"	

class CharacterComputerMove
{
public:
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

	static void GetParams(Param& out, StrongType::Value st);

private:
	//����penum
	enum MoveMode
	{
		Stop, 
		Forward,
		Distance,
		Distance_B,
		Keep
	};

	MoveMode movemode;

	Param m_cParam;
	int       m_Count;
	Vector3   m_MoveTargetPos;

	CharacterBase* const m_cCharacter;
public:

	CharacterComputerMove(CharacterBase* cb);
	~CharacterComputerMove();

	Vector2 StateMoveFront(CharacterBase* cb);//�@�^�[�Q�b�g�ɑO�i
	Vector2 StateMoveDistance(CharacterBase* cb);//�@�^�[�Q�b�g�Ƃ̋������Ƃ�
	Vector2 StateStop(CharacterBase* cb);//�@�����~�܂�
	
	Vector2 SwitchAction(CharacterBase* cb);
	static Vector3 GetMoveTargetPos(CharacterBase* cb);

//�@�싅��p
public:

	Vector2 SwitcAction_Baseball(CharacterBase* cb, bool flg = false);
	
	Vector2 StateStop_Baseball(CharacterBase* cb, bool flg = false);//�@�����~�܂�
	Vector2 StateMoveDistance_Baseball(CharacterBase* cb);//�@�^�[�Q�b�g�Ƃ̋������Ƃ�
	Vector2 StateKeepLen(CharacterBase* cb);//�@�^�[�Q�b�g�Ƃ̋�����ۂ�
	
	
};
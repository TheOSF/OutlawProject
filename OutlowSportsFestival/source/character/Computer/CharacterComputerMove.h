#pragma once
#include "../CharacterBase.h"
#include "../CharacterRenderer.h"
#include "../CharacterStateTemplate.h"	

class CharacterComputerMove
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
	//����penum
	enum MoveMode
	{
		Stop, 
		Forward,
		Distance
	};
	MoveMode movemode;

	Param m_cParam;
	void(CharacterComputerMove::*m_pStateFunc)();

	int       m_Count;
	Vector3   m_MoveTargetPos;

	CharacterBase* const m_cCharacter;
public:

	CharacterComputerMove(CharacterBase* cb);
	~CharacterComputerMove();
	Vector2 Move(CharacterBase* cb);

	void GetParams(Param& out, StrongType::Value st);

	Vector2 StateMoveFront(CharacterBase* cb);
	Vector2 StateMoveDistance(CharacterBase* cb);
	Vector2 StateStop(CharacterBase* cb);

	Vector2 SwitchAction(CharacterBase* cb);

	Vector3 GetMoveTargetPos();
};
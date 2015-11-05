#pragma once
#include "../CharacterBase.h"
#include "../CharacterRenderer.h"
#include "../CharacterStateTemplate.h"	

class CharacterComputerMove
{
public:
	struct Param
	{
		RATIO   BallCounter;     //ボール対処率
		RATIO   BallCounterSpeed;//ボール反応速度
		RATIO   BallCounterTec;  //対処の的確さ
		RATIO   RunStop;         //立ち止まる率
		RATIO   RunPlaceTec;     //移動目標の正確さ
		RATIO   DangerEscape;    //危険察知の正確さ
		RATIO   ActionFrequence; //行動の頻度
		RATIO   NamePlay;        //なめプレイ度
	};

	static void GetParams(Param& out, StrongType::Value st);

private:
	//分岐用enum
	enum MoveMode
	{
		Stop, 
		Forward,
		Distance
	};

	MoveMode movemode;

	Param m_cParam;
	int       m_Count;
	Vector3   m_MoveTargetPos;

	CharacterBase* const m_cCharacter;
public:

	CharacterComputerMove(CharacterBase* cb);
	~CharacterComputerMove();



	Vector2 StateMoveFront(CharacterBase* cb);
	Vector2 StateMoveDistance(CharacterBase* cb);
	Vector2 StateStop(CharacterBase* cb);

	Vector2 SwitchAction(CharacterBase* cb);

	Vector3 GetMoveTargetPos();
	Vector3 NearCheak();//　一番近いやつを見つける
};
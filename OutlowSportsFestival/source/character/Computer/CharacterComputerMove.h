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

	Vector2 StateMoveFront(CharacterBase* cb);//　ターゲットに前進
	Vector2 StateMoveDistance(CharacterBase* cb);//　ターゲットとの距離をとる
	Vector2 StateStop(CharacterBase* cb);//　立ち止まる
	
	Vector2 SwitchAction(CharacterBase* cb);
	static Vector3 GetMoveTargetPos(CharacterBase* cb);

//　野球専用
public:

	Vector2 SwitcAction_Baseball(CharacterBase* cb, bool flg = false);
	
	Vector2 StateStop_Baseball(CharacterBase* cb, bool flg = false);//　立ち止まる
	Vector2 StateMoveDistance_Baseball(CharacterBase* cb);//　ターゲットとの距離をとる
	Vector2 StateKeepLen(CharacterBase* cb);//　ターゲットとの距離を保つ
	
	
};
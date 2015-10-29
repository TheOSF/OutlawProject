#pragma once

#include "../BaseballPlayer.h"
#include "../../../GameSystem/ForwardDecl.h"

//****************************************************
//	野球プレイヤー(Com)の通常移動クラスヘッダー
//****************************************************



//コンピューターの移動クラス
class BaseballPlayerState_ComMove :public BaseballState
{
private:
	bool batterflg;//　(true:バッター,false:投手)

public:

	class MoveControllClass
	{
	public:
		virtual ~MoveControllClass(){}
		virtual Vector2 Move(BaseballPlayer* b) = 0;
		
	};
public:
	//　コンストラクタ
	BaseballPlayerState_ComMove() :batterflg(true){}
	//　ステート開始
	void Enter(BaseballPlayer* b)override;
	//　ステート実行
	void Execute(BaseballPlayer* b)override;
	//　ステート終了
	void Exit(BaseballPlayer* b)override;

public:
	static bool SwitchGameState(BaseballPlayer* pb);

	static BaseballState* GetPlayerControllMove(BaseballPlayer* pt);

	//　バッター時実行
	void Batter(BaseballPlayer* b);
	//　投手時実行
	void Pitcher(BaseballPlayer* b);
	//　セッター
	void SetBatterFlg(BaseballPlayer* b){ batterflg = b->getBatterFlg(); }

private:
	CharacterUsualMove*		m_pMoveClass;
	MoveControllClass*      m_pMoveControllClass;
};



//サンドバッグコントロールクラス
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
		RATIO   BallCounter;     //ボール対処率
		RATIO   BallCounterSpeed;//ボール反応速度
		RATIO   BallCounterTec;  //対処の的確さ
		RATIO   RunStop;         //立ち止まる率
		RATIO   RunPlaceTec;     //移動目標の正確さ
		RATIO   DangerEscape;    //危険察知の正確さ
		RATIO   ActionFrequence; //行動の頻度
		RATIO   NamePlay;        //なめプレイ度
	};

	Param m_Param;
	void(ComBaseballMoveControllClass::*m_pStateFunc)(Vector2&);

	int       m_Count;
	int       m_Type;//　行動タイプ
	Vector3 nearpos;//　一番近いキャラのpos
	Vector3   m_MoveTargetPos;
	Vector3   m_Vec;//　移動する向き
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
	Vector3 GetTargetPos();//　ターゲット設定
	Vector3 GetMoveTargetPos();//　通常移動時
};

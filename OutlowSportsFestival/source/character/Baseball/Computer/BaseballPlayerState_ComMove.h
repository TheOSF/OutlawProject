#pragma once

#include "../BaseballPlayer.h"
#include "../../../GameSystem/ForwardDecl.h"
#include "../../../character/Computer/CharacterComputerMove.h"
#include "../../../character/Computer/CharacterComputerDoAction.h"
#include "../../../character/Computer/CharacterComputerReaction.h"

//****************************************************
//	野球プレイヤー(Com)の通常移動クラスヘッダー
//****************************************************


class BaseballPlayer_Change;
//class BaseballEquip;

//コンピューターの移動クラス
class BaseballPlayerState_ComMove :public BaseballState
{
private:
	bool batterflg;//　(true:バッター,false:投手)
	Vector3 nearpos;//　一番近いキャラのpos
	int changeTime;//　切り替えタイミング用
public:

	class MoveControllClass
	{
	public:
		virtual ~MoveControllClass(){}
		virtual Vector2 Move(BaseballPlayer* b) = 0;

	};
public:
	//　コンストラクタ
	BaseballPlayerState_ComMove() :batterflg(true), nearpos(Vector3Zero), changeTime(0){

	}
	//　ステート開始
	void Enter(BaseballPlayer* b)override;
	//　ステート実行
	void Execute(BaseballPlayer* b)override;
	//　ステート終了
	void Exit(BaseballPlayer* b)override;
	static bool SwitchGameState(BaseballPlayer* pb);
public:

	//　セッター
	void SetBatterFlg(BaseballPlayer* b){ batterflg = b->getBatterFlg(); }
	Vector3 GetNearTargetPos(BaseballPlayer* b);

public:
	//　切り替え
	void doChange(BaseballPlayer* b);
	//　移動
	void doMove(BaseballPlayer* b);
	//　攻撃
	void doAction(BaseballPlayer* b);
	//　反応(反射or回避)
	void doReaction(BaseballPlayer* b);
private:
	CharacterUsualMove*		m_pMoveClass;
	CharacterComputerMove*      m_pMoveControllClass;
	CharacterComputerDoAction*  m_pDoActionClass;
	CharacterComputerReaction*  m_pReactionClass;
};


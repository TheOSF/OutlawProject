#ifndef __BASEBALL_PLAYER_H__
#define __BASEBALL_PLAYER_H__

#include "../CharacterBase.h"
#include "../CharacterRenderer.h"
#include "../CharacterStateTemplate.h"	
#include "../../Ball/UsualBall.h"

class BaseballPlayer;

//ステートマシン型名付け
typedef ChrStateMachine< BaseballPlayer, CharacterBase::MsgType> BaseballStateMachine;
typedef ChrState< BaseballPlayer, CharacterBase::MsgType>BaseballState;

//　最大加速度
#define MaxAcceleration 0.8f

//*************************************************************
//		野球プレイヤークラス
//*************************************************************

namespace baseball_player{
	enum MotionType{
		_mt_Stand,
		_mt_Run,
		_mt_Shot,
		_mt_CounterPose,
		_mt_Counter,
		_mt_Evasion = 5,  // 回避
	};

	enum PositionMode
	{
		_pm_Butter,
		_pm_Pitcher,
	};
}

class BaseballPlayer :public CharacterBase
{
protected:
	bool batterflg;//　(true:バッター,false:投手)
	int characounter;//　出現キャラ数
	int outcounter;//　視野角外,死んでる数

private:
	BaseballStateMachine*		m_pStateMachine;
	float acc;//　加速度
public:


	BaseballPlayer(const CharacterBase::PlayerInfo& info);
	~BaseballPlayer();

	CharacterRenderer	m_Renderer;


	void SetState(BaseballState* state);
	bool Update();
	bool Msg(MsgType mt);
public:
	//　切り替え
	void Change();
	//　遠距離関数化
	BallBase::Params BaseballShot(BaseballPlayer* b, BallBase::Params p);
	//　遠距離ターゲット選定
	BallBase::Params TargetDecision( BallBase::Params p, Vector3 t);
public:
	//　ゲッター
	bool getBatterFlg(){ return batterflg; }

public:
	//　セッター
	void setAcc(float a){ acc = a; }//　加速度
public:

};
#endif
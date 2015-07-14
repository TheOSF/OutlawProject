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
		_mt_Damage_Weak,

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
	int num;       //　ターゲットナンバー
protected:
	//　視野角用
	Vector3 vf;
	Vector3 vt;
	float cross ;
	float dot;
	//　距離関係
	Vector3 v;
	float len;
	float templen;
	//　pos一時保存用
	Vector3 temppos;
	//　targetのpos
	Vector3 target;
	//　ホーミング関係
	D3DXQUATERNION q, invq, qpos;
	Vector3 v1, v2, axis;
	BallBase::Params tempparam;

private:
	BaseballStateMachine*		m_pStateMachine;
	float acc;//　加速度
public:


	BaseballPlayer(const CharacterBase::PlayerInfo& info);
	~BaseballPlayer();

	CharacterRenderer	m_Renderer;
	const float m_ModelSize;


	void SetState(BaseballState* state);
	bool Update();
	bool Msg(MsgType mt);
public:
	//　切り替え
	void Change();
	//　遠距離関数化
	BallBase::Params BaseballShot(BaseballPlayer* b, BallBase::Params p, float speed);
	//　視野角内計算
	Vector3 AngleField(Vector3 t, float range);
	//　遠距離ターゲット選定
	BallBase::Params TargetDecision( BallBase::Params p, Vector3 t);
	//　ホーミング計算
	BallBase::Params Homing(BallBase::Params p, Vector3 t);
	

public:
	//　ゲッター
	bool getBatterFlg(){ return batterflg; }
	int getCharacounter(){ return characounter; }
	int getOutcounter(){ return outcounter; }
	int getNum(){ return num; }
public:	   
	//　セッター
	void setAcc(float a){ acc = a; }//　加速度
	void setCharacounter(int num){ characounter = num; }
	void setOutcounter(int num){ outcounter = num; }
public:

};
#endif
#ifndef __BASEBALL_PLAYER_H__
#define __BASEBALL_PLAYER_H__

#include "../CharacterBase.h"
#include "../CharacterRenderer.h"
#include "../CharacterStateTemplate.h"	
#include "../../Ball/UsualBall.h"
#include "BaseballEquip.h"

class BaseballPlayer;
class BaseballEquip;
//ステートマシン型名付け
typedef ChrStateMachine< BaseballPlayer, CharacterBase::MsgType> BaseballStateMachine;
typedef ChrState< BaseballPlayer, CharacterBase::MsgType>BaseballState;



//*************************************************************
//		野球プレイヤークラス
//*************************************************************

namespace baseball_player{
	enum MotionType{
		_mb_Stand,
		_mb_Run,
		_mb_Shot,
		_mb_CounterPose = 17,
		_mb_CounterRight = 3,
		_mb_CounterLeft = 3,
		_mb_LosePose = 12,
		_mb_WinPose = 11,
		_mb_Evasion = 5,  // 回避
		_mb_Damage_Weak,
		_mb_Damage_Vanish_Fly,
		_mb_Damage_Vanish_Down,
		_mb_Damage_Vanish_StandUp,
		_mb_Atk1 = 15,//　近距離攻撃
		_mb_Atk2 = 2,
		_mb_Atk3 = 16,
		_mb_SpAtk1 = 13,
		_mb_SpAtk2 = 14

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
	bool temp_batterflg;//　一時保存
	int changetime;
private:
	BaseballStateMachine*		m_pStateMachine;
	BaseballEquip* helmetEquip;
	BaseballEquip* capEquip;
	BaseballEquip* batEquip;
	BaseballEquip* groveEquip;
public:

	//　攻撃によるスキルゲージ増加量
	struct SkillParam
	{
		float atk1Value;//　近初撃
		float atk2Value;//　2撃
		float atk3Value;//　3撃

	};
	static const SkillParam skillparams;


	BaseballPlayer(const CharacterBase::PlayerInfo& info);
	~BaseballPlayer();

	const float m_ModelSize;

	void SetState(BaseballState* state, bool Important = false);
	bool Update();
	bool CharacterMsg(MsgType mt);
public:
	//　装備切り替え
	void CheangeEquip();
	//　リセット
	void Riset();

public:
	//　ゲッター
	bool getBatterFlg(){ return batterflg; }
	int getChangeTime(){ return changetime; }
	void setChangeTime(int t){ changetime = t; }
	void setBatterFlg(bool flg){ batterflg = flg; }

public:

};
#endif
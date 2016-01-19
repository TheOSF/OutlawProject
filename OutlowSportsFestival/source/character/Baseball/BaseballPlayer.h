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
		//　バッター
        _mb_Stand_B = 1,				//　待機
        _mb_Run_B,				        //　走り
        _mb_Stop_B,				        //　立ち止まり
        _mb_Shot_B,                     //　遠距離
        _mb_Evasion_B,	                //　回避
        _mb_Atk1_B,						//　近距離攻撃1
        _mb_Atk2_B = 8,						//　近距離攻撃2
        _mb_Atk3_B = 11 ,						//　近距離攻撃3
		_mb_CounterPose_B,			    //　カウンター構え
		_mb_CounterRight_B,			    //　カウンター（右）
		_mb_CounterLeft_B,			    //　カウンター（左）
		_mb_Change_B,			        //　切替(バッター→ピッチャー)
		_mb_Change_P,			        //　切替(ピッチャー→バッター)
		_mb_SpAtk_B,			        //　必殺技

		_mb_Damage_Weak = 19,			    //　ダメージ
		_mb_Damage_Vanish_Fly,          //　吹き飛び中
		_mb_Damage_Vanish_Down,         //　吹き飛び着地
		_mb_Damage_Vanish_StandUp,      //　起き上がり
		_mb_Damage_Vanish_Wall,         //　吹き飛び(壁)
		_mb_Damage_Vanish_WallDown,     //　ずり落ち
		_mb_Damage_Vanish_Bound,        //　地面でバウンド
		//　ピッチャー
		_mb_Stand_P = 27,               //　待機
		_mb_Run_P,					    //　走り
		_mb_Stop_P,						//　立ち止まり
		_mb_Shot_P,						//　遠距離
		_mb_Evasion_P,                  //　回避
		_mb_Atk1_P,                     //　近距離攻撃1ー)
		_mb_Atk2_P,						//　近距離攻撃2
		_mb_CounterPose_P,				//　カウンター構え
		_mb_CounterRight_P,				//　カウンター（右）
		_mb_CounterLeft_P,				//　カウンター（左）
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
	bool changeflg;//　Com用
private:
	BaseballStateMachine*		m_pStateMachine;
    BaseballEquip* HeadEquip;
    BaseballEquip* WeaponEquip;
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

	void SetState(BaseballState* state, int Important = 0);
	bool Update();
	bool CharacterMsg(MsgType mt);
public:
	//　装備切り替え
	void CheangeEquip();
	//　リセット
	void Riset();

public:
	//　ゲッター・セッター
	bool getBatterFlg(){ return batterflg; }
	int getChangeTime(){ return changetime; }
	void setChangeTime(int t){ changetime = t; }
	void setBatterFlg(bool flg){ batterflg = flg; }
	void setChangeFlg(bool flg){ changeflg = flg; }
	bool getChangeFlg(){ return changeflg; }

public:

};
#endif
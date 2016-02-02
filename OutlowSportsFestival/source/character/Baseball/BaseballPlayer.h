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

namespace baseball_player
{
	enum MotionType
    {
		//　バッター
        _mb_Stand_B = 1,				//　待機
        _mb_Run_B,				        //　走り
        _mb_Stop_B,				        //　立ち止まり
        _mb_Shot_B,                     //　遠距離
        _mb_Evasion_B,	                //　回避
        _mb_Atk1_B,						//　近距離攻撃1
        _mb_Atk2_B = 8,						//　近距離攻撃2
        _mb_Atk3_B = 11,						//　近距離攻撃3
        _mb_CounterPose_B,			    //　カウンター構え
        _mb_CounterRight_B,			    //　カウンター（右）
        _mb_CounterLeft_B,			    //　カウンター（左）
        _mb_Change_B,			        //　切替(バッター→ピッチャー)
        _mb_Change_P,			        //　切替(ピッチャー→バッター)
        _mb_SpAtk_B,			        //　必殺技
        _mb_SpAtk_P = 30 ,			        //　必殺技

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
}

class BaseballPlayer :public CharacterBase
{
private:
	BaseballStateMachine*		m_pStateMachine;
    CharacterRenderer*          m_pBatterModel;
    CharacterRenderer*          m_pPitcherModel;
    bool                        m_BatterFlg; //　(true:バッター,false:投手)
public:

	BaseballPlayer(const CharacterBase::PlayerInfo& info);
	~BaseballPlayer();

    //現在フォームのモデルにモーションをセット
    void SetMotion(int motion);

    //現在フォームのモデルを更新
    void ModelUpdate(RATIO Speed = 1.0f);

	bool SetState(BaseballState* state, int Important = 0);
	bool Update();
	bool CharacterMsg(MsgType mt);

	//　装備切り替え
	void ChangeMode();

    //フォームを指定してセットする
    bool SetMode(bool isBatter);

	//　ゲッター・セッター
    inline bool getBatterFlg()const{ return m_BatterFlg; }
    inline bool getPitcherFlg()const{ return !m_BatterFlg; }

    inline bool isBatter()const{ return m_BatterFlg; }
    inline bool isPitcher()const{ return !m_BatterFlg; }

    //現在フォームのモデルをゲット
    CharacterRenderer* getNowModeModel();

private:
    //　リセット
    void Reset();
};

#endif
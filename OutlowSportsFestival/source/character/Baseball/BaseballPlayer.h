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
        _mb_Evasion_B = 6,	                //　回避
        _mb_Atk1_B,						//　近距離攻撃1
        _mb_Atk2_B = 8,						//　近距離攻撃2
        _mb_Atk3_B = 9,						//　近距離攻撃3
        _mb_CounterPose_B,			    //　カウンター構え
        _mb_CounterRight_B,			    //　カウンター（右）
        _mb_CounterLeft_B,			    //　カウンター（左）
        //    _mb_Change_B,			        //　切替(バッター→ピッチャー)
        //  _mb_Change_P,			        //　切替(ピッチャー→バッター)
        _mb_SpAtk_B = 14,			        //　必殺技

        _mb_Damage_Weak_B = 15,			    //　ダメージ
        _mb_Damage_Vanish_Fly_B,          //　吹き飛び中
        _mb_Damage_Vanish_Down_B,         //　吹き飛び着地
        _mb_Damage_Vanish_StandUp_B,      //　起き上がり
        _mb_Damage_Vanish_WallHitAndDown_B = 20,     //　ずり落ち
        _mb_Damage_Vanish_Bound_B,        //　地面でバウンド

        _mb_Change_B = 10,
        _mb_VanishHold_B = 22,       //つかまれている

        _mb_WinPose,
        _mb_LosePose,

        //　ピッチャー
        _mb_Stand_P = 2,               //　待機
        _mb_Run_P = 4,					    //　走り
        _mb_Stop_P,						//　立ち止まり
        _mb_Evasion_P = 7,                  //　回避

        _mb_Atk1_P = 6,                     //　近距離攻撃1ー)
        _mb_Atk2_P = 8,
        _mb_Atk3_P = 10,						//　近距離攻撃2
        _mb_CounterPose_P,				//　カウンター構え
        _mb_CounterRight_P = 13,				//　カウンター（右）
        _mb_CounterLeft_P,				//　カウンター（左）

        _mb_SpAtk_P = 15,			        //　必殺技

        _mb_Damage_Weak_P = 16,			    //　ダメージ
        _mb_Damage_Vanish_Fly_P,          //　吹き飛び中
        _mb_Damage_Vanish_Down_P,         //　吹き飛び着地
        _mb_Damage_Vanish_StandUp_P,      //　起き上がり
        _mb_Damage_Vanish_WallHitAndDown_P = 21,     //　ずり落ち
        _mb_Damage_Vanish_Bound_P,        //　地面でバウンド

        _mb_Change_P = 5,
        _mb_BallCharge = 1,

        _mb_VanishHold_P = 23,       //つかまれている
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

    int m_ChargeBallCount;


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

    void MinusChargeBallCount(int value);

private:
    //　リセット
    void Reset();
};

#endif
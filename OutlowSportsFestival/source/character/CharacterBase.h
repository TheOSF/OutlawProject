#ifndef __CHARACTER_BASE_H__
#define __CHARACTER_BASE_H__

#include "../GameSystem/GameObject.h"
#include "../GameSystem/GameSystem.h"
#include "iextreme.h"

//***************************************************
//		キャラクタクラス
//***************************************************


class CharacterBase :public GameObjectBase
{
private:
	CharacterBase(const CharacterBase&);
public:

	struct PlayerInfo
	{
		PlayerNum::Value		number;			//プレイヤー番号
		PlayerType::Value		player_type;	//プレイヤータイプ
		StrongType::Value		strong_type;	//強さタイプ
		CharacterType::Value	chr_type;		//キャラクタタイプ
	};

	struct Params
	{
		UINT                win;        //勝ったラウンド数
		Vector3				pos;		//座標
		Vector3				move;		//移動量
		float				hitScale;	//あたり判定の円の大きさ
		float				angle;		//アングル(ラジアン)
		float				maxHP;		//最大体力０～
		float				HP;			//０～
		float				SP;			//０～１００
	};

    //キャラクタ共通パラメータ
    struct CommonParams
    {
        float  GroundY; //地面の高さ    
        float  Glavity; //重力加速度
    };

    static const CommonParams m_CommonParams;

	const PlayerInfo	m_PlayerInfo; //プレイヤー情報にする構造体
	Params				m_Params;     //キャラクタ基本パラメーター

   
	CharacterBase(const PlayerInfo& info);
	virtual ~CharacterBase();

    static COLOR GetPlayerColor(PlayerNum::Value number);

	virtual bool Update() = 0;	//この関数がfalseを返すとdeleteされる
	virtual bool Msg(MsgType mt) = 0;	//受信できたメッセージはtrueを
};

#endif
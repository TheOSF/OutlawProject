#ifndef __CHARACTER_BASE_H__
#define __CHARACTER_BASE_H__

#include "../GameSystem/GameObject.h"
#include "../GameSystem/GameSystem.h"
#include "../utillity/ColorUtility.h"

#include "CharacterRenderer.h"
#include "iextreme.h"
#include "CharacterPhysic.h"
#include "../Camera/Camera.h"

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
        float               size;       //他のキャラクタを押しのける大きさ
        bool                DoCheckOtherChrSpace; //他のキャラクタを押しのけるかどうか
	};

    //キャラクタ共通パラメータ
    struct CommonParams
    {
        float  GroundY; //地面の高さ    
        float  Glavity; //重力加速度
    };

    //キャラクタの現在の状態
    enum class State
    {
        Usual,    //通常(戦闘中
        WinPose,  //勝ちポーズ中
        LosePose, //負けポーズ中
        Freeze,   //とまっている
    };

    static const CommonParams m_CommonParams;

	const PlayerInfo	m_PlayerInfo; //プレイヤー情報にする構造体
	Params				m_Params;     //キャラクタ基本パラメーター
    CharacterRenderer	m_Renderer;   //レンダラ
    float               m_ModelSize;  //モデルスケール
    CameraDrawObject    m_DrawObject; //カメラを写すオブジェクト
   
    CharacterBase(const PlayerInfo& info, BlendAnimationMesh* pMesh);
	virtual ~CharacterBase();

    static COLOR  GetPlayerColor (PlayerNum::Value number);
    static COLORf GetPlayerColorF(PlayerNum::Value number);

    static const char* GetCharacterModelPath(CharacterType::Value chr_type);
    static void CreateCharacterModel(CharacterRenderer* pOut, CharacterType::Value chr_type, PlayerNum::Value PlNum);

    static int GetWinMotion(CharacterType::Value chr_type);
    static int GetLoseMotion(CharacterType::Value chr_type);

    //当たり判定をとる座標を得る
    Vector3 CalcHitCheckPos()const;

    //ラウンドリセット時のパラメターにセットする
    void ResetRound();

    //毎フレームの更新
    void BaseUpdate();

	virtual bool Update() = 0;	//この関数がfalseを返すとdeleteされる
    virtual bool CharacterMsg(MsgType mt) = 0; //受信できたメッセージはtrueを
    
    bool Msg(MsgType mt);

    State GetStateType()const;
private:
    CharacterPhysic     m_PhysicObj;  //物理判定オブジェクト
    State               m_StateType;  //現在のステートタイプ

    void CameraDrawObjUpdate();

protected:

    void RendererUpdate(CharacterRenderer* pRenderer);
};

#endif
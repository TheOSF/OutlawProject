#include "CharacterBase.h"
#include "CharacterManager.h"

//***************************************************
//		キャラクタクラス
//***************************************************

const CharacterBase::CommonParams CharacterBase::m_CommonParams =
{
    0.0f,          //地面の高さ    
    -0.01f,        //重力加速度
};

CharacterBase::CharacterBase(const PlayerInfo& info):
m_PlayerInfo(info)
{
	m_Params.pos = Vector3Zero;
	m_Params.move = Vector3Zero;
	m_Params.hitScale = 1.5f;
	m_Params.angle = 0;
	m_Params.HP = 100;
	m_Params.SP = 0;

	//マネージャに登録
	DefCharacterMgr.Add(this);
}

CharacterBase::~CharacterBase()
{
	//マネージャから削除
	DefCharacterMgr.Erace(this);
}

COLOR CharacterBase::GetPlayerColor(PlayerNum::Value number)
{
    MyAssert(((int)number >= 0 && (int)number < 4) , "色が存在しないプレイヤー番号が引数に送られました num= %d ", (int)number);

    static const COLOR player_colors[]=
    {
        0xFFFF0000,
        0xFF0000FF,
        0xFFFFFF00,
        0xFF00FF00
    };

    return player_colors[number];
}
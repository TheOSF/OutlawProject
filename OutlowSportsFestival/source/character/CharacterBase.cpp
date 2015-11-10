#include "CharacterBase.h"
#include "CharacterManager.h"
#include "CharacterFunction.h"
#include "../UI/PlayerCursorBillbord.h"
#include "../Ball/UsualBall.h"

//***************************************************
//		キャラクタクラス
//***************************************************

const CharacterBase::CommonParams CharacterBase::m_CommonParams =
{
    0.0f,          //地面の高さ
    -0.02f,        //重力加速度
};

CharacterBase::CharacterBase(const PlayerInfo& info) :
m_PlayerInfo(info),
m_PhysicObj(this),
m_StateType(State::Freeze)
{

	m_Params.win = 0;
    m_Params.camera_draw = true;
	m_Params.pos = Vector3Zero;
	m_Params.move = Vector3Zero;
	m_Params.hitScale = 1.5f;
	m_Params.angle = 0;
	m_Params.HP = 100;
    m_Params.maxHP = 100;
	m_Params.SP = 0;
    m_Params.size = 1.2f;

    //カーソルＵＩ
    new PlayerCursorBillbord(this);

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
        0xFFFF2000,
        0xFF00FFFF,
        0xFFFFFF00,
        0xFF00FF00
    };

    return player_colors[number];
}

COLORf CharacterBase::GetPlayerColorF(PlayerNum::Value number)
{
    
    COLORf ret(1, 1, 1, 1);

    switch ((int)number)
    {
    case 0: ret = COLORf(1,1,0.145f,0); break;
    case 1: ret = COLORf(1,0,1,1); break;
    case 2: ret = COLORf(1,1,1,0); break;
    case 3: ret = COLORf(1,0,1,0); break;

    default:
        MyAssert(false, "色が存在しないプレイヤー番号が引数に送られました num= %d ", (int)number);
        break;
    }

    return ret;
}

//当たり判定をとる座標を得る
Vector3 CharacterBase::CalcHitCheckPos()const
{
    Vector3 ret = m_Params.pos;

    ret.y += UsualBall::UsualBallShotY;

    return ret;
}

//ラウンドリセット時のパラメターにセットする
void CharacterBase::ResetRound()
{
    m_Params.HP = m_Params.maxHP;
    m_Params.pos = DefCharacterMgr.GetRoundStartPos(m_PlayerInfo.number);
    m_Params.move = Vector3Zero;
    m_Params.camera_draw = true;
    chr_func::AngleControll(this, Vector3(0, m_Params.pos.y, 0));
}

//毎フレームの更新
void CharacterBase::BaseUpdate()
{
    m_PhysicObj.Update();
}

bool CharacterBase::Msg(MsgType mt)
{
    switch (mt)
    {
    case MsgType::_PlayStart:
        m_StateType = State::Usual;
        break;

    case MsgType::_RoundReset:
        m_StateType = State::Freeze;
        ResetRound();
        break;

    case MsgType::_GameSet:
        m_StateType = State::Freeze;
        break;

    case MsgType::_WinPose:
        m_StateType = State::WinPose;
        break;

    default:
        return false;
    }

    return CharacterMsg(mt);
}


CharacterBase::State CharacterBase::GetStateType()const
{
    return m_StateType;
}
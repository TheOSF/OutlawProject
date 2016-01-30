#include "CharacterBase.h"
#include "CharacterManager.h"
#include "CharacterFunction.h"
#include "../UI/PlayerCursorBillbord.h"
#include "../Ball/UsualBall.h"
#include "../UI/PlayerGauge.h"

#include "Tennis\TennisPlayer.h"
#include "Baseball\BaseballPlayer.h"
#include "Soccer\SoccerPlayer.h"
#include "Amefoot\AmefootPlayer.h"

//***************************************************
//		キャラクタクラス
//***************************************************

const CharacterBase::CommonParams CharacterBase::m_CommonParams =
{
    0.000f,          //地面の高さ
    -0.02f,        //重力加速度
};

CharacterBase::CharacterBase(const PlayerInfo& info, BlendAnimationMesh* pMesh) :
m_PlayerInfo(info),
m_PhysicObj(this),
m_StateType(State::Freeze),
m_Renderer(pMesh)
{

	m_Params.win = 0;
	m_Params.pos = Vector3Zero;
	m_Params.move = Vector3Zero;
	m_Params.hitScale = 1.5f;
	m_Params.angle = 0;
	m_Params.HP = 100;
    m_Params.maxHP = 100;
	m_Params.SP = 0;
    m_Params.size = 1.8f;
    m_Params.DoCheckOtherChrSpace = true;
    m_DrawObject.m_isDraw = true;

    //ゲージ
    new PlayerGauge(this);

	//マネージャに登録
	DefCharacterMgr.Add(this);
  
    //モデル構築
    CharacterBase::CreateCharacterModel(&m_Renderer, m_PlayerInfo.chr_type, m_PlayerInfo.number);
    chr_func::CreateTransMatrix(this, &m_Renderer.m_TransMatrix);
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

const char* CharacterBase::GetCharacterModelPath(CharacterType::Value chr_type)
{
    static const char* Paths[] = 
    {
        "DATA\\CHR\\SanoTennis\\tennis.iem",
        "DATA\\CHR\\SanoBaseBall\\baseball.iem",
        "DATA\\CHR\\SanoSoccer\\soccer.iem",
        "DATA\\CHR\\AmericanFootball\\AmericanFootball_v6.iem",
    };

    MyAssert((int)chr_type >= 0 && (int)chr_type < 4, "存在しないキャラクタタイプが引数に送られました type= %d ", (int)chr_type);

    return Paths[(int)chr_type];

}

void CharacterBase::CreateCharacterModel(CharacterRenderer* pOut, CharacterType::Value chr_type, PlayerNum::Value PlNum)
{

    //マテリアル指定
    switch (chr_type)
    {
    case CharacterType::_Tennis:
        pOut->SetMaterialRenderType(0, CharacterRenderer::RenderType::Face);
        pOut->SetMaterialRenderType(1, CharacterRenderer::RenderType::Skin);
        pOut->SetMaterialRenderType(2, CharacterRenderer::RenderType::Clothes);
        break;

    case CharacterType::_Baseball:
        pOut->SetMaterialRenderType(0, CharacterRenderer::RenderType::Face);
        pOut->SetMaterialRenderType(1, CharacterRenderer::RenderType::Clothes);
        pOut->SetMaterialRenderType(2, CharacterRenderer::RenderType::Skin);
        break;

    case CharacterType::_Soccer:
        pOut->SetMaterialRenderType(0, CharacterRenderer::RenderType::Hair);
        pOut->SetMaterialRenderType(1, CharacterRenderer::RenderType::Face);
        pOut->SetMaterialRenderType(2, CharacterRenderer::RenderType::Skin);
        pOut->SetMaterialRenderType(3, CharacterRenderer::RenderType::Clothes);
        break;

    case CharacterType::_Americanfootball:
        pOut->SetMaterialRenderType(0, CharacterRenderer::RenderType::Clothes);
        pOut->SetMaterialRenderType(1, CharacterRenderer::RenderType::Bloom);
        pOut->SetMaterialRenderType(2, CharacterRenderer::RenderType::Normal);
        break;

    default:
        MyAssert(false, "存在しないキャラクタタイプが引数に送られました type= %d ", (int)chr_type);
        break;
    }

    //服の色
    {
        COLORf col = GetPlayerColorF(PlNum);
        pOut->m_ClothesColor = Vector3(col.r, col.g, col.b);
    }
}

int CharacterBase::GetWinMotion(CharacterType::Value chr_type)
{
    int Motions[]=
    {
        (int)TennisPlayer::_mt_WinPose,
        (int)baseball_player::_mb_Stand_P,
        (int)SoccerPlayer::_ms_Win,
        (int)AmefootPlayer::Motion_Win_Pose,
    };

    MyAssert((int)chr_type >= 0 && (int)chr_type < 4, "存在しないキャラクタタイプが引数に送られました type= %d ", (int)chr_type);

    return Motions[(int)chr_type];
}

int CharacterBase::GetLoseMotion(CharacterType::Value chr_type)
{
    int Motions[] =
    {
        (int)TennisPlayer::_mt_LosePose,
        (int)baseball_player::_mb_Stand_P,
        (int)SoccerPlayer::_ms_Lose,
        (int)AmefootPlayer::Motion_Lose_Pose,
    };

    MyAssert((int)chr_type >= 0 && (int)chr_type < 4, "存在しないキャラクタタイプが引数に送られました type= %d ", (int)chr_type);

    return Motions[(int)chr_type];
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
    m_DrawObject.m_isDraw = true;

    chr_func::AngleControll(this, Vector3(0, m_Params.pos.y, 0));
}

//毎フレームの更新
void CharacterBase::BaseUpdate()
{
    m_PhysicObj.Update();
    RendererUpdate(&m_Renderer);
    CameraDrawObjUpdate();
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

	case MsgType::_LosePose:
		m_StateType = State::LosePose;
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

void CharacterBase::RendererUpdate(CharacterRenderer* pRenderer)
{
    Vector3  LerpColor(0, 0, 0);
    Vector3  Color(pRenderer->m_OutlineColor.r, pRenderer->m_OutlineColor.g, pRenderer->m_OutlineColor.b);

    if (chr_func::isCanSpecialAttack(this) && chr_func::isDie(this) == false)
    {
        LerpColor = Vector3(1.8f, 1.25f, 1.0f);
    }

    switch (GetStateType())
    {
    case State::Usual:
        Color += (LerpColor - Color)*0.1f;
        break;

    default:
        Color += -Color * 0.1f;
        break;
    }

    pRenderer->m_OutlineColor = COLORf(0, Color.x, Color.y, Color.z);
    pRenderer->m_OutlineVisible = Color.Length() > 0.1f;
}

void CharacterBase::CameraDrawObjUpdate()
{
    m_DrawObject.m_Pos = m_Params.pos;
}
#include "BaseballPlayer.h"
#include "BaseballPlayerState.h"

#include "Computer\/BaseballPlayerState_ComMove.h"
#include "BaseballState_Change.h"
#include "../../Damage/Damage.h"
#include "../../GameSystem/GameController.h"
#include "../../Ball/UsualBall.h"
#include "../../Effect/EffectFactory.h"
#include "Sound/Sound.h"
#include "../CharacterFunction.h"
#include "../CharacterManager.h"

//*************************************************************
//		野球プレイヤークラス
//*************************************************************

//　コンストラクタ
BaseballPlayer::BaseballPlayer(const CharacterBase::PlayerInfo& info) :
CharacterBase(info, new  BlendAnimationMesh(GetCharacterModelPath(CharacterType::_Baseball))),
m_BatterFlg(true)
{
	m_pStateMachine = new BaseballStateMachine(this);
	SetState(BaseballState_PlayerControll_Move::GetPlayerControllMove(this));

	m_Params.maxHP = m_Params.HP = 100;
	
    m_ModelSize = 0.06f;
}

//　デストラクタ
BaseballPlayer::~BaseballPlayer()
{
	delete m_pStateMachine;
	
}

//現在フォームのモデルにモーションをセット
void BaseballPlayer::SetMotion(int motion)
{
    CharacterRenderer* const pNow = getNowModeModel();
    
    pNow->SetMotion(motion);
}

//現在フォームのモデルを更新
void BaseballPlayer::ModelUpdate(RATIO Speed)
{
    getNowModeModel()->Update(Speed);
}

//　ステートセット
bool BaseballPlayer::SetState(BaseballState* state, int Important)
{
	//　新しいステートをセット
	return m_pStateMachine->set_state(state,Important);
}

//　更新
bool BaseballPlayer::Update()
{
	// ステート実行
	m_pStateMachine->state_execute();

	//キャラクタ基本更新
	BaseUpdate();
	
	return true;	//常にtrueを返すと消去されない
}


bool  BaseballPlayer::CharacterMsg(MsgType mt)
{
	//　ラウンドごとにリセット
	if (mt == MsgType::_RoundReset)
	{
		Reset();
	}

	return m_pStateMachine->Msg(mt);
}

//　装備切り替え
void BaseballPlayer::ChangeMode()
{
    SetMode(!m_BatterFlg);
}

//フォームを指定してセットする
bool BaseballPlayer::SetMode(bool isBatter)
{
    //同じだった場合
    if (isBatter == m_BatterFlg)
    {
        return false;
    }

    CharacterRenderer*const pPre = getNowModeModel();

    m_BatterFlg = isBatter;

    //行列を同期
    getNowModeModel()->m_TransMatrix = pPre->m_TransMatrix;

    return true;
}

//　リセット
void BaseballPlayer::Reset()
{
	SetState(BaseballState_PlayerControll_Move::GetPlayerControllMove(this));
}


//現在フォームのモデルをゲット
CharacterRenderer* BaseballPlayer::getNowModeModel()
{
    if (isBatter())
    {
        return m_pBatterModel;
    }

    return m_pPitcherModel;
}
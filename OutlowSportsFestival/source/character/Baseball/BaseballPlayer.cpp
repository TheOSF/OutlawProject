#include "BaseballPlayer.h"
#include "BaseballPlayerState.h"
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
CharacterBase(info), batterflg(false), m_ModelSize(0.05f), changetime(18),
m_Renderer(new  BlendAnimationMesh("DATA\\CHR\\BaseBall\\player_B.iem"))
{
	m_pStateMachine = new BaseballStateMachine(this);

	//　体力低下(デバック用)
	m_Params.maxHP = m_Params.HP = 10;
}

//　デストラクタ
BaseballPlayer::~BaseballPlayer(){
	delete m_pStateMachine;
}

//　ステートセット
void BaseballPlayer::SetState(BaseballState* state){
	//　新しいステートをセット
	m_pStateMachine->set_state(state);
}

//　更新
bool BaseballPlayer::Update(){
	//　切り替え
	Change();
	// ステート実行
	m_pStateMachine->state_execute();

	return true;	//常にtrueを返すと消去されない
}

bool  BaseballPlayer::Msg(MsgType mt)
{
	//　ラウンドごとにリセット
	if (mt == MsgType::_RoundReset)
	{
		Riset();
	}

	return m_pStateMachine->Msg(mt);
}

//　切り替え
void BaseballPlayer::Change()
{
	//　切り替え可能時間増加
	changetime++;
	if (controller::GetTRG(controller::button::_L1, m_PlayerInfo.number)){
		if (changetime >= 18)
		{
			//　エフェクト
			EffectFactory::Counter(Vector3(m_Params.pos.x, m_Params.pos.y + 2.0f, m_Params.pos.z), 10.0f);
			//　効果音
			Sound::Play(Sound::Change);
			if (batterflg){
				batterflg = false;
			}
			else{
				batterflg = true;
			}
			//　リセット
			changetime = 0;
		}
	}

}


//　リセット
void BaseballPlayer::Riset()
{
	SetState(BaseballState_PlayerControll_Move::GetPlayerControllMove(this));
	m_Renderer.SetMotion(baseball_player::_mb_Stand);
	m_Renderer.Update(0);
	ResetRound();
	batterflg = true;
	changetime = 30;

}
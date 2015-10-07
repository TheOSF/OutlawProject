#include "BaseballPlayer.h"
#include "../../Damage/Damage.h"
#include "../../GameSystem/GameController.h"
#include "../../Ball/UsualBall.h"
#include "../../Effect/EffectFactory.h"

#include "../CharacterFunction.h"
#include "../CharacterManager.h"

//*************************************************************
//		野球プレイヤークラス
//*************************************************************

//　コンストラクタ
BaseballPlayer::BaseballPlayer(const CharacterBase::PlayerInfo& info) :
CharacterBase(info), batterflg(false),  m_ModelSize(0.05f),
m_Renderer(new  BlendAnimationMesh("DATA\\CHR\\BaseBall\\player_B.iem"))
{
	m_pStateMachine = new BaseballStateMachine(this);
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

bool  BaseballPlayer::Msg(MsgType mt){
	return m_pStateMachine->Msg(mt);
}

//　切り替え
void BaseballPlayer::Change(){
	if (controller::GetTRG(controller::button::_L1, m_PlayerInfo.number)){
		//　エフェクト
		EffectFactory::Counter(Vector3(m_Params.pos.x, m_Params.pos.y + 2.0f, m_Params.pos.z), 10.0f);
		if (batterflg){
			batterflg = false;
		}
		else{
			batterflg = true;
		}
	}

}

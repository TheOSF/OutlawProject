#include "BaseballState_Change.h"
#include "BaseballPlayerState.h"
#include "../../GameSystem/GameController.h"
#include "../CharacterFunction.h"
#include "../CharacterManager.h"

#include "../../Effect/EffectFactory.h"
#include "Sound/Sound.h"

//-------------切り替えステートクラス-------------

BaseballState_Change::BaseballState_Change():
batterflg(false)
{

}


BaseballState_Change::~BaseballState_Change()
{

}

// ステート開始
void  BaseballState_Change::Enter(BaseballPlayer* b)
{
	
}


// ステート実行
void BaseballState_Change::Execute(BaseballPlayer* b)
{
	batterflg = b->getBatterFlg();
	if (b->getChangeTime() >= 20)
	{
		//　エフェクト
		EffectFactory::Counter(Vector3(b->m_Params.pos.x, b->m_Params.pos.y + 2.0f, b->m_Params.pos.z), 10.0f);
		//　効果音
		Sound::Play(Sound::Change);
		if (batterflg)
		{
			b->setBatterFlg(false);
		}
		else{
			b->setBatterFlg(true);
		}
		//　リセット
		b->setChangeTime(0);
	}

	b->SetState(BaseballState_PlayerControll_Move::GetPlayerControllMove(b));
}

// ステート終了
void BaseballState_Change::Exit(BaseballPlayer* b)
{

}
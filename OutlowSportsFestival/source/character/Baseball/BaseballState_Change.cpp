#include "BaseballState_Change.h"
#include "BaseballPlayerState.h"
#include "../../GameSystem/GameController.h"
#include "../CharacterFunction.h"
#include "../CharacterManager.h"
#include "Baseball_HitEvent.h"
#include "../../Effect/EffectFactory.h"
#include "Sound/Sound.h"

//-------------切り替えステートクラス-------------

BaseballState_Change::BaseballState_Change() :
batterflg(false), m_Timer(0)
{

}


BaseballState_Change::~BaseballState_Change()
{

}

// ステート開始
void  BaseballState_Change::Enter(BaseballPlayer* b)
{
	m_pBaseball = b;
}


// ステート実行
void BaseballState_Change::Execute(BaseballPlayer* b)
{
    ++m_Timer;

    chr_func::XZMoveDown(b, 0.05f);

    if (m_Timer == 1)
    {
        //　エフェクト
        EffectFactory::Change(Vector3(m_pBaseball->m_Params.pos.x, m_pBaseball->m_Params.pos.y + 2.0f, m_pBaseball->m_Params.pos.z), 10.0f);
        //　効果音
        Sound::Play(Sound::Change);

        b->ChangeMode();
    }

	
	chr_func::UpdateAll(b, &BaseballHitEvent(b));
    b->ModelUpdate();
    chr_func::CreateTransMatrix(b, &b->getNowModeModel()->m_TransMatrix);
}

// ステート終了
void BaseballState_Change::Exit(BaseballPlayer* b)
{

}


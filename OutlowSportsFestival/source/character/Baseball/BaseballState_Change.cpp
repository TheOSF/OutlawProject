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

    //　エフェクト
    EffectFactory::Change(m_pBaseball->m_Params.pos + Vector3(0, 2, 0));
    //　効果音
    Sound::Play(Sound::Change);

    b->ChangeMode();

    if (b->isBatter())
    {
        b->SetMotion(baseball_player::_mb_Change_B);
    }
    else
    {
        b->SetMotion(baseball_player::_mb_Change_P);
    }
}


// ステート実行
void BaseballState_Change::Execute(BaseballPlayer* b)
{
    ++m_Timer;

    chr_func::XZMoveDown(b, 0.05f);

    if (m_Timer > 25)
    {
        b->SetState(BaseballState_PlayerControll_Move::GetPlayerControllMove(b));
    }

    b->ModelUpdate();

	chr_func::UpdateAll(b, &BaseballHitEvent(b));
    
    chr_func::CreateTransMatrix(b, &b->getNowModeModel()->m_TransMatrix);
}

// ステート終了
void BaseballState_Change::Exit(BaseballPlayer* b)
{

}


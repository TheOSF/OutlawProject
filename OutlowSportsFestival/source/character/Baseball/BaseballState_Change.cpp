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
	m_pStateFunc = &BaseballState_Change::State_Change;
}


// ステート実行
void BaseballState_Change::Execute(BaseballPlayer* b)
{
	
	chr_func::XZMoveDown(b, 0.05f);
	(this->*m_pStateFunc)();
	chr_func::UpdateAll(b, &BaseballHitEvent(b));
	b->m_Renderer.Update(1);
	chr_func::CreateTransMatrix(b, b->m_ModelSize, &b->m_Renderer.m_TransMatrix);
}

// ステート終了
void BaseballState_Change::Exit(BaseballPlayer* b)
{

}

void BaseballState_Change::State_Change()
{


	if (m_pBaseball->getChangeTime() >= 20)
	{
		m_Timer++;
		if (m_Timer == 1)
		{
			batterflg = m_pBaseball->getBatterFlg();

			//　エフェクト
			EffectFactory::Change(Vector3(m_pBaseball->m_Params.pos.x, m_pBaseball->m_Params.pos.y + 2.0f, m_pBaseball->m_Params.pos.z), 10.0f);
			//　効果音
			Sound::Play(Sound::Change);
			m_pBaseball->m_Renderer.SetMotion(baseball_player::_mb_WinPose);
		}

		if (m_Timer == 3)
		{
			if (batterflg)
			{
				m_pBaseball->setBatterFlg(false);
			}
			else{
				m_pBaseball->setBatterFlg(true);
			}
			m_pStateFunc = &BaseballState_Change::State_End;
		}

	}

}

void BaseballState_Change::State_End()
{
	const int EndFrame = 20;

	if (++m_Timer > EndFrame)
	{
		//　リセット
		m_Timer = 0;
		m_pBaseball->setChangeTime(0);
		m_pBaseball->SetState(BaseballState_PlayerControll_Move::GetPlayerControllMove(m_pBaseball));
	}
}
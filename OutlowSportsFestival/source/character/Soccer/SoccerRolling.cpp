#include "SoccerRolling.h"
#include "../CharacterFunction.h"
#include "SoccerPlayerState.h"
#include "SoccerHitEvent.h"
#include "../../Effect/EffectFactory.h"
#include "../../Sound/Sound.h"


SoccerState_Rolling::SoccerState_Rolling(CallBackClass* pCallBackClass,bool dash) :
m_pCallBackClass(pCallBackClass), Dash(dash)
{

}

SoccerState_Rolling::~SoccerState_Rolling()
{
	delete m_pCallBackClass;
}


// ステート開始
void SoccerState_Rolling::Enter(SoccerPlayer* s)
{
	m_Timer = 0;

	//デフォルトのローリング方向としてキャラの前ベクトルを入れる
	chr_func::GetFront(s, &m_Vec);

	//キャラクタの移動量を初期化
	chr_func::ResetMove(s);
}

// ステート実行
void SoccerState_Rolling::Execute(SoccerPlayer* s)
{
	const int EndFrame = 52;          //終了までのフレーム
	const int CanControllFrame = 2;   //移動方向をコントロールできるフレーム
	const int NoDamageFrame = 10;     //無敵時間

	float DownValue = 0.5f;
	float MoveValue = 0.05f;
	
	if (Dash)
	{
		 MoveValue = 0.6f;    //移動量
		 DownValue = 0.1f;     //減速量
	}
	else
	{
		 MoveValue = 0.4f;    //移動量
		 DownValue = 0.09f;     //減速量
	}

	

	//フレームカウント更新
	++m_Timer;

	//モーションセット
	if (m_Timer == 1)
	{
		s->m_Renderer.SetMotion(SoccerPlayer::_ms_Rolling);
	}

	//移動方向をコントロール
	if (m_Timer < CanControllFrame)
	{
		Vector3 Vec = m_pCallBackClass->GetVec();

		//値があった場合は更新
		if (Vec != Vector3Zero)
		{
			m_Vec = Vec;
			chr_func::AngleControll(s, s->m_Params.pos + m_Vec, 0.3f);
		}
	}

	//コントロールできるフレームが終わった場合向きと移動を設定
	if (m_Timer == CanControllFrame)
	{
		/*m_Vec.y = 0;
		m_Vec.Normalize();*/

		chr_func::AngleControll(s, s->m_Params.pos + m_Vec);

		chr_func::AddMoveFront(s, MoveValue, MoveValue);
	}

	//終了
	if (m_Timer == EndFrame)
	{
		//通常ステートに移行
		s->SetState(SoccerState_PlayerControll_Move::GetPlayerControllMove(s));
	}

	//煙エフェクト
	{
		//スタート時
		if (m_Timer == 2)
		{
			for (int i = 0; i < 2; ++i)
			{
				EffectFactory::Smoke(
					s->m_Params.pos + Vector3(frand() - 0.5f, frand(), frand() - 0.5f)*2.0f,
					Vector3Zero,
					1.5f,
					1.0f,
					true
					);
			}
		}

		//軌跡
		if (m_Timer < 5)
		{
			for (int i = 0; i < 2; ++i)
			{
				EffectFactory::Smoke(
					s->m_Params.pos + Vector3(0, 2, 0) + Vector3Rand() * 0.2f,
					Vector3Zero,
					1.0f + frand()*0.5f,
					0.6f
					);
			}
		}

		//着地時
		if (m_Timer == 43)
		{
			for (int i = 0; i < 2; ++i)
			{
				EffectFactory::Smoke(
					s->m_Params.pos + Vector3(frand() - 0.5f, frand(), frand() - 0.5f)*2.0f,
					Vector3Zero,
					2.5f,
					1.0f,
					true
					);
			}
		}

	}
	if (m_Timer == EndFrame - 25)
	{
		//ズザー音
		Sound::Play(Sound::Sand1);
	}
	//基本的な更新
	{
		DamageManager::HitEventBase NoDmgHitEvent;   //ノーダメージ
		SoccerHitEvent              UsualHitEvent(s);//通常


		//移動量の減衰
		if (m_Timer > EndFrame - 13)
		{
			chr_func::XZMoveDown(s, DownValue);
		}

		//無敵フレームかによってヒットイベントクラスの分岐
		if (m_Timer < NoDamageFrame)
		{
			chr_func::UpdateAll(s, &NoDmgHitEvent);
		}
		else
		{
			chr_func::UpdateAll(s, &UsualHitEvent);
		}

		//モデル更新
		s->m_Renderer.Update(1);

		//行列更新
		chr_func::CreateTransMatrix(s, s->m_ModelSize, &s->m_Renderer.m_TransMatrix);
	}
}

// ステート終了
void SoccerState_Rolling::Exit(SoccerPlayer* t)
{

}
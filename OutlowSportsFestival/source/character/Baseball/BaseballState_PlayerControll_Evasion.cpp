#include "BaseballState_PlayerControll_Evasion.h"
#include "../CharacterFunction.h"
#include "BaseballPlayerState.h"
#include "Baseball_HitEvent.h"
#include "../../Effect/EffectFactory.h"
#include "../../Sound/Sound.h"



BaseballState_Rolling::BaseballState_Rolling(CallBackClass* pCallBackClass) :
m_pCallBackClass(pCallBackClass)
{

}

BaseballState_Rolling::~BaseballState_Rolling()
{
	delete m_pCallBackClass;
}


// ステート開始
void BaseballState_Rolling::Enter(BaseballPlayer* b)
{
	m_Timer = 0;

	//デフォルトのローリング方向としてキャラの前ベクトルを入れる
	chr_func::GetFront(b, &m_Vec);

	//キャラクタの移動量を初期化
	chr_func::ResetMove(b);
}

// ステート実行
void BaseballState_Rolling::Execute(BaseballPlayer* b)
{
	const int EndFrame = 52;          //終了までのフレーム
	const int CanControllFrame = 2;   //移動方向をコントロールできるフレーム
	const int NoDamageFrame = 10;     //無敵時間
	const float DownValue = 0.05f;     //減速量
	float MoveValue = 0.0f;

	//　バッターorピッチャーの移動量
	if (b->getBatterFlg())
	{
		MoveValue = 0.5f;    //バッター時
	}
	else
	{
		MoveValue = 0.8f;    //ピッチャー時
	}

	//フレームカウント更新
	++m_Timer;

	//モーションセット
	if (m_Timer == 1)
	{
		b->m_Renderer.SetMotion(baseball_player::_mb_Evasion);
	}

	//移動方向をコントロール
	if (m_Timer < CanControllFrame)
	{
		Vector3 Vec = m_pCallBackClass->GetVec();

		//値があった場合は更新
		if (Vec != Vector3Zero)
		{
			m_Vec = Vec;
		}
	}

	//コントロールできるフレームが終わった場合向きと移動を設定
	if (m_Timer == CanControllFrame)
	{
		m_Vec.y = 0;
		m_Vec.Normalize();

		chr_func::AngleControll(b, b->m_Params.pos + m_Vec);

		chr_func::AddMoveFront(b, MoveValue, MoveValue);
	}

	//終了
	if (m_Timer == EndFrame)
	{
		//通常ステートに移行
		b->SetState(BaseballState_PlayerControll_Move::GetPlayerControllMove(b));
	}

	//煙エフェクト
	{
		//スタート時
		if (m_Timer == 2)
		{
			for (int i = 0; i < 2; ++i)
			{
				EffectFactory::Smoke(
					b->m_Params.pos + Vector3(frand() - 0.5f, frand(), frand() - 0.5f)*2.0f,
					Vector3Zero,
					1.5f,
					0xFFFFA080,
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
					b->m_Params.pos + Vector3(0, 2, 0) + Vector3Rand() * 0.2f,
					Vector3Zero,
					1.0f + frand()*0.5f,
					0x20FFA080
					);
			}
		}

		//着地時
		if (m_Timer == 43)
		{
			for (int i = 0; i < 2; ++i)
			{
				EffectFactory::Smoke(
					b->m_Params.pos + Vector3(frand() - 0.5f, frand(), frand() - 0.5f)*2.0f,
					Vector3Zero,
					2.5f,
					0xFFFFA080,
					true
					);
			}
		}

	}

	//サウンド
	if (m_Timer == EndFrame - 25)
	{
		//ズザー音
		Sound::Play(Sound::Sand1);
	}

	//基本的な更新
	{
		DamageManager::HitEventBase NoDmgHitEvent;   //ノーダメージ
		BaseballHitEvent              UsualHitEvent(b);//通常


		//移動量の減衰
		chr_func::XZMoveDown(b, DownValue);

		//無敵フレームかによってヒットイベントクラスの分岐
		if (m_Timer < NoDamageFrame)
		{
			chr_func::UpdateAll(b, &NoDmgHitEvent);
		}
		else
		{
			chr_func::UpdateAll(b, &UsualHitEvent);
		}

		//モデル更新
		b->m_Renderer.Update(1);

		//行列更新
		chr_func::CreateTransMatrix(b, b->m_ModelSize, &b->m_Renderer.m_TransMatrix);
	}
}

// ステート終了
void BaseballState_Rolling::Exit(BaseballPlayer* b)
{

}
#include "BaseballState_PlayerControll_Evasion.h"
#include "../CharacterFunction.h"
#include "BaseballPlayerState.h"
#include "Baseball_HitEvent.h"
#include "../../Effect/EffectFactory.h"
#include "../../Sound/Sound.h"
#include "../../GameSystem/GameController.h"


BaseballState_Rolling::BaseballState_Rolling()
{
    m_Vec = Vector3Zero;
}

BaseballState_Rolling::BaseballState_Rolling(CrVector3 Vec)
{
    m_Vec = Vec;
}

BaseballState_Rolling::~BaseballState_Rolling()
{

}


// ステート開始
void BaseballState_Rolling::Enter(BaseballPlayer* b)
{
	m_Timer = 0;

	//デフォルトのローリング方向としてキャラの前ベクトルを入れる
    if (m_Vec.Length() < 0.1f)
    {
        chr_func::GetFront(b, &m_Vec);
    }

	//キャラクタの移動量を初期化
	chr_func::ResetMove(b);

    SetParam(b);
}

// ステート実行
void BaseballState_Rolling::Execute(BaseballPlayer* b)
{
	const int CanControllFrame = 4;   //移動方向をコントロールできるフレーム

	//フレームカウント更新
	++m_Timer;

	//モーションセット
	if (m_Timer == 1)
	{
        b->m_Renderer.SetMotion(m_Param.Motion);

        //ズザー音
        Sound::Play(Sound::Sand1);
	}

	//移動方向をコントロール
	if (m_Timer < CanControllFrame)
	{
        if (b->m_PlayerInfo.player_type == PlayerType::_Player)
        {
            m_Vec = GetPlayerControllMoveVec(b);
        }

        //移動方向をコントロール
        chr_func::AngleControll(b, b->m_Params.pos + m_Vec, 0.3f);
	}

    //移動値を徐々にダウン
    chr_func::XZMoveDown(b, m_Param.MoveDownFrame);

	//コントロールできるフレームが終わった場合向きと移動を設定
	if (m_Timer == CanControllFrame)
	{
		chr_func::AngleControll(b, b->m_Params.pos + m_Vec);

		chr_func::AddMoveFront(b, m_Param.MoveValue, 100.0f);
	}



	//終了
	if (m_Timer >= m_Param.AllFrame)
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
					1.0f,
					true
					);
			}
		}

	}

	//基本的な更新
	{
		DamageManager::HitEventBase NoDmgHitEvent;   //ノーダメージ
		BaseballHitEvent            UsualHitEvent(b);//通常


		//無敵フレームかによってヒットイベントクラスの分岐
		if (m_Timer < m_Param.NoDamageFrame)
		{
			chr_func::UpdateAll(b, &NoDmgHitEvent);
		}
		else
		{
			chr_func::UpdateAll(b, &UsualHitEvent);
		}

		//モデル更新
		b->m_Renderer.Update(2);

		//行列更新
		chr_func::CreateTransMatrix(b, &b->m_Renderer.m_TransMatrix);
	}
}

// ステート終了
void BaseballState_Rolling::Exit(BaseballPlayer* b)
{

}

Vector3 BaseballState_Rolling::GetPlayerControllMoveVec(BaseballPlayer* b)
{   
    Vector2 st = controller::GetStickValue(controller::stick::left, b->m_PlayerInfo.number);
    Vector3 ret(st.x, 0, st.y);

    return ret;
}

void BaseballState_Rolling::SetParam(BaseballPlayer* b)
{
    if (b->getBatterFlg())
    {
        m_Param.AllFrame = 40;
        m_Param.Motion = baseball_player::_mb_Evasion_B;
        m_Param.NoDamageFrame = 15;
        m_Param.MoveValue = 0.8f;
        m_Param.MoveDownFrame = 0.05f;
    }
    else
    {
        m_Param.AllFrame = 30;
        m_Param.Motion = baseball_player::_mb_Evasion_P;
        m_Param.NoDamageFrame = 10;
        m_Param.MoveValue = 0.8f;
        m_Param.MoveDownFrame = 0.08f;
    }
}
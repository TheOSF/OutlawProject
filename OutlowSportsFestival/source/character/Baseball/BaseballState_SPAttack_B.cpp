#include "BaseballState_SPAttack_B.h"
#include "BaseballPlayerState.h"
#include "Computer\/BaseballPlayerState_ComMove.h"
#include "../../GameSystem/GameController.h"
#include "../CharacterFunction.h"
#include "../CharacterManager.h"

#include "../../Effect/GlavityLocus.h"
#include "../../Effect/BlurImpact.h"
#include "../../Effect/ThunderEffect.h"
#include "../../Camera/Camera.h"
#include "../../Sound/Sound.h"
#include "../../Effect/SpecialAttackEffect.h"

//-------------近距離攻撃ステートクラス-------------

BaseballState_SPAttack_B::BaseballState_SPAttack_B(BaseballPlayer* b) :timeflg(false)
{

	m_Damage.m_Enable = false;
	m_Damage.HitCount = 0;
	m_Damage.m_Param.pos = Vector3Zero;
	m_Damage.m_Param.size = 3.0f;
	m_Damage.pBall = nullptr;
	m_Damage.pParent = b;
	m_Damage.type = DamageBase::Type::_VanishDamage;
	m_Damage.Value = 0.0f;
	m_Damage.m_Vec = Vector3AxisZ;

    m_Damage.AddSkillGaugeValue = 0;

	m_Light.Visible = false;
}


BaseballState_SPAttack_B::~BaseballState_SPAttack_B()
{

}

// ステート開始
void  BaseballState_SPAttack_B::Enter(BaseballPlayer* b)
{


	m_Timer = 0;

	m_pStateFunc = &BaseballState_SPAttack_B::State_Atk;

	m_pBaseBall = b;

	chr_func::XZMoveDown(b, 1);


	//　スキルゲージリセット
	chr_func::ResetSkillGauge(b);

}


// ステート実行
void BaseballState_SPAttack_B::Execute(BaseballPlayer* b)
{
	//ノーダメージ
	DamageManager::HitEventBase NoDmgHitEvent;

	//　実行
	(this->*m_pStateFunc)();

	//　無敵
	chr_func::UpdateAll(b, &NoDmgHitEvent);

    b->ModelUpdate(2);
    chr_func::CreateTransMatrix(b, &b->getNowModeModel()->m_TransMatrix);

	if (m_pStateFunc == &BaseballState_SPAttack_B::State_Finish)
	{

		if (b->m_PlayerInfo.player_type == PlayerType::_Player)
		{
			//攻撃終了時に通常移動モードに戻る
			b->SetState(new BaseballState_PlayerControll_Move());
		}
		else
		{
			b->SetState(new BaseballPlayerState_ComMove());
		}
	}
}

// ステート終了
void BaseballState_SPAttack_B::Exit(BaseballPlayer* b)
{

}



void BaseballState_SPAttack_B::State_Atk()
{
	m_Timer++;
	if (!timeflg)
	{
		//　発動音再生&The World
		if (m_Timer == 1)
		{
			Sound::Play(Sound::Skill);
			//エフェクト
			new SpecialAttackEffect(m_pBaseBall, 55);
			FreezeGame(55);
		}
		//　雷エフェクト発動
		ThunderInvoke(8);

		//　モーション開始
		if (m_Timer == 1)
		{
			m_pBaseBall->SetMotion(baseball_player::_mb_SpAtk_B);
		}

		if (m_Timer >= 56)
		{
			timeflg = true;
			m_Timer = 0;
		}
	}
	else
	{
		//　雷エフェクト発動
		ThunderInvoke(8);

		//　当たり判定の場所とか
		if (m_Timer == 1)
		{
			m_Damage.m_Enable = true;
			chr_func::GetFront(m_pBaseBall, &m_Damage.m_Param.pos);
			m_Damage.m_Vec = m_Damage.m_Param.pos;
			m_Damage.m_Vec.y = 0.8f;//　飛ぶ角度
			m_Damage.m_VecPower.x = 1.0f;
			m_Damage.m_VecPower.y = 1.5f;
			m_Damage.m_Param.pos *= 3.0f;
			m_Damage.m_Param.pos += m_pBaseBall->m_Params.pos;
			m_Damage.Value = 50.0f;
			m_ChrLiveCount = DefCharacterMgr.GetCharacterLiveCount();
		}
		else
		{
			m_Damage.m_Enable = false;
		}

		if (m_Damage.HitCount > 0 && m_Timer == 2)
		{
			//　カキーン
			Sound::Play(Sound::BaseBall_SP);

			Vector3 power(0, -0.02f, 0);
			GlavityLocus* g;

			const Vector4
				stCol(1, 1, 1, 0.0f),
				endCol(1, 1, 1, 0);

			const Vector4
				stHdCol(1, 1, 1, 0.5f),
				endHdCol(1, 1, 1, 0);

			const Vector3 move = Vector3Normalize(m_Damage.m_Vec)*0.8f;

			Vector3 pos = m_Damage.m_Param.pos + Vector3(0, 3, 0);


			//　エフェクト
			for (int i = 0; i < 60; ++i)
			{
				g = new GlavityLocus(
					pos, move*1.5f + Vector3Rand() * 0.8f, power, 4, 40 + rand() % 30
					);

				g->m_BoundRatio = 0.2f;
				g->m_CheckWall = false;

				g->m_Locus.m_StartParam.Color = stCol;
				g->m_Locus.m_EndParam.Color = endCol;

				g->m_Locus.m_StartParam.HDRColor = stHdCol;
				g->m_Locus.m_EndParam.HDRColor = endHdCol;

				g->m_Locus.m_StartParam.Width = 0.1f;
				g->m_Locus.m_EndParam.Width = 0.00f;

			}

			//　雷
			for (int i = 0; i < 8; i++)
			{
				new ThunderEffect
					(pos/* - move*0.2f*i,*/,
					pos + move * 10 + Vector3Rand() * 10,
					4.5f,
					0.1f,
					10,
					Vector4(0, 0, 1, 0),
					15);
			}

			m_Light.param.color = Vector3(0, 0.4f, 0.8f);
			m_Light.param.pos = pos;
			m_Light.param.size = 50.0f;
			m_Light.Visible = true;


			//　ブラー
			new BlurImpactSphere(
				pos,
				30,
				100,
				10
				);

			//　画面揺れ
			DefCamera.SetShock(Vector2(0.5f, 0.5f), 50);

			if (m_ChrLiveCount == DefCharacterMgr.GetCharacterLiveCount())
			{
				std::list<LpGameObjectBase> UpdateObjList;

				DefGameObjMgr.FreezeOtherObjectUpdate(UpdateObjList, 20);

			}
		}

		m_Light.param.size *= 0.9f;

		if (m_Timer == 30)
		{
			m_Timer = 0;
			m_pStateFunc = &BaseballState_SPAttack_B::State_Finish;
		}
	}
}

//　The World
void BaseballState_SPAttack_B::FreezeGame(UINT frame)
{
	std::list<LpGameObjectBase> UpdateObjList;

	UpdateObjList.push_back(m_pBaseBall);

	DefGameObjMgr.FreezeOtherObjectUpdate(UpdateObjList, frame, true);

	
}

//　雷エフェクト発動
void BaseballState_SPAttack_B::ThunderInvoke(UINT point_num)
{
	Matrix  BoneMat;
	Vector3 Forward;
	Vector3 Pos;
	Vector3 Thundderpos;
    m_pBaseBall->getNowModeModel()->GetWorldBoneMatirx(BoneMat, 8);

	Pos = Vector3(BoneMat._41, BoneMat._42, BoneMat._43);

	Forward = Vector3(BoneMat._31, BoneMat._32, BoneMat._33);
	Forward.Normalize();
	Thundderpos = Pos + Forward*0.8f;

	new ThunderEffect
		(Pos,
		Thundderpos + Vector3Rand() * 3,
		1.5f,
		0.1f,
		17,
		Vector4(0, 0, 1, 0),
		point_num);
}
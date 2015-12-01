
#include "SoccerAttackClass.h"
#include "../CharacterFunction.h"
#include "SoccerHitEvent.h"
#include "../../GameSystem/ResourceManager.h"

SoccerAttackClass::SoccerAttackClass(
	SoccerPlayer*   pOwner,
	ControllEvent*  pEvent,
	int Frame
	) :
	m_pOwner(pOwner),
	m_pEvent(pEvent),
	m_Timer(0),
	m_ComboCount(-1),
	m_DoHit(false),
	m_pStateFunc(&SoccerAttackClass::State_NextAtk),
	m_Locus(7),
	NoDamageFrame(Frame)
{
	m_Damage.m_Enable = false;

	m_Locus.m_Division = 10;
	m_Locus.m_pTexture = DefResource.Get(Resource::TextureType::Locus1);

	m_Locus.m_StartParam.Color = Vector4(1, 1, 1, 1);
	//m_Locus.m_StartParam.HDRColor = Vector4(1, 1, 1, 0.2f);

	m_Locus.m_EndParam.Color = Vector4(1, 1, 1, 0);
	//m_Locus.m_EndParam.HDRColor = Vector4(1, 0, 1, 0);
}

SoccerAttackClass::~SoccerAttackClass()
{
	for (auto& it : m_AttackInfoArray)
	{
		delete it;
	}

	delete m_pEvent;
}

void SoccerAttackClass::Update()
{
	m_pOwner->m_Renderer.Update(1);
	DamageManager::HitEventBase NoDmgHitEvent;   //ノーダメージ
	SoccerHitEvent              UsualHitEvent(m_pOwner);//通常
	(this->*m_pStateFunc)();


	//キャラクタ更新
	{
		if (m_Timer < NoDamageFrame)
		{
			chr_func::UpdateAll(m_pOwner, &NoDmgHitEvent);
		}
		else
		{
			chr_func::UpdateAll(m_pOwner, &UsualHitEvent);
		}
	}

	chr_func::CreateTransMatrix(m_pOwner, m_pOwner->m_ModelSize, &m_pOwner->m_Renderer.m_TransMatrix);
}

bool SoccerAttackClass::isEnd()const
{
	return (m_pStateFunc == &SoccerAttackClass::State_End);
}

//----------------------------------------------------------------


void SoccerAttackClass::State_Attack()
{
	AttackInfo* const pNowAtk = m_AttackInfoArray.at(m_ComboCount);

	//カウント進行
	++m_Timer;

	//毎フレームの更新
	{
		pNowAtk->Update(m_Timer, &m_Locus);
		pNowAtk->DamagePosSet(&m_Damage, m_pOwner);
	}
	//攻撃ヒット関数の呼び出し
	if (m_DamageHitCount != m_Damage.HitCount)
	{
		m_DamageHitCount = m_Damage.HitCount;
		pNowAtk->HitAttack(&m_Damage);
		m_DoHit = true;

		//    m_HitStopCount = 5;
	}
	//角度更新
	RADIAN ControllRadian = 0;

	if (pNowAtk->isCanAngleControll(m_Timer, ControllRadian))
	{
		m_pEvent->AngleControll(ControllRadian);

		{
			Vector3 m = m_pOwner->m_Params.move;
			const float l = Vector3XZLength(m);

			chr_func::GetFront(m_pOwner, &m);
			m *= l;
			m_pOwner->m_Params.move = m;
		}
	}

	//コンボ実行フラグのチェック
	if (m_DoCombo == false &&
		pNowAtk->isComboButtonFrame(m_Timer))
	{
		m_DoCombo = m_pEvent->isDoCombo();

	}

	//コンボ移行
	if (!isLastAtk() &&
		m_DoCombo    &&
		pNowAtk->isComboSwitchFrame(m_Timer) &&
		m_DoHit == true)
	{
		m_DoHit = false;
		m_pStateFunc = &SoccerAttackClass::State_NextAtk;
	}

	//ダメージ有効・無効
	m_Damage.m_Enable = pNowAtk->isDamageEnable(m_Timer);

	//攻撃終了
	if (pNowAtk->isEnd(m_Timer))
	{
		m_pStateFunc = &SoccerAttackClass::State_End;
		m_DoHit = false;
	}
}


//次の攻撃に移行する
void SoccerAttackClass::State_NextAtk()
{
	MyAssert(!isLastAtk(), "攻撃情報クラスがない状態でTennisAttackClass::State_NextAtkが実行されました");

	AttackInfo* const pNowAtk = m_AttackInfoArray.at(++m_ComboCount);

	pNowAtk->DamageParamSet(&m_Damage);
	pNowAtk->DamagePosSet(&m_Damage, m_pOwner);
	pNowAtk->MotionSet(m_pOwner);

    m_Damage.ResetCounts();

	m_Timer = 0;
	m_DoCombo = false;

	m_pStateFunc = &SoccerAttackClass::State_Attack;
}

void SoccerAttackClass::State_End()
{

}
//最終段攻撃かどうか
bool SoccerAttackClass::isLastAtk()
{
	return ((int)m_AttackInfoArray.size() - 1) <= m_ComboCount;
}
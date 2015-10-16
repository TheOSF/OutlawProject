

#include "BaseballAttackClass.h"
#include "../CharacterFunction.h"
#include "Baseball_HitEvent.h"
#include "../../GameSystem/ResourceManager.h"

BaseballAttackClass::BaseballAttackClass(
	BaseballPlayer*   pOwner,
	ControllEvent*  pEvent
	) :
	m_pOwner(pOwner),
	m_pEvent(pEvent),
	m_Timer(0),
	m_ComboCount(-1),
	m_pStateFunc(&BaseballAttackClass::State_NextAtk),
	m_Locus(7)
{
	m_Damage.m_Enable = false;

	m_Locus.m_Division = 10;
	m_Locus.m_pTexture = DefResource.Get(Resource::TextureType::Locus1);

	m_Locus.m_StartParam.Color = Vector4(1, 1, 1, 1);
	m_Locus.m_StartParam.HDRColor = Vector4(1, 1, 1, 0.2f);

	m_Locus.m_EndParam.Color = Vector4(1, 1, 1, 0);
	m_Locus.m_EndParam.HDRColor = Vector4(1, 0, 1, 0);
}

BaseballAttackClass::~BaseballAttackClass()
{
	for (auto& it : m_AttackInfoArray)
	{
		delete it;
	}

	delete m_pEvent;
}

void BaseballAttackClass::Update()
{
	m_pOwner->m_Renderer.Update(1);

	(this->*m_pStateFunc)();


	//キャラクタ更新
	{
		BaseballHitEvent HitEvent(m_pOwner);

		chr_func::UpdateAll(m_pOwner, &HitEvent);
	}

	chr_func::CreateTransMatrix(m_pOwner, m_pOwner->m_ModelSize, &m_pOwner->m_Renderer.m_TransMatrix);
}

bool BaseballAttackClass::isEnd()const
{
	return (m_pStateFunc == &BaseballAttackClass::State_End);
}

//----------------------------------------------------------------


void BaseballAttackClass::State_Attack()
{
	AttackInfo* const pNowAtk = m_AttackInfoArray.at(m_ComboCount);

	//カウント進行
	++m_Timer;

	//毎フレームの更新
	{
		pNowAtk->Update(m_Timer, &m_Locus);
		pNowAtk->DamagePosSet(&m_Damage, m_pOwner);
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

	//☆変更予定点(必殺技時->当たったらにする)
	//コンボ移行
	if (!isLastAtk() &&
		m_DoCombo    &&
		pNowAtk->isComboSwitchFrame(m_Timer))
	{
		m_pStateFunc = &BaseballAttackClass::State_NextAtk;
	}

	//ダメージ有効・無効
	m_Damage.m_Enable = pNowAtk->isDamageEnable(m_Timer);

	//攻撃終了
	if (pNowAtk->isEnd(m_Timer))
	{
		m_pStateFunc = &BaseballAttackClass::State_End;
	}
}


//次の攻撃に移行する
void BaseballAttackClass::State_NextAtk()
{
	MyAssert(!isLastAtk(), "攻撃情報クラスがない状態でBaseballAttackClass::State_NextAtkが実行されました");

	AttackInfo* const pNowAtk = m_AttackInfoArray.at(++m_ComboCount);

	pNowAtk->DamageParamSet(&m_Damage);
	pNowAtk->DamagePosSet(&m_Damage, m_pOwner);
	pNowAtk->MotionSet(m_pOwner);

	m_Timer = 0;
	m_DoCombo = false;

	m_pStateFunc = &BaseballAttackClass::State_Attack;
}

void BaseballAttackClass::State_End()
{

}


//最終段攻撃かどうか
bool BaseballAttackClass::isLastAtk()
{
	return ((int)m_AttackInfoArray.size() - 1) <= m_ComboCount;
}
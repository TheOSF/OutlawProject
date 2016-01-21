#include "CharacterBase.h"
#include "CharacterShotAttackClass.h"
#include "CharacterFunction.h"
#include "../Effect/ParticleRenderer.h"
#include "../Effect/ParticleMoveObject.h"
#include "../GameSystem/ResourceManager.h"

#include "../Ball/Ball.h"
#include "../Effect/HitEffectObject.h"

//***************************************************
//		ラクロス用 遠距離攻撃 (ボール打ち出し) クラス
//***************************************************

// コンストラクタ
CharacterShotAttack::CharacterShotAttack(
	CharacterBase* pCharacter,
	Event* pEvent,
	const AttackParams& attack_params,
	HitEventBase*    pHitEventBase
	) :
	m_AttackParams(attack_params),
	m_StickValue(0, 0),
	m_pCharacter(pCharacter),
	m_Timer(0),
	m_pEvent(pEvent),
	m_pHitEventBase(pHitEventBase)
{

}


// デストラクタ
CharacterShotAttack::~CharacterShotAttack()
{
	delete m_pEvent;
	delete m_pHitEventBase;
}


// 更新
bool CharacterShotAttack::Update()
{
	if (m_Timer == 0)
	{// 攻撃開始
		m_pEvent->AttackStart();
	}

    if (m_Timer < m_AttackParams.ShotFrame)
    {
        CharacterBase* pTarget = nullptr;
        
        if (chr_func::CalcAtkTarget(m_pCharacter, D3DXToRadian(90), 60.0f, &pTarget))
        {
            // 向き補正
            chr_func::AngleControll(
                m_pCharacter,
                pTarget->m_Params.pos,
                D3DXToRadian(10)
                );
        }

        
    }

	// 減速
	chr_func::XZMoveDown(
		m_pCharacter,
		m_AttackParams.MoveDownSpeed);

	// 更新
	m_pEvent->Update();

	if (m_Timer == m_AttackParams.ShotFrame)
	{// ダメージ発生フレーム
		m_pEvent->Shot();
	}

	if (m_Timer >= m_AttackParams.AllFrame)
	{// 攻撃終了
		m_pEvent->AttackEnd();
		return false;
	}

	// タイマー更新
	m_Timer++;


    chr_func::UpdateAll(m_pCharacter, m_pHitEventBase);


	return true;
}


// スティックの値セット
void CharacterShotAttack::SetStickValue(CrVector2 stickValue)
{
	m_StickValue = stickValue;
}


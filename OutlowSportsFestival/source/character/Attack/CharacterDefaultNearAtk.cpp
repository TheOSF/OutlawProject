#include "CharacterDefaultNearAtk.h"
#include "../CharacterFunction.h"
#include "../CharacterBase.h"


//****************************************************************
//		キャラクタ共通の攻撃クラス
//****************************************************************

CharacterDefaultNearAttack::CharacterDefaultNearAttack(
    CharacterBase*	pParent,	//操るキャラクタのポインタ
    const Params&	param,
    AttackEvent*	pAtttackEvent) :
    m_pCharacter(pParent),
    m_timer(0),
    m_pAttackEvent(pAtttackEvent)
{
    m_Params = param;

    m_Damage.pBall = NULL;
    m_Damage.pParent = pParent;

}

CharacterDefaultNearAttack::~CharacterDefaultNearAttack()
{
    delete m_pAttackEvent;
}

void CharacterDefaultNearAttack::Update()
{
    if (m_timer == 0)
    {
        // 攻撃開始
        m_pAttackEvent->Start();

    }

    // 座標更新
    chr_func::PositionUpdate(m_pCharacter);

    // 更新
    m_pAttackEvent->Update(m_timer);

    {
        if (m_timer == m_Params.DamageStartFrame)
        {
            // ダメージ発生フレーム
            m_Damage.m_Enable = true;
            m_pAttackEvent->DamageStart();
        }

        if (m_timer > m_Params.DamageStartFrame&&
            m_timer < m_Params.DamageEndFrame)
        {
            // ダメージ発生中
            m_pAttackEvent->DamageUpdate();
        }

        if (m_timer == m_Params.DamageEndFrame)
        {
            // ダメージ消滅フレーム
            m_Damage.m_Enable = false;
            m_pAttackEvent->DamageEnd();
        }
    }

    //あたり判定位置の更新
    UpdateDamagePos();

    if (m_timer == m_Params.EndFrame)
    {// 攻撃終了
        m_pAttackEvent->End();
    }

    // タイマー更新
    m_timer = min(m_timer + 1, INT_MAX);
}

void CharacterDefaultNearAttack::UpdateDamagePos()
{
    Matrix R;

    D3DXMatrixRotationY(&R, m_pCharacter->m_Params.angle);
    
    m_Damage.m_Param.pos = m_pCharacter->m_Params.pos + Vector3MulMatrix3x3(m_Params.DamagePosOffset, R);
}

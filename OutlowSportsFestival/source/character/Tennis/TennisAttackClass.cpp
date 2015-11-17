
#include "TennisAttackClass.h"
#include "../CharacterFunction.h"
#include "Tennis_HitEvent.h"
#include "../../GameSystem/ResourceManager.h"

TennisAttackClass::TennisAttackClass(
    TennisPlayer*   pOwner,
    ControllEvent*  pEvent
    ) :
    m_pOwner(pOwner),
    m_pEvent(pEvent),
    m_Timer(0),
    m_ComboCount(-1),
    m_pStateFunc(&TennisAttackClass::State_NextAtk),
    m_Locus(6),
    m_DamageHitCount(0)
{
    m_Damage.m_Enable = false;

    m_Locus.m_Division = 5;
    //m_Locus.m_pTexture = DefResource.Get(Resource::TextureType::Locus1);

    Vector4 EffectColor = Vector4(1, 1, 1, 0.5f);// CharacterBase::GetPlayerColorF(m_pOwner->m_PlayerInfo.number).toVector4();
    EffectColor.w = 0.5f;

    m_Locus.m_StartParam.Color = EffectColor;
    m_Locus.m_StartParam.HDRColor = EffectColor;
    m_Locus.m_StartParam.Width = 0.8f;

    EffectColor.w = 0;

    m_Locus.m_EndParam.Color = EffectColor;
    m_Locus.m_EndParam.HDRColor = EffectColor;
    m_Locus.m_EndParam.Color.w = 0;
    m_Locus.m_EndParam.HDRColor.w = 0;
    m_Locus.m_EndParam.Width = 0.5f;
}

TennisAttackClass::~TennisAttackClass()
{
    for (auto& it : m_AttackInfoArray)
    {
        delete it;
    }

    delete m_pEvent;
}

void TennisAttackClass::Update()
{
    chr_func::CreateTransMatrix(m_pOwner, m_pOwner->m_ModelSize, &m_pOwner->m_Renderer.m_TransMatrix);

    (this->*m_pStateFunc)();

}

bool TennisAttackClass::isEnd()const
{
    return (m_pStateFunc == &TennisAttackClass::State_End);
}

//----------------------------------------------------------------


void TennisAttackClass::State_Attack()
{
    AttackInfo* const pNowAtk = m_AttackInfoArray.at(m_ComboCount);


    //コンボ実行フラグのチェック
    if (m_DoCombo == false &&
        pNowAtk->isComboButtonFrame(m_Timer))
    {
        m_DoCombo = m_pEvent->isDoCombo();
    }

    if (pNowAtk->isHitStopFrame())
    {
        TennisHitEvent HitEvent(m_pOwner);

        //壁との接触判定
        chr_func::CheckWall(m_pOwner);

        //床との接触判定
        chr_func::CheckGround(m_pOwner);

        //あたり判定を取る
        chr_func::DamageCheck(m_pOwner, &HitEvent);

        pNowAtk->HitStopUpdate();

        return;
    }

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

    //コンボ移行
    if (!isLastAtk() &&
        m_DoCombo    &&
        pNowAtk->isComboSwitchFrame(m_Timer))
    {
        m_pStateFunc = &TennisAttackClass::State_NextAtk;
    }

    //ダメージ有効・無効
    m_Damage.m_Enable = pNowAtk->isDamageEnable(m_Timer);

    //攻撃終了
    if (pNowAtk->isEnd(m_Timer))
    {
        m_pStateFunc = &TennisAttackClass::State_End;
    }


    //キャラクタ更新
    {
        TennisHitEvent HitEvent(m_pOwner);

        m_pOwner->m_Renderer.Update(1);

        chr_func::UpdateAll(m_pOwner, &HitEvent);
    }
}


//次の攻撃に移行する
void TennisAttackClass::State_NextAtk()
{
    MyAssert(!isLastAtk(), "攻撃情報クラスがない状態でTennisAttackClass::State_NextAtkが実行されました");

    AttackInfo* const pNowAtk = m_AttackInfoArray.at(++m_ComboCount);

    m_Damage.m_Enable = false;

    pNowAtk->DamageParamSet(&m_Damage);
    pNowAtk->DamagePosSet(&m_Damage, m_pOwner);
    pNowAtk->MotionSet(m_pOwner);

    m_Timer = 0;
    m_DoCombo = false;
    m_DamageHitCount = 0;

    m_pStateFunc = &TennisAttackClass::State_Attack;
}

void TennisAttackClass::State_End()
{

}


//最終段攻撃かどうか
bool TennisAttackClass::isLastAtk()
{
    return ((int)m_AttackInfoArray.size() - 1) <= m_ComboCount;
}
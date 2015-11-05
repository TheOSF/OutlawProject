#include "TennisAttackInfo_UsualAtk.h"
#include "../CharacterFunction.h"
#include "../../Sound/Sound.h"

#include "../../GameSystem/GameController.h"

TennisAttackInfo_UsualAtk::TennisAttackInfo_UsualAtk(
    TennisPlayer* pOwner
    ):
    m_pOwner(pOwner)
{
    
}

TennisAttackInfo_UsualAtk::~TennisAttackInfo_UsualAtk()
{

}

//ダメージパラメータをセット
void TennisAttackInfo_UsualAtk::DamageParamSet(DamageShpere* pDmg)
{
    pDmg->HitCount = 0;
    pDmg->m_Param.size = m_Param.DamageSize;
    pDmg->pBall = nullptr;
    pDmg->pParent = m_pOwner;
    pDmg->type = m_Param.DamageType;
    pDmg->Value = m_Param.DamageValue;

    DamagePosSet(pDmg,m_pOwner);
}


//ダメージ位置をセット
void TennisAttackInfo_UsualAtk::DamagePosSet(DamageShpere* pDmg, TennisPlayer* pTennis)
{
    Matrix  BoneMat;
    Vector3 Forward;
    Vector3 Pos1, Pos2;

    m_pOwner->m_Renderer.GetWorldBoneMatirx(BoneMat, m_Param.LocusBoneNum1);

    Pos1 = Vector3(BoneMat._41, BoneMat._42, BoneMat._43);

    m_pOwner->m_Renderer.GetWorldBoneMatirx(BoneMat, m_Param.LocusBoneNum2);

    Pos2 = Vector3(BoneMat._41, BoneMat._42, BoneMat._43);

    pDmg->m_Param.pos = Pos2;

    chr_func::GetFront(pTennis, &pDmg->vec);
    pDmg->vec *= 0.5f;

    if (m_Param.DamageType == DamageBase::Type::_VanishDamage)
    {
        pDmg->vec.y = 0.2f;
    }

    m_LocusPos = (Pos2 + Pos1)*0.5f;
    m_LocusVec = Vector3Normalize(Pos2 - Pos1);
}


//モーションをセット
void TennisAttackInfo_UsualAtk::MotionSet(TennisPlayer* pTennis)
{
    pTennis->m_Renderer.SetMotion(m_Param.Motion);
}


//引数のフレームが攻撃終了かどうか
bool TennisAttackInfo_UsualAtk::isEnd(int Frame)
{
    return Frame >= m_Param.AllFrame;
}


//引数のフレームがコンボ移行フレームかどうか
bool TennisAttackInfo_UsualAtk::isComboSwitchFrame(int Frame)
{
    return
        Frame >= m_Param.ComboSwitchStartFrame&&
        Frame <= m_Param.ComboSwitchEndFrame;
}


//引数のフレームが、コンボの移行判定ボタンを押せるフレームかどうか
bool TennisAttackInfo_UsualAtk::isComboButtonFrame(int Frame)
{
    return Frame >= m_Param.CanComboButtonPushFrame;
}


//引数のフレームが、角度調整を行えるフレームかどうか
bool TennisAttackInfo_UsualAtk::isCanAngleControll(int Frame, RADIAN& OutControllVal)
{
    OutControllVal = m_Param.AngleControllValue;

    if (Frame < 3)
    {
        OutControllVal = D3DXToRadian(45);
    }

    return
        Frame >= m_Param.AngleControllStartFrame&&
        Frame <= m_Param.AngleControllEndFrame;
}


//引数のフレームが、ダメージ判定有効フレームかどうか
bool TennisAttackInfo_UsualAtk::isDamageEnable(int Frame)
{
    if (Frame == m_Param.DamageEnableStart)
    {
        Sound::Play(Sound::Swing2);
    }

    return 
        Frame >= m_Param.DamageEnableStart&&
        Frame <= m_Param.DamageEnableEnd;
}


//毎フレーム呼ばれる
void TennisAttackInfo_UsualAtk::Update(int Frame, LocusHDR* pLocus)
{
    pLocus->AddPoint(m_LocusPos, m_LocusVec);

    chr_func::XZMoveDown(m_pOwner, 0.1f);

    if (Frame >= m_Param.MoveStartFrame&&
        Frame <= m_Param.MoveEndFrame)
    {
        chr_func::AddMoveFront(m_pOwner, m_Param.MoveSpeed, 1000);
    }
}

//攻撃があたったときに呼ばれる
void TennisAttackInfo_UsualAtk::HitAttack(DamageShpere* pDmg)
{
    chr_func::AddSkillGauge(m_pOwner, pDmg->Value*10.0f);

    //コントローラを振動
    controller::SetVibration(
        5000,
        0.15f,
        m_pOwner->m_PlayerInfo.number
        );
}
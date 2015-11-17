#include "TennisAttackInfo_UsualAtk.h"
#include "../CharacterFunction.h"
#include "../../Sound/Sound.h"
#include "../CharacterManager.h"

#include "../../GameSystem/GameController.h"

TennisAttackInfo_UsualAtk::TennisAttackInfo_UsualAtk(
    TennisPlayer* pOwner
    ) :
    m_pOwner(pOwner),
    m_HitStopCount(0),
    m_LocusPos(Vector3Zero),
    m_LocusVec(Vector3Zero),
    m_Hit(false)
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


    //ボーン位置を算出
    {
        m_pOwner->m_Renderer.GetWorldBoneMatirx(BoneMat, m_Param.LocusBoneNum1);

        Pos1 = Vector3(BoneMat._41, BoneMat._42, BoneMat._43);

        m_pOwner->m_Renderer.GetWorldBoneMatirx(BoneMat, m_Param.LocusBoneNum2);

        Pos2 = Vector3(BoneMat._41, BoneMat._42, BoneMat._43);
    }

    //方向はキャラクタの正面ベクトル
    chr_func::GetFront(pTennis, &pDmg->m_Vec);
    pDmg->m_VecPower.x = 0.1f;
    
    //pDmg->m_Param.pos = Pos2;  //ボーン先端位置にダメージをセット

    //キャラクタの少し前にダメージをセット

    pDmg->m_Param.pos = m_pOwner->m_Params.pos + pDmg->m_Vec* m_Param.DamagePosLength + Vector3(0, 2, 0);

    if (m_Param.DamageType == DamageBase::Type::_VanishDamage)
    {
        pDmg->m_VecPower.x = 0.5f;
        pDmg->m_VecPower.y = 0.5f;
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
        (Frame >= m_Param.DamageEnableStart&&
        Frame <= m_Param.DamageEnableEnd) &&
        m_Hit == false;
        
}


//毎フレーム呼ばれる
void TennisAttackInfo_UsualAtk::Update(int Frame, LocusHDR* pLocus)
{
    pLocus->AddPoint(m_LocusPos, m_LocusVec);

    chr_func::XZMoveDown(m_pOwner, 0.1f);


    if (Frame >= m_Param.MoveStartFrame&&
        Frame <= m_Param.MoveEndFrame)
    {
        if (isFrontStayEnemy() && (m_Param.DamageType != DamageBase::Type::_VanishDamage))
        {
            
        }
        else
        {
            chr_func::AddMoveFront(m_pOwner, m_Param.MoveSpeed, 1000);
        }
    }
}

//ヒットストップ時
void TennisAttackInfo_UsualAtk::HitStopUpdate()
{

    m_HitStopCount = max(0, m_HitStopCount - 1);
}

//攻撃があたったときに呼ばれる
void TennisAttackInfo_UsualAtk::HitAttack(DamageShpere* pDmg)
{
    chr_func::AddSkillGauge(m_pOwner, pDmg->Value);

    //コントローラを振動
    controller::SetVibration(
        5000,
        0.15f,
        m_pOwner->m_PlayerInfo.number
        );

    m_HitStopCount = m_Param.HitStopFrame;

    m_Hit = true;
}


//前方に敵がいるかどうか
bool TennisAttackInfo_UsualAtk::isFrontStayEnemy()
{   
    const CharacterManager::CharacterMap& chr_map = DefCharacterMgr.GetCharacterMap();
    Vector3 front, vec;

    const float OKlen = 3.0f;
    const RADIAN OKrad = D3DXToRadian(45);

    chr_func::GetFront(m_pOwner, &front);

    for (auto& it : chr_map)
    {
        if (chr_func::isDie(it.first) ||
            it.first == m_pOwner)
        {
            continue;
        }


        vec = it.first->m_Params.pos - m_pOwner->m_Params.pos;

        if (vec.Length() < OKlen && Vector3Radian(vec, front) < OKrad)
        {
            return true;
        }
    }

    return false;;
}


//ヒットストップかどうか
bool TennisAttackInfo_UsualAtk::isHitStopFrame()
{
    return m_HitStopCount > 0;
}
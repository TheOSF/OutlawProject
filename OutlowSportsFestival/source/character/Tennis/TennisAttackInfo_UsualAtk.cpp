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

//�_���[�W�p�����[�^���Z�b�g
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


//�_���[�W�ʒu���Z�b�g
void TennisAttackInfo_UsualAtk::DamagePosSet(DamageShpere* pDmg, TennisPlayer* pTennis)
{
    Matrix  BoneMat;
    Vector3 Forward;
    Vector3 Pos1, Pos2;


    //�{�[���ʒu���Z�o
    {
        m_pOwner->m_Renderer.GetWorldBoneMatirx(BoneMat, m_Param.LocusBoneNum1);

        Pos1 = Vector3(BoneMat._41, BoneMat._42, BoneMat._43);

        m_pOwner->m_Renderer.GetWorldBoneMatirx(BoneMat, m_Param.LocusBoneNum2);

        Pos2 = Vector3(BoneMat._41, BoneMat._42, BoneMat._43);
    }

    //�����̓L�����N�^�̐��ʃx�N�g��
    chr_func::GetFront(pTennis, &pDmg->m_Vec);
    pDmg->m_VecPower.x = 0.1f;
    
    //pDmg->m_Param.pos = Pos2;  //�{�[����[�ʒu�Ƀ_���[�W���Z�b�g

    //�L�����N�^�̏����O�Ƀ_���[�W���Z�b�g

    pDmg->m_Param.pos = m_pOwner->m_Params.pos + pDmg->m_Vec* m_Param.DamagePosLength + Vector3(0, 2, 0);

    if (m_Param.DamageType == DamageBase::Type::_VanishDamage)
    {
        pDmg->m_VecPower.x = 0.5f;
        pDmg->m_VecPower.y = 0.5f;
    }

    m_LocusPos = (Pos2 + Pos1)*0.5f;
    m_LocusVec = Vector3Normalize(Pos2 - Pos1);
}


//���[�V�������Z�b�g
void TennisAttackInfo_UsualAtk::MotionSet(TennisPlayer* pTennis)
{
    pTennis->m_Renderer.SetMotion(m_Param.Motion);
}


//�����̃t���[�����U���I�����ǂ���
bool TennisAttackInfo_UsualAtk::isEnd(int Frame)
{
    return Frame >= m_Param.AllFrame;
}


//�����̃t���[�����R���{�ڍs�t���[�����ǂ���
bool TennisAttackInfo_UsualAtk::isComboSwitchFrame(int Frame)
{
    return
        Frame >= m_Param.ComboSwitchStartFrame&&
        Frame <= m_Param.ComboSwitchEndFrame;
}


//�����̃t���[�����A�R���{�̈ڍs����{�^����������t���[�����ǂ���
bool TennisAttackInfo_UsualAtk::isComboButtonFrame(int Frame)
{
    return Frame >= m_Param.CanComboButtonPushFrame;
}


//�����̃t���[�����A�p�x�������s����t���[�����ǂ���
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


//�����̃t���[�����A�_���[�W����L���t���[�����ǂ���
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


//���t���[���Ă΂��
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

//�q�b�g�X�g�b�v��
void TennisAttackInfo_UsualAtk::HitStopUpdate()
{

    m_HitStopCount = max(0, m_HitStopCount - 1);
}

//�U�������������Ƃ��ɌĂ΂��
void TennisAttackInfo_UsualAtk::HitAttack(DamageShpere* pDmg)
{
    chr_func::AddSkillGauge(m_pOwner, pDmg->Value);

    //�R���g���[����U��
    controller::SetVibration(
        5000,
        0.15f,
        m_pOwner->m_PlayerInfo.number
        );

    m_HitStopCount = m_Param.HitStopFrame;

    m_Hit = true;
}


//�O���ɓG�����邩�ǂ���
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


//�q�b�g�X�g�b�v���ǂ���
bool TennisAttackInfo_UsualAtk::isHitStopFrame()
{
    return m_HitStopCount > 0;
}
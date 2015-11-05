#include "TennisPlayerState_Attack.h"
#include "TennisPlayerState_UsualMove.h"
#include "../../GameSystem/GameController.h"
#include "../CharacterFunction.h"
#include "../CharacterManager.h"
#include "TennisAttackInfo_UsualAtk.h"
#include "../../Camera/Camera.h"

//------------�v���C���[����̍U������N���X--------------

TennisState_PlayerControll_Attack::PlayerControllEvent::PlayerControllEvent(TennisPlayer*const pTennis):
m_pTennis(pTennis)
{

}

bool TennisState_PlayerControll_Attack::PlayerControllEvent::isDoCombo()
{
    return controller::GetTRG(controller::button::shikaku, m_pTennis->m_PlayerInfo.number);
}

void TennisState_PlayerControll_Attack::PlayerControllEvent::AngleControll(RADIAN angle)
{
    const CharacterBase* const pTargetCharacter = GetFrontTargetEnemy();

    if (pTargetCharacter != nullptr)
    {
        //������]
        chr_func::AngleControll(m_pTennis, pTargetCharacter->m_Params.pos, angle);
    }
    else
    {
        const Vector2 Stick = controller::GetStickValue(controller::stick::left, m_pTennis->m_PlayerInfo.number);

        //�X�e�B�b�N�����ȏ�|����Ă��邩�ǂ���
        if (Vector2Length(Stick) > 0.25f)
        {
            Vector3 Vec;

            //�X�e�B�b�N�l���J������Ԃ�
            Vec = DefCamera.GetForward()*Stick.y + DefCamera.GetRight()*Stick.x;

            //�L�����N�^��]
            chr_func::AngleControll(m_pTennis, m_pTennis->m_Params.pos + Vec, angle);
        }
    } 
}

const CharacterBase* TennisState_PlayerControll_Attack::PlayerControllEvent::GetFrontTargetEnemy()
{
    CharacterManager::CharacterMap ChrMap = DefCharacterMgr.GetCharacterMap();

    const float  AutoDistance = 10.0f;               //�������ł���ő勗��
    const RADIAN AutoMaxAngle = D3DXToRadian(90);   //�������ł���ő�p�x

    const CharacterBase* pTargetEnemy = nullptr;    //�^�[�Q�b�g�ێ��̃|�C���^
    RADIAN MostMinAngle = PI;                       //�����Ƃ������p�x
    RADIAN TempAngle;

    Vector3 MyFront;      //���g�̑O���x�N�g��
    chr_func::GetFront(m_pTennis, &MyFront);

    auto it = ChrMap.begin();

    while (it != ChrMap.end())
    {
        //���g�����O
        if (m_pTennis->m_PlayerInfo.number == it->first->m_PlayerInfo.number||
            chr_func::isDie(it->first)
            )
        {
            ++it;
            continue;
        }

        //���������ȏ�̃L�����N�^�����O����
        if (Vector3Distance(it->first->m_Params.pos, m_pTennis->m_Params.pos) > AutoDistance)
        {
            it = ChrMap.erase(it);
            continue;
        }

        //�O�x�N�g���ƓG�ւ̃x�N�g���̊p�x���v�Z����
        TempAngle = Vector3Radian(MyFront, (it->first->m_Params.pos - m_pTennis->m_Params.pos));

        //�p�x����ԋ���������X�V
        if (TempAngle < MostMinAngle)
        {
            pTargetEnemy = it->first;
            MostMinAngle = TempAngle;
        }

        ++it;
    }
    
    return pTargetEnemy;

}

//-------------�ߋ����U���X�e�[�g�N���X-------------

TennisState_PlayerControll_Attack::TennisState_PlayerControll_Attack(TennisPlayer* t) :
m_Attack(t, new PlayerControllEvent(t))
{

}


TennisState_PlayerControll_Attack::~TennisState_PlayerControll_Attack()
{

}

// �X�e�[�g�J�n
void TennisState_PlayerControll_Attack::Enter(TennisPlayer* t)
{
    //�U���N���X�쐬
    TennisAttackInfo_UsualAtk* pAtk;

    TennisAttackInfo_UsualAtk::Param AtkParam[] = 
    {
        { 6, 1.0f, 1.5f, DamageBase::Type::_WeekDamage,   15, 22, 0.11f , 8,12, TennisPlayer::_mt_Attack1, 35, 20, 25, 32, 0, 3,  D3DXToRadian(45), 42,43 },
        { 2, 1.0f, 1.5f, DamageBase::Type::_WeekDamage, 5, 8, 0.02f, 1, 5, TennisPlayer::_mt_Attack2, 20, 5, 15, 20, 0, 5, D3DXToRadian(8), 42, 43 },
        { 8, 1.0f, 1.5f, DamageBase::Type::_VanishDamage, 8, 16, 0.05f, 1, 6, TennisPlayer::_mt_Attack3, 40, -1, -1, -1, 0, 8, D3DXToRadian(8), 42, 43 },
    };

    for (int i = 0; i < (int)ARRAYSIZE(AtkParam); ++i)
    {
        pAtk = new TennisAttackInfo_UsualAtk(t);

        pAtk->m_Param = AtkParam[i];

        m_Attack.m_AttackInfoArray.push_back(pAtk);
    }
}


// �X�e�[�g���s
void TennisState_PlayerControll_Attack::Execute(TennisPlayer* t)
{
    m_Attack.Update();

    if (m_Attack.isEnd())
    {
        t->SetState(TennisState_PlayerControll_Move::GetPlayerControllMove(t));
    }
}

// �X�e�[�g�I��
void TennisState_PlayerControll_Attack::Exit(TennisPlayer* t)
{

}
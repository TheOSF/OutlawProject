#include "CharacterHitEventFunc.h"
#include "CharacterFunction.h"


CharacterHitEventFunc::SetType CharacterHitEventFunc::CheckDamage(DamageBase* pDmg, CharacterBase* pOwner, Vector3* pOutVec)
{

    //�����̍���Ă���_���[�W�������ꍇ�͉������Ȃ�
    if (pDmg->pParent != nullptr &&
        pDmg->pParent->m_PlayerInfo.number == pOwner->m_PlayerInfo.number || 
        pDmg->isCanHitCharacter(pOwner) == false
        )
    {
        return SetType::_None;
    }

    //�_���[�W�̃p�����[�^
    Vector3 dmg_pos, dmg_vec;

    //�_���[�W�����ƈʒu���Z�o
    pDmg->CalcPosVec(pOwner->CalcHitCheckPos(), &dmg_pos, pOutVec);

    //�q�b�g�J�E���g���Z
    pDmg->AddHitCount(pOwner);

    //�_���[�W�v�Z
    chr_func::CalcDamage(pOwner, pDmg->Value, pDmg->isOptionOn(DamageBase::Option::_DontDie));


    //�����̗͂��Ȃ�������A�ǂ�ȍU���ł��낤�Ǝ��S�X�e�[�g��
    if (chr_func::isDie(pOwner))
    {
        return SetType::Die;
    }


    //�����������ɂ��̃_���[�W�̎�ނ���A���ꂼ��̃X�e�[�g�ɔh��������
    switch (pDmg->type)
    {
    case DamageBase::Type::_WeekDamage:

        //�n�ʂɕt���Ă����ꍇ�̂�
        if (chr_func::isTouchGround(pOwner))
        {
            //��Ђ�݂͏�Ɉړ����Ȃ�
            pOutVec->y = 0;
            //��_���[�W
            return SetType::Weak_Hit;
        }

        pOutVec->y = 0;

        //���쐬
    case DamageBase::Type::_VanishDamage:
        //������у_���[�W
        return SetType::Vanish_Hit;

    case DamageBase::Type::_ControllDamage:
        //������уR���g���[���_���[�W
        return SetType::Controll_Hit;

    default:

        break;
    }

    return SetType::_None;
}

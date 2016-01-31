#pragma once

#include "../Damage/Damage.h"
#include "CharacterBase.h"


//************************************************************
//	�L�����N�^�@�_���[�W�̂Ђ�ݕ���֐�
//************************************************************

class CharacterHitEventFunc
{
public:

    enum class SetType
    {
        Die,      //���S�X�e�[�g
        Weak_Hit, //��Ђ�݃X�e�[�g
        Vanish_Hit,//������уX�e�[�g
        Controll_Hit,//������уR���g���[���X�e�[�g

        _None, //�������Ȃ��Ă悢
    };

    //�_���[�W�l * m_DamageAddSkillValue���X�L���Q�[�W�����Z����
    static const float m_DamageAddSkillValue;

    //�߂�l�F�Ђ�ރ^�C�v
    static SetType CheckDamage(
        DamageBase*    pDmg,   //���������_���[�W�ւ̃|�C���^
        CharacterBase* pOwner, //���g
        Vector3*       pOutVec //�߂�l�F�Ђ�ޕ���
        );
};

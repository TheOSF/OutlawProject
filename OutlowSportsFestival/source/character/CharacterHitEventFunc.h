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

        _None, //�������Ȃ��Ă悢
    };

    //�߂�l�F�Ђ�ރ^�C�v
    static SetType CheckDamage(
        DamageBase*    pDmg,   //���������_���[�W�ւ̃|�C���^
        CharacterBase* pOwner, //���g
        Vector3*       pOutVec //�߂�l�F�Ђ�ޕ���
        );
};

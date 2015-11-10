#include "SoccerHitEvent.h"
#include "SoccerCommonState.h"
#include "../CharacterFunction.h"

SoccerHitEvent::SoccerHitEvent(SoccerPlayer* ps) :
m_pSoccer(ps)
{

}

bool SoccerHitEvent::Hit(DamageBase* pDmg)
{

	//�����̍���Ă���_���[�W�������ꍇ�͉������Ȃ�
	if (pDmg->pParent != nullptr &&
		pDmg->pParent->m_PlayerInfo.number == m_pSoccer->m_PlayerInfo.number)
	{
		return false;
	}

    //�_���[�W�v�Z
    chr_func::CalcDamage(m_pSoccer, pDmg->Value);


    //�_���[�W�̃p�����[�^
    Vector3 dmg_pos, dmg_vec;

    //�_���[�W�����ƈʒu���Z�o
    pDmg->CalcPosVec(m_pSoccer->CalcHitCheckPos(), &dmg_pos, &dmg_vec);



	//�����̗͂��Ȃ�������A�ǂ�ȍU���ł��낤�Ǝ��S�X�e�[�g��
	if (chr_func::isDie(m_pSoccer))
	{
        m_pSoccer->SetState(new SoccerState_DamageMotion_Die(m_pSoccer, dmg_vec));
		return true;
	}


	//�����������ɂ��̃_���[�W�̎�ނ���A���ꂼ��̃X�e�[�g�ɔh��������
	switch (pDmg->type)
	{
	case DamageBase::Type::_WeekDamage:
		//��U��
        m_pSoccer->SetState(new SoccerState_SmallDamage(m_pSoccer, dmg_vec));
		return true;

		//���쐬
	case DamageBase::Type::_VanishDamage:
		//������у_���[�W
        m_pSoccer->SetState(new SoccerState_DamageVanish(m_pSoccer, dmg_vec));
		return true;
		/*
		case DamageBase::Type::_UpDamage:
		//��ɐ������
		m_pTennis->SetState(new TennisState_(m_pTennis));
		return true;
		*/

	default:break;
	}

	return false;
}

#include "Tennis_HitEvent.h"
#include "TennisPlayerState_DamageMotionWeak.h"
#include "TennisPlayerState_Vanish.h"
#include "TennisPlayerState_DamageMotionDie.h"
#include "../CharacterFunction.h"



TennisHitEvent::TennisHitEvent(TennisPlayer* pt) :
m_pTennis(pt)
{

}

bool TennisHitEvent::Hit(DamageBase* pDmg)
{

	//�����̍���Ă���_���[�W�������ꍇ�͉������Ȃ�
    if (pDmg->pParent != nullptr &&
        pDmg->pParent->m_PlayerInfo.number == m_pTennis->m_PlayerInfo.number)
	{
		return false;
	}

    //�_���[�W�v�Z
    chr_func::CalcDamage(m_pTennis, pDmg->Value);


    //�����̗͂��Ȃ�������A�ǂ�ȍU���ł��낤�Ǝ��S�X�e�[�g��
    if (chr_func::isDie(m_pTennis))
    {
        m_pTennis->SetState(new TennisState_DamageMotion_Die(m_pTennis, pDmg->vec), true);
        return true;
    }


	//�����������ɂ��̃_���[�W�̎�ނ���A���ꂼ��̃X�e�[�g�ɔh��������
	switch (pDmg->type)
	{
	case DamageBase::Type::_WeekDamage:
		//��U��
        m_pTennis->SetState(new TennisState_DamageMotion_Weak(m_pTennis, pDmg->vec), true);
		return true;
	
	//���쐬
	case DamageBase::Type::_VanishDamage:
		//������у_���[�W
        m_pTennis->SetState(new TennisState_DamageVanish(m_pTennis, pDmg->vec), true);
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

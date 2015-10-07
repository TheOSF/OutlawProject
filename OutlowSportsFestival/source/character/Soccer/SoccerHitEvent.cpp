#include "SoccerHitEvent.h"
#include "SoccerCommonState.h"
//#include "TennisPlayerState_Vanish.h"

SoccerHitEvent::SoccerHitEvent(SoccerPlayer* ps) :
m_pSoccer(ps)
{

}

bool SoccerHitEvent::Hit(DamageBase* pDmg)
{

	//�����̍���Ă���_���[�W�������ꍇ�͉������Ȃ�
	if (pDmg->pParent->m_PlayerInfo.number == m_pSoccer->m_PlayerInfo.number)
	{
		return false;
	}

	//�����������ɂ��̃_���[�W�̎�ނ���A���ꂼ��̃X�e�[�g�ɔh��������
	switch (pDmg->type)
	{
	case DamageBase::Type::_WeekDamage:
		//��U��
		m_pSoccer->SetState(new SoccerState_SmallDamage(m_pSoccer, pDmg->vec));
		return true;

		//���쐬
	case DamageBase::Type::_VanishDamage:
		//������у_���[�W
		m_pSoccer->SetState(new SoccerState_DamageVanish(m_pSoccer, pDmg->vec));
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
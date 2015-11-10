#include "Baseball_HitEvent.h"
#include "BaseballplayerState_DamageMotionWeak.h"
#include "BaseballPlayerState_DamageMotion_Vanish.h"
#include "BaseballPlayerState_DamageMotion_Die.h"
#include "../CharacterFunction.h"


BaseballHitEvent::BaseballHitEvent(BaseballPlayer* pt) :
m_pBaseball(pt)
{

}

bool BaseballHitEvent::Hit(DamageBase* pDmg)
{

	//�����̍���Ă���_���[�W�������ꍇ�͉������Ȃ�
	if (pDmg->pParent != nullptr &&
		pDmg->pParent->m_PlayerInfo.number == m_pBaseball->m_PlayerInfo.number)
	{
		return false;
	}

    //�_���[�W�v�Z
    chr_func::CalcDamage(m_pBaseball, pDmg->Value);

	//�����̗͂��Ȃ�������A�ǂ�ȍU���ł��낤�Ǝ��S�X�e�[�g��
	if (chr_func::isDie(m_pBaseball))
	{
		m_pBaseball->SetState(new BaseballState_DamageMotion_Die(m_pBaseball, pDmg->vec), true);
		return true;
	}

	//�����������ɂ��̃_���[�W�̎�ނ���A���ꂼ��̃X�e�[�g�ɔh��������
	switch (pDmg->type)
	{
	case DamageBase::Type::_WeekDamage:
		//��U��
		m_pBaseball->SetState(new BaseballState_DamageMotion_Weak(m_pBaseball, pDmg->vec), true);
		return true;

		//�������
		case DamageBase::Type::_VanishDamage:
			m_pBaseball->SetState(new BaseballState_DamageVanish(m_pBaseball, pDmg->vec), true);
		return true;
		case DamageBase::Type::_UpDamage:
		////��ɐ������
		//m_pTennis->SetState(new TennisState_(m_pTennis));
		//return true;
		

	default:break;
	}

	return false;
}


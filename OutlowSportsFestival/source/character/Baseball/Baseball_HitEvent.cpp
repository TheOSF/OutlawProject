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
	CalcDamage(pDmg);


	//�����̗͂��Ȃ�������A�ǂ�ȍU���ł��낤�Ǝ��S�X�e�[�g��
	if (chr_func::isDie(m_pBaseball))
	{
		m_pBaseball->SetState(new BaseballState_DamageMotion_Die(m_pBaseball, pDmg->vec));
		return true;
	}

	//�����������ɂ��̃_���[�W�̎�ނ���A���ꂼ��̃X�e�[�g�ɔh��������
	switch (pDmg->type)
	{
	case DamageBase::Type::_WeekDamage:
		//��U��
		m_pBaseball->SetState(new BaseballState_DamageMotion_Weak(m_pBaseball, pDmg->vec));
		return true;

		//�������
		case DamageBase::Type::_VanishDamage:
			m_pBaseball->SetState(new BaseballState_DamageVanish(m_pBaseball, pDmg->vec));
		return true;
		case DamageBase::Type::_UpDamage:
		////��ɐ������
		//m_pTennis->SetState(new TennisState_(m_pTennis));
		//return true;
		

	default:break;
	}

	return false;
}

//�_���[�W�v�Z
void BaseballHitEvent::CalcDamage(DamageBase* pDmg)
{
	m_pBaseball->m_Params.HP -= pDmg->Value;
	m_pBaseball->m_Params.HP = max(m_pBaseball->m_Params.HP, 0);
}
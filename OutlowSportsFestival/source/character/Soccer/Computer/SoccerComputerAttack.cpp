#include "SoccerComputerAttack.h"
#include "../../CharacterFunction.h"
#include "../../CharacterManager.h"
#include "../SoccerPlayerState.h"

//------------�R���s���[�^����̍U������N���X--------------

SoccerState_ComputerControll_Attack::ComputerControllEvent::ComputerControllEvent(SoccerPlayer*const pSoccer) :
	m_pSoccer(pSoccer)
{

}
bool SoccerState_ComputerControll_Attack::ComputerControllEvent::isDoCombo()
{
	
	if (rand() % 100 > 60)
	{
		return true;
	}
	else
	{
		return false;
	}
}
void SoccerState_ComputerControll_Attack::ComputerControllEvent::AngleControll(RADIAN angle)
{
	const CharacterBase* const pTargetCharacter = GetFrontTargetEnemy();

	
	//������]
	chr_func::AngleControll(m_pSoccer, pTargetCharacter->m_Params.pos, angle);

}
const CharacterBase* SoccerState_ComputerControll_Attack::ComputerControllEvent::GetFrontTargetEnemy()
{
	CharacterManager::CharacterMap ChrMap = DefCharacterMgr.GetCharacterMap();

	const float  AutoDistance = 10.0f;               //�������ł���ő勗��
	const RADIAN AutoMaxAngle = D3DXToRadian(90);   //�������ł���ő�p�x

	const CharacterBase* pTargetEnemy = nullptr;    //�^�[�Q�b�g�ێ��̃|�C���^
	RADIAN MostMinAngle = PI;                       //�����Ƃ������p�x
	RADIAN TempAngle;

	Vector3 MyFront;      //���g�̑O���x�N�g��
	chr_func::GetFront(m_pSoccer, &MyFront);

	auto it = ChrMap.begin();

	while (it != ChrMap.end())
	{
		//���g�����O
		if (m_pSoccer->m_PlayerInfo.number == it->first->m_PlayerInfo.number ||
			chr_func::isDie(it->first)
			)
		{
			++it;
			continue;
		}

		//���������ȏ�̃L�����N�^�����O����
		if (Vector3Distance(it->first->m_Params.pos, m_pSoccer->m_Params.pos) > AutoDistance)
		{
			it = ChrMap.erase(it);
			continue;
		}

		//�O�x�N�g���ƓG�ւ̃x�N�g���̊p�x���v�Z����
		TempAngle = Vector3Radian(MyFront, (it->first->m_Params.pos - m_pSoccer->m_Params.pos));

		//�p�x����ԋ���������X�V
		if (TempAngle < MostMinAngle)
		{
			pTargetEnemy = it->first;
			MostMinAngle = TempAngle;
		}

		++it;
	}
	chr_func::AngleControll(m_pSoccer, pTargetEnemy->m_Params.pos,0.7f);
	return pTargetEnemy;

}
SoccerState_ComputerControll_Attack::SoccerState_ComputerControll_Attack(SoccerPlayer* s) :
	m_Attack(s, new ComputerControllEvent(s), 0)
{

}
SoccerState_ComputerControll_Attack::~SoccerState_ComputerControll_Attack()
{

}
void SoccerState_ComputerControll_Attack::Enter(SoccerPlayer* s)
{
	//�U���N���X�쐬
	SoccerAttackInfo_UsualAtk* pAtk;

	SoccerAttackInfo_UsualAtk::Param AtkParam[] =
	{
		{ 6, 1.0f, 1.5f, DamageBase::Type::_WeekDamage, 3, 20, 0.07f, 5, 10, SoccerPlayer::_ms_Atk1, 50, 20, 27, 35, 0, 5, D3DXToRadian(5), 21 },
		{ 2, 1.0f, 1.5f, DamageBase::Type::_WeekDamage, 5, 18, 0.02f, 1, 5, SoccerPlayer::_ms_Atk2, 35, 5, 15, 20, 0, 5, D3DXToRadian(5), 3 },
		{ 8, 1.0f, 1.5f, DamageBase::Type::_VanishDamage, 8, 16, 0.05f, 1, 6, SoccerPlayer::_ms_Atk3, 40, -1, -1, -1, 0, 8, D3DXToRadian(5), 21 },
	};

	for (int i = 0; i < (int)ARRAYSIZE(AtkParam); ++i)
	{
		pAtk = new SoccerAttackInfo_UsualAtk(s);

		pAtk->m_Param = AtkParam[i];

		m_Attack.m_AttackInfoArray.push_back(pAtk);
	}
}
void SoccerState_ComputerControll_Attack::Execute(SoccerPlayer* s)
{
	m_Attack.Update();

	if (m_Attack.isEnd())
	{
		s->SetState(SoccerState_PlayerControll_Move::GetPlayerControllMove(s));
	}
}
void SoccerState_ComputerControll_Attack::Exit(SoccerPlayer* s)
{

}
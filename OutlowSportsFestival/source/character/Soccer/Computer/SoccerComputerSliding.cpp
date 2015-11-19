#include "SoccerComputerAttack.h"
#include "../../CharacterFunction.h"
#include "../../CharacterManager.h"
#include "../SoccerPlayerState.h"
#include "SoccerComputerSliding.h"
#include "../../../Sound/Sound.h"
SoccerState_ComputerControll_Sliding::PlayerControllEvent::PlayerControllEvent(SoccerPlayer*const pSoccer) :
	m_pSoccer(pSoccer)
{

}
bool SoccerState_ComputerControll_Sliding::PlayerControllEvent::isDoCombo()
{
	return controller::GetTRG(controller::button::shikaku, m_pSoccer->m_PlayerInfo.number);
}
void SoccerState_ComputerControll_Sliding::PlayerControllEvent::AngleControll(RADIAN angle)
{
	const CharacterBase* const pTargetCharacter = GetFrontTargetEnemy();

	if (pTargetCharacter != nullptr)
	{
		//������]
		chr_func::AngleControll(m_pSoccer, pTargetCharacter->m_Params.pos, angle);
	}
	else
	{
		const Vector2 Stick = controller::GetStickValue(controller::stick::left, m_pSoccer->m_PlayerInfo.number);

		//�X�e�B�b�N�����ȏ�|����Ă��邩�ǂ���
		if (Vector2Length(Stick) > 0.25f)
		{
			Vector3 Vec(Stick.x, 0, Stick.y);

			//�X�e�B�b�N�l���J������Ԃ�
			Vec = Vector3MulMatrix3x3(Vec, matView);

			//�L�����N�^��]
			chr_func::AngleControll(m_pSoccer, m_pSoccer->m_Params.pos + Vec, angle);
		}
	}
}

const CharacterBase* SoccerState_ComputerControll_Sliding::PlayerControllEvent::GetFrontTargetEnemy()
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

	return pTargetEnemy;

}


SoccerState_ComputerControll_Sliding::SoccerState_ComputerControll_Sliding(SoccerPlayer* s) :
	m_Attack(s, new PlayerControllEvent(s), 10)
{

}


SoccerState_ComputerControll_Sliding::~SoccerState_ComputerControll_Sliding()
{

}
void SoccerState_ComputerControll_Sliding::Enter(SoccerPlayer* s)
{
	//�U���N���X�쐬
	SoccerAttackInfo_UsualAtk* pAtk;

	SoccerAttackInfo_UsualAtk::Param AtkParam[] =
	{
		{ 6, 1.0f, 1.5f, DamageBase::Type::_VanishDamage, 5, 22, 0.17f, 5, 10, SoccerPlayer::_ms_Rolling, 35, 20, 27, 35, 0, 2, D3DXToRadian(1), 18 },
	};
	Sound::Play(Sound::Sand2);

	for (int i = 0; i < (int)ARRAYSIZE(AtkParam); ++i)
	{
		pAtk = new SoccerAttackInfo_UsualAtk(s);

		pAtk->m_Param = AtkParam[i];

		m_Attack.m_AttackInfoArray.push_back(pAtk);
	}
}
void SoccerState_ComputerControll_Sliding::Execute(SoccerPlayer* s)
{
	m_Attack.Update();

	if (m_Attack.isEnd())
	{
		s->SetState(SoccerState_PlayerControll_Move::GetPlayerControllMove(s));
	}
}
void SoccerState_ComputerControll_Sliding::Exit(SoccerPlayer* s)
{

}
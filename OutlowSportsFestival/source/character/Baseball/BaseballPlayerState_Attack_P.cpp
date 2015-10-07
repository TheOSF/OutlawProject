#include "BaseballPlayerState_Attack_P.h"
#include "BaseballPlayerState.h"
#include "../../GameSystem/GameController.h"
#include "../CharacterFunction.h"
#include "../CharacterManager.h"
#include "BaseballAttackInfo_UsualAtk.h"


//------------�v���C���[����̍U������N���X--------------

Baseball_PlayerControll_Attack_P::PlayerControllEvent::PlayerControllEvent(BaseballPlayer*const pBaseball) :
m_pBaseball(pBaseball)
{

}

bool Baseball_PlayerControll_Attack_P::PlayerControllEvent::isDoCombo()
{
	return controller::GetTRG(controller::button::shikaku, m_pBaseball->m_PlayerInfo.number);
}

void  Baseball_PlayerControll_Attack_P::PlayerControllEvent::AngleControll(RADIAN angle)
{
	const CharacterBase* const pTargetCharacter = GetFrontTargetEnemy();

	if (pTargetCharacter != nullptr)
	{
		//������]
		chr_func::AngleControll(m_pBaseball, pTargetCharacter->m_Params.pos, angle);
	}
	else
	{
		const Vector2 Stick = controller::GetStickValue(controller::stick::left, m_pBaseball->m_PlayerInfo.number);

		//�X�e�B�b�N�����ȏ�|����Ă��邩�ǂ���
		if (Vector2Length(Stick) > 0.25f)
		{
			Vector3 Vec(Stick.x, 0, Stick.y);

			//�X�e�B�b�N�l���J������Ԃ�
			Vec = Vector3MulMatrix3x3(Vec, matView);

			//�L�����N�^��]
			chr_func::AngleControll(m_pBaseball, m_pBaseball->m_Params.pos + Vec, angle);
		}
	}
}

const CharacterBase*  Baseball_PlayerControll_Attack_P::PlayerControllEvent::GetFrontTargetEnemy()
{
	CharacterManager::CharacterMap ChrMap = DefCharacterMgr.GetCharacterMap();

	const float  AutoDistance = 10.0f;               //�������ł���ő勗��
	const RADIAN AutoMaxAngle = D3DXToRadian(90);   //�������ł���ő�p�x

	const CharacterBase* pTargetEnemy = nullptr;    //�^�[�Q�b�g�ێ��̃|�C���^
	RADIAN MostMinAngle = PI;                       //�����Ƃ������p�x
	RADIAN TempAngle;

	Vector3 MyFront;      //���g�̑O���x�N�g��
	chr_func::GetFront(m_pBaseball, &MyFront);

	auto it = ChrMap.begin();

	while (it != ChrMap.end())
	{
		//���g�����O
		if (m_pBaseball->m_PlayerInfo.number == it->first->m_PlayerInfo.number ||
			chr_func::isDie(it->first)
			)
		{
			++it;
			continue;
		}

		//���������ȏ�̃L�����N�^�����O����
		if (Vector3Distance(it->first->m_Params.pos, m_pBaseball->m_Params.pos) > AutoDistance)
		{
			it = ChrMap.erase(it);
			continue;
		}

		//�O�x�N�g���ƓG�ւ̃x�N�g���̊p�x���v�Z����
		TempAngle = Vector3Radian(MyFront, (it->first->m_Params.pos - m_pBaseball->m_Params.pos));

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

//-------------�ߋ����U���X�e�[�g�N���X-------------

Baseball_PlayerControll_Attack_P::Baseball_PlayerControll_Attack_P(BaseballPlayer* b) :
m_Attack(b, new PlayerControllEvent(b))
{

}


Baseball_PlayerControll_Attack_P::~Baseball_PlayerControll_Attack_P()
{

}

// �X�e�[�g�J�n
void  Baseball_PlayerControll_Attack_P::Enter(BaseballPlayer* b)
{
	//�U���N���X�쐬
	BaseballAttackInfo_UsualAtk* pAtk;

	BaseballAttackInfo_UsualAtk::Param AtkParam[] =
	{

		{ 4, 1.2f, 1.5f, DamageBase::Type::_WeekDamage, 2, 5, 0.07f, 5, 10, baseball_player::_mb_Atk1, 35, 20, 23, 35, 0, 15, D3DXToRadian(8), 8 },
		{ 6, 1.2f, 1.5f, DamageBase::Type::_VanishDamage, 5, 8, 0.02f, 1, 5, baseball_player::_mb_Atk2, 20, 5, 15, 20, 0, 5, D3DXToRadian(8), 8 },
		
	};

	for (int i = 0; i < (int)ARRAYSIZE(AtkParam); ++i)
	{
		pAtk = new BaseballAttackInfo_UsualAtk(b);

		pAtk->m_Param = AtkParam[i];

		m_Attack.m_AttackInfoArray.push_back(pAtk);
	}
}


// �X�e�[�g���s
void Baseball_PlayerControll_Attack_P::Execute(BaseballPlayer* b)
{
	m_Attack.Update();

	if (m_Attack.isEnd())
	{
		b->SetState(BaseballState_PlayerControll_Move::GetPlayerControllMove(b));
	}
}

// �X�e�[�g�I��
void Baseball_PlayerControll_Attack_P::Exit(BaseballPlayer* b)
{

}
#include "BaseballPlayerState_Attack_B.h"
#include "BaseballPlayerState.h"
#include "../../GameSystem/GameController.h"
#include "../CharacterFunction.h"
#include "../CharacterManager.h"
#include "BaseballAttackInfo_UsualAtk.h"


//------------�v���C���[����̍U������N���X--------------

Baseball_PlayerControll_Attack_B::PlayerControllEvent::PlayerControllEvent(BaseballPlayer*const pBaseball,BaseballAttackClass* Attack) :
m_pBaseball(pBaseball), m_Attack(Attack)
{

}

bool Baseball_PlayerControll_Attack_B::PlayerControllEvent::isDoCombo()
{
	//�@�R���s���[�^�[�Ȃ�
	if (m_pBaseball->m_PlayerInfo.player_type == PlayerType::_Computer)
	{
		return ComDoCombo();
	}
	return controller::GetTRG(controller::button::shikaku, m_pBaseball->m_PlayerInfo.number);
}

void  Baseball_PlayerControll_Attack_B::PlayerControllEvent::AngleControll(RADIAN angle)
{
	const CharacterBase* const pTargetCharacter = GetFrontTargetEnemy();

	if (pTargetCharacter == nullptr)
	{
		return;
	}
	
	//�@�R���s���[�^�[�Ȃ�
	if (m_pBaseball->m_PlayerInfo.player_type == PlayerType::_Computer)
	{
		//������]
		chr_func::AngleControll(m_pBaseball, pTargetCharacter->m_Params.pos, angle);
	}
	//�@�v���C���[�Ȃ�
	else
	{
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
}

const CharacterBase*  Baseball_PlayerControll_Attack_B::PlayerControllEvent::GetFrontTargetEnemy()
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
		if (TempAngle <= MostMinAngle)
		{
			pTargetEnemy = it->first;
			MostMinAngle = TempAngle;
		}

		++it;
	}

	
	//�@�R���s���[�^�Ȃ�
	if (pTargetEnemy != nullptr&&
		m_pBaseball->m_PlayerInfo.player_type == PlayerType::_Computer)
	{
		//������]
		chr_func::AngleControll(m_pBaseball, pTargetEnemy->m_Params.pos, 0.7f);
	}


	return pTargetEnemy;

}

//-------------�ߋ����U���X�e�[�g�N���X-------------

Baseball_PlayerControll_Attack_B::Baseball_PlayerControll_Attack_B(BaseballPlayer* b) :
m_Attack(b, new PlayerControllEvent(b,&m_Attack))
{

}


Baseball_PlayerControll_Attack_B::~Baseball_PlayerControll_Attack_B()
{

}

// �X�e�[�g�J�n
void  Baseball_PlayerControll_Attack_B::Enter(BaseballPlayer* b)
{
	//�U���N���X�쐬
	BaseballAttackInfo_UsualAtk* pAtk;

	BaseballAttackInfo_UsualAtk::Param AtkParam[] =
	{
        { 4, 1.8f, 4.0f, DamageBase::Type::_WeekDamage, 13, 18, 0.11f, 1, 5, baseball_player::_mb_Atk1_B,  39, 2, 15, 23, 0, 12, D3DXToRadian(8), 9, Vector2(0.2f, 0), false }, 
        { 4, 1.8f, 2.5f, DamageBase::Type::_VanishDamage, 21, 25, 0.04f, 1, 5, baseball_player::_mb_Atk2_B, 48, 18, 35, 37, 0, 12, D3DXToRadian(8), 17, Vector2(0.05f, 0.62f), false }, 
        { 10, 2.3f, 2.5f, DamageBase::Type::_VanishDamage, 18, 23, 0.05f, 1, 6, baseball_player::_mb_Atk3_B, 59, -1, -1, -1, 0, 8, D3DXToRadian(8), 17, Vector2(1.5f, 0.2f), true }, 
	};

	for (int i = 0; i < (int)ARRAYSIZE(AtkParam); ++i)
	{
		pAtk = new BaseballAttackInfo_UsualAtk(b);

		pAtk->m_Param = AtkParam[i];

		m_Attack.m_AttackInfoArray.push_back(pAtk);
	}
}


// �X�e�[�g���s
void Baseball_PlayerControll_Attack_B::Execute(BaseballPlayer* b)
{
	m_Attack.Update();

	if (m_Attack.isEnd())
	{
		b->SetState(BaseballState_PlayerControll_Move::GetPlayerControllMove(b));
	}
}

// �X�e�[�g�I��
void Baseball_PlayerControll_Attack_B::Exit(BaseballPlayer* b)
{
	
}

bool Baseball_PlayerControll_Attack_B::PlayerControllEvent::ComDoCombo()
{
	int FoolPoint = rand() % 100;
	switch (m_pBaseball->m_PlayerInfo.strong_type)
	{
	case StrongType::_Strong:
		if (m_Attack->getDoHit())
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
	case StrongType::_Usual:
		if (m_Attack->getDoHit())
		{
			if (FoolPoint > 75)
			{
				return false;
			}
			else
			{
				return true;
			}

		}
		else
		{
			if (FoolPoint > 75)
			{
				return true;
			}
			else
			{
				return false;
			}

		}
		break;
	case StrongType::_Weak:
		if (m_Attack->getDoHit())
		{

			if (FoolPoint > 50)
			{
				return false;
			}
			else
			{
				return true;
			}

		}
		else
		{
			if (FoolPoint > 50)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		break;
	default:
		return false;
		break;

	}
	return false;
}
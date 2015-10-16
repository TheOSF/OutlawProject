

#include "BaseballAttackClass.h"
#include "../CharacterFunction.h"
#include "Baseball_HitEvent.h"
#include "../../GameSystem/ResourceManager.h"

BaseballAttackClass::BaseballAttackClass(
	BaseballPlayer*   pOwner,
	ControllEvent*  pEvent
	) :
	m_pOwner(pOwner),
	m_pEvent(pEvent),
	m_Timer(0),
	m_ComboCount(-1),
	m_pStateFunc(&BaseballAttackClass::State_NextAtk),
	m_Locus(7)
{
	m_Damage.m_Enable = false;

	m_Locus.m_Division = 10;
	m_Locus.m_pTexture = DefResource.Get(Resource::TextureType::Locus1);

	m_Locus.m_StartParam.Color = Vector4(1, 1, 1, 1);
	m_Locus.m_StartParam.HDRColor = Vector4(1, 1, 1, 0.2f);

	m_Locus.m_EndParam.Color = Vector4(1, 1, 1, 0);
	m_Locus.m_EndParam.HDRColor = Vector4(1, 0, 1, 0);
}

BaseballAttackClass::~BaseballAttackClass()
{
	for (auto& it : m_AttackInfoArray)
	{
		delete it;
	}

	delete m_pEvent;
}

void BaseballAttackClass::Update()
{
	m_pOwner->m_Renderer.Update(1);

	(this->*m_pStateFunc)();


	//�L�����N�^�X�V
	{
		BaseballHitEvent HitEvent(m_pOwner);

		chr_func::UpdateAll(m_pOwner, &HitEvent);
	}

	chr_func::CreateTransMatrix(m_pOwner, m_pOwner->m_ModelSize, &m_pOwner->m_Renderer.m_TransMatrix);
}

bool BaseballAttackClass::isEnd()const
{
	return (m_pStateFunc == &BaseballAttackClass::State_End);
}

//----------------------------------------------------------------


void BaseballAttackClass::State_Attack()
{
	AttackInfo* const pNowAtk = m_AttackInfoArray.at(m_ComboCount);

	//�J�E���g�i�s
	++m_Timer;

	//���t���[���̍X�V
	{
		pNowAtk->Update(m_Timer, &m_Locus);
		pNowAtk->DamagePosSet(&m_Damage, m_pOwner);
	}

	//�p�x�X�V
	RADIAN ControllRadian = 0;

	if (pNowAtk->isCanAngleControll(m_Timer, ControllRadian))
	{
		m_pEvent->AngleControll(ControllRadian);

		{
			Vector3 m = m_pOwner->m_Params.move;
			const float l = Vector3XZLength(m);

			chr_func::GetFront(m_pOwner, &m);
			m *= l;
			m_pOwner->m_Params.move = m;
		}
	}

	//�R���{���s�t���O�̃`�F�b�N
	if (m_DoCombo == false &&
		pNowAtk->isComboButtonFrame(m_Timer))
	{
		m_DoCombo = m_pEvent->isDoCombo();
	}

	//���ύX�\��_(�K�E�Z��->����������ɂ���)
	//�R���{�ڍs
	if (!isLastAtk() &&
		m_DoCombo    &&
		pNowAtk->isComboSwitchFrame(m_Timer))
	{
		m_pStateFunc = &BaseballAttackClass::State_NextAtk;
	}

	//�_���[�W�L���E����
	m_Damage.m_Enable = pNowAtk->isDamageEnable(m_Timer);

	//�U���I��
	if (pNowAtk->isEnd(m_Timer))
	{
		m_pStateFunc = &BaseballAttackClass::State_End;
	}
}


//���̍U���Ɉڍs����
void BaseballAttackClass::State_NextAtk()
{
	MyAssert(!isLastAtk(), "�U�����N���X���Ȃ���Ԃ�BaseballAttackClass::State_NextAtk�����s����܂���");

	AttackInfo* const pNowAtk = m_AttackInfoArray.at(++m_ComboCount);

	pNowAtk->DamageParamSet(&m_Damage);
	pNowAtk->DamagePosSet(&m_Damage, m_pOwner);
	pNowAtk->MotionSet(m_pOwner);

	m_Timer = 0;
	m_DoCombo = false;

	m_pStateFunc = &BaseballAttackClass::State_Attack;
}

void BaseballAttackClass::State_End()
{

}


//�ŏI�i�U�����ǂ���
bool BaseballAttackClass::isLastAtk()
{
	return ((int)m_AttackInfoArray.size() - 1) <= m_ComboCount;
}
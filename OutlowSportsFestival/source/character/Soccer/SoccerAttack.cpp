
#include "SoccerAttackClass.h"
#include "../CharacterFunction.h"
#include "SoccerHitEvent.h"
#include "../../GameSystem/ResourceManager.h"

SoccerAttackClass::SoccerAttackClass(
	SoccerPlayer*   pOwner,
	ControllEvent*  pEvent
	) :
	m_pOwner(pOwner),
	m_pEvent(pEvent),
	m_Timer(0),
	m_ComboCount(-1),
	m_pStateFunc(&SoccerAttackClass::State_NextAtk),
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

SoccerAttackClass::~SoccerAttackClass()
{
	for (auto& it : m_AttackInfoArray)
	{
		delete it;
	}

	delete m_pEvent;
}

void SoccerAttackClass::Update()
{
	m_pOwner->m_Renderer.Update(1);

	(this->*m_pStateFunc)();


	//�L�����N�^�X�V
	{
		SoccerHitEvent HitEvent(m_pOwner);

		chr_func::UpdateAll(m_pOwner, &HitEvent);
	}

	chr_func::CreateTransMatrix(m_pOwner, m_pOwner->m_ModelSize, &m_pOwner->m_Renderer.m_TransMatrix);
}

bool SoccerAttackClass::isEnd()const
{
	return (m_pStateFunc == &SoccerAttackClass::State_End);
}

//----------------------------------------------------------------


void SoccerAttackClass::State_Attack()
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

	//�R���{�ڍs
	if (!isLastAtk() &&
		m_DoCombo    &&
		pNowAtk->isComboSwitchFrame(m_Timer))
	{
		m_pStateFunc = &SoccerAttackClass::State_NextAtk;
	}

	//�_���[�W�L���E����
	m_Damage.m_Enable = pNowAtk->isDamageEnable(m_Timer);

	//�U���I��
	if (pNowAtk->isEnd(m_Timer))
	{
		m_pStateFunc = &SoccerAttackClass::State_End;
	}
}


//���̍U���Ɉڍs����
void SoccerAttackClass::State_NextAtk()
{
	MyAssert(!isLastAtk(), "�U�����N���X���Ȃ���Ԃ�TennisAttackClass::State_NextAtk�����s����܂���");

	AttackInfo* const pNowAtk = m_AttackInfoArray.at(++m_ComboCount);

	pNowAtk->DamageParamSet(&m_Damage);
	pNowAtk->DamagePosSet(&m_Damage, m_pOwner);
	pNowAtk->MotionSet(m_pOwner);

	m_Timer = 0;
	m_DoCombo = false;

	m_pStateFunc = &SoccerAttackClass::State_Attack;
}

void SoccerAttackClass::State_End()
{

}


//�ŏI�i�U�����ǂ���
bool SoccerAttackClass::isLastAtk()
{
	return ((int)m_AttackInfoArray.size() - 1) <= m_ComboCount;
}
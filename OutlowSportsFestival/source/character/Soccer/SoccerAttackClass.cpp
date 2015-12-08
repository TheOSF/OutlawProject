
#include "SoccerAttackClass.h"
#include "../CharacterFunction.h"
#include "SoccerHitEvent.h"
#include "../../GameSystem/ResourceManager.h"
#include "../../GameSystem/GameSystem.h"

SoccerAttackClass::SoccerAttackClass(
	SoccerPlayer*   pOwner,
	ControllEvent*  pEvent
	) :
	m_pOwner(pOwner),
	m_pEvent(pEvent),
	m_Timer(0),
	m_ComboCount(-1),
	m_DoHit(false),
	m_pStateFunc(&SoccerAttackClass::State_NextAtk),
	m_Locus(7),
	m_DamageHitCount(0)
{
	m_Damage.m_Enable = false;

	m_Locus.m_Division = 10;
	m_Locus.m_pTexture = DefResource.Get(Resource::TextureType::Locus1);

	m_Locus.m_StartParam.Color = Vector4(1, 1, 1, 1);
	//m_Locus.m_StartParam.HDRColor = Vector4(1, 1, 1, 0.2f);

	m_Locus.m_EndParam.Color = Vector4(1, 1, 1, 0);
	//m_Locus.m_EndParam.HDRColor = Vector4(1, 0, 1, 0);
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


	//�R���{���s�t���O�̃`�F�b�N
	if (m_DoCombo == false &&
		pNowAtk->isComboButtonFrame(m_Timer))
	{
		m_DoCombo = m_pEvent->isDoCombo();

	}
	if (pNowAtk->isHitStopFrame())
	{
		SoccerHitEvent HitEvent(m_pOwner);

		//�ǂƂ̐ڐG����
		chr_func::CheckWall(m_pOwner);

		//���Ƃ̐ڐG����
		chr_func::CheckGround(m_pOwner);

		//�����蔻������
		chr_func::DamageCheck(m_pOwner, &HitEvent);

		pNowAtk->HitStopUpdate();

		return;
	}

	//�J�E���g�i�s
	++m_Timer;

	//���t���[���̍X�V
	{
		pNowAtk->Update(m_Timer, &m_Locus);
		pNowAtk->DamagePosSet(&m_Damage, m_pOwner);
	}
	//�U���q�b�g�֐��̌Ăяo��
	if (m_DamageHitCount != m_Damage.HitCount)
	{
		m_DamageHitCount = m_Damage.HitCount;
		pNowAtk->HitAttack(&m_Damage);
		m_DoHit = true;

		//    m_HitStopCount = 5;
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


	//�R���{�ڍs


	if (m_pOwner->m_PlayerInfo.player_type == PlayerType::_Player)
	{
		if (!isLastAtk() &&
			m_DoCombo    &&
			pNowAtk->isComboSwitchFrame(m_Timer))
		{
		
			m_pStateFunc = &SoccerAttackClass::State_NextAtk;
		}
	}
	else if (m_pOwner->m_PlayerInfo.player_type == PlayerType::_Computer)
	{
		if (!isLastAtk() &&
			m_DoCombo    &&
			pNowAtk->isComboSwitchFrame(m_Timer) &&
			m_DoHit==true)
		{
			m_pStateFunc = &SoccerAttackClass::State_NextAtk;
		}
	}

	//�_���[�W�L���E����
	m_Damage.m_Enable = pNowAtk->isDamageEnable(m_Timer);

	//�U���I��
	if (pNowAtk->isEnd(m_Timer))
	{
		m_pStateFunc = &SoccerAttackClass::State_End;
		m_DoHit = false;
	}


    //�L�����N�^�X�V
    {
        chr_func::UpdateAll(m_pOwner, &SoccerHitEvent(m_pOwner, pNowAtk->isCounterFrame(m_Timer)));
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

    m_Damage.ResetCounts();

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
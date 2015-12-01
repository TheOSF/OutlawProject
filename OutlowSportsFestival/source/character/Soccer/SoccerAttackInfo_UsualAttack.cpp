#include "SoccerAttackInfo_UsualAttack.h"
#include "../CharacterFunction.h"
#include "../../Sound/Sound.h"

#include "../../GameSystem/GameController.h"


SoccerAttackInfo_UsualAtk::SoccerAttackInfo_UsualAtk(
	SoccerPlayer* pOwner
	) :
	m_pOwner(pOwner)
{

}

SoccerAttackInfo_UsualAtk::~SoccerAttackInfo_UsualAtk()
{

}

//�_���[�W�p�����[�^���Z�b�g
void SoccerAttackInfo_UsualAtk::DamageParamSet(DamageShpere* pDmg)
{
	pDmg->HitCount = 0;
	pDmg->m_Param.size = m_Param.DamageSize;
	pDmg->pBall = nullptr;
	pDmg->pParent = m_pOwner;
	pDmg->type = m_Param.DamageType;
	pDmg->Value = m_Param.DamageValue;

	DamagePosSet(pDmg, m_pOwner);
}


//�_���[�W�ʒu���Z�b�g
void SoccerAttackInfo_UsualAtk::DamagePosSet(DamageShpere* pDmg, SoccerPlayer* pTennis)
{
	Matrix  BoneMat;
	Vector3 Forward;
	Vector3 Pos;

	m_pOwner->m_Renderer.GetWorldBoneMatirx(BoneMat, m_Param.LocusBoneNum);

	Pos = Vector3(BoneMat._41, BoneMat._42, BoneMat._43);

	Forward = Vector3(BoneMat._31, BoneMat._32, BoneMat._33);
	Forward.Normalize();

	pDmg->m_Param.pos = Pos + Forward * m_Param.DamagePosLength;

	// pDmg->vec = pDmg->m_Param.pos - pTennis->m_Params.pos;
	chr_func::GetFront(pTennis, &pDmg->m_Vec);
    pDmg->m_Vec.y = 0.5f;
	pDmg->m_VecPower.x = 0.25f;


	m_LocusPos = pDmg->m_Param.pos;
	m_LocusVec = Forward;
}


//���[�V�������Z�b�g
void SoccerAttackInfo_UsualAtk::MotionSet(SoccerPlayer* pTennis)
{
	pTennis->m_Renderer.SetMotion(m_Param.Motion);
}


//�����̃t���[�����U���I�����ǂ���
bool SoccerAttackInfo_UsualAtk::isEnd(int Frame)
{
	return Frame >= m_Param.AllFrame;
}


//�����̃t���[�����R���{�ڍs�t���[�����ǂ���
bool SoccerAttackInfo_UsualAtk::isComboSwitchFrame(int Frame)
{
	return
		Frame >= m_Param.ComboSwitchStartFrame&&
		Frame <= m_Param.ComboSwitchEndFrame;
}


//�����̃t���[�����A�R���{�̈ڍs����{�^����������t���[�����ǂ���
bool SoccerAttackInfo_UsualAtk::isComboButtonFrame(int Frame)
{
	return Frame >= m_Param.CanComboButtonPushFrame;
}


//�����̃t���[�����A�p�x�������s����t���[�����ǂ���
bool SoccerAttackInfo_UsualAtk::isCanAngleControll(int Frame, RADIAN& OutControllVal)
{
	OutControllVal = m_Param.AngleControllValue;

	return
		Frame >= m_Param.AngleControllStartFrame&&
		Frame <= m_Param.AngleControllEndFrame;
}


//�����̃t���[�����A�_���[�W����L���t���[�����ǂ���
bool SoccerAttackInfo_UsualAtk::isDamageEnable(int Frame)
{
	if (Frame == m_Param.DamageEnableStart)
	{
		Sound::Play(Sound::Swing2);
	}
	return
		Frame >= m_Param.DamageEnableStart&&
		Frame <= m_Param.DamageEnableEnd;
}


//���t���[���Ă΂��
void SoccerAttackInfo_UsualAtk::Update(int Frame, Locus* pLocus)
{
	pLocus->AddPoint(m_LocusPos, m_LocusVec);

	chr_func::XZMoveDown(m_pOwner, 0.1f);

	if (Frame >= m_Param.MoveStartFrame&&
		Frame <= m_Param.MoveEndFrame)
	{
		chr_func::AddMoveFront(m_pOwner, m_Param.MoveSpeed, 1000);
	}
}
//�U�������������Ƃ��ɌĂ΂��
void SoccerAttackInfo_UsualAtk::HitAttack(DamageShpere* pDmg)
{
	chr_func::AddSkillGauge(m_pOwner, pDmg->Value*0.01f);

	//�R���g���[����U��
	controller::SetVibration(
		5000,
		0.15f,
		m_pOwner->m_PlayerInfo.number
		);
}